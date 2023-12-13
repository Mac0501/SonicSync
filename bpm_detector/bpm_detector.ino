// SENDER ESP

#define TXD_PIN 1  // Define TXD pin




#include <driver/i2s.h>
#include <arduinoFFT.h>
#include <limits>
#include <iostream>
using namespace std;


// Connections to INMP441 I2S microphone
#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32

// Use I2S Processor 0
#define I2S_PORT I2S_NUM_0

// Define input buffer length and FFT size (must be power of 2)
#define BUFFER_LEN 128
#define FFT_SIZE BUFFER_LEN
#define SAMPLE_RATE 20000  // Hz

//Max Frequ: 10000Hz
//Min Frequ: 39Hz
//Time Intervall = 25,6ms

//Arraylength to analyze BPMs
#define BEAT_ARRAY_LEN 15
//Arraylength of last Measured BPM for errorcorrection
#define BPM_ARRAY_LEN 5

//Define The Average Values
#define AVG_AMNT 50
#define BIN_AMNT 5

//Beat Validation.
//The Time in mr that one detection should wait before boing able to detect again.
//With 120BPM 2 Times a second would be 500ms per beat
#define MIN_BEAT_DIST 150

//Inizialize actual bpm variables:
double bpm = 0;
double confidence = 0;
float avgConfidence = 0;
float timeConfidence = 0;
float commonConfidence = 0;

//Array for the freq bands of lastvalidbeats
unsigned long lastValidBeatTime[5];

// FFT variables
arduinoFFT FFT = arduinoFFT();
double vReal[FFT_SIZE];
double vImag[FFT_SIZE];
int16_t sBuffer[BUFFER_LEN];


//Variables for average caluclation
double freqAverage[BIN_AMNT][AVG_AMNT];
int averageIndex = 0;


//History Size For Ring Buffer
//IMPORTAINT! Rung Buffer Size must be larger that the longest Beat intervall, otherwise the average is not capable of achecking if the beat has beem broken
const int historySize = 80;
double amplitudeHistory[historySize][5];  // Assuming 5 bins as per your code
int currentHistoryIndex = 0;

// Function declarations
void i2s_install();
void i2s_setpin();

void setup() {
  // Start serial communication
  Serial.begin(9600);  // Start the serial communication


  // I2S microphone setup
  i2s_install();
  i2s_setpin();

  // Starting I2S driver
  i2s_start(I2S_PORT);

  //initalize the Array of lastvalidbeats to 0
  for (int i = 0; i < 5; i++) {
    lastValidBeatTime[i] = 0;
  }
}



//Timestamp to BPM calculation with Removal of wrong beats

//Indicated the index of the current Starting point of the BPM Array
short currentBPMindexStart = 0;

//BPM Array Initialisation
unsigned long BeatArray[BEAT_ARRAY_LEN];
unsigned int ProcessedBeatArray[BEAT_ARRAY_LEN - 1];


//Stores a Beat into the array
void storeBeat(unsigned long beatTimestamp) {
  BeatArray[currentBPMindexStart] = beatTimestamp;
  currentBPMindexStart = (currentBPMindexStart + 1) % BEAT_ARRAY_LEN;
}

//Finds the most frequent occuring Number in the ProcessedBeatArray
pair<unsigned int, int> findMostFrequent(unsigned int ProcessedBeatArray[]) {

  int maxCount = 0;
  int currentCount = 1;
  unsigned int mostFrequent = ProcessedBeatArray[0];

  for (int i = 1; i < (BEAT_ARRAY_LEN - 1); i++) {
    if (ProcessedBeatArray[i] == ProcessedBeatArray[i - 1]) {
      currentCount++;
    } else {
      if (currentCount > maxCount) {
        maxCount = currentCount;
        mostFrequent = ProcessedBeatArray[i - 1];
      }
      currentCount = 1;
    }
  }

  if (currentCount > maxCount) {
    maxCount = currentCount;
    mostFrequent = ProcessedBeatArray[(BEAT_ARRAY_LEN - 1) - 1];
  }

  return make_pair(mostFrequent, maxCount);
}



//Debug function to print raw Beat Array
void printRawBeatArray() {
  //Serial.print("RawBeatArray: ");
  for (int i = 0; i < BEAT_ARRAY_LEN; i++) {
    //Serial.print(BeatArray[i]);
    //Serial.print(" ");
  }
}

//Debug function to print raw Beat Array
void printProcessedBeatArray() {
  //Serial.print("ProcessedBeatArray: ");
  for (int i = 0; i < (BEAT_ARRAY_LEN - 1); i++) {
    //Serial.print(ProcessedBeatArray[i]);
    //Serial.print(" ");
  }
}

//Takes the raw Beat data and gives a sorted, and cleaned output where onyl the time distnaces between beats are present and sorted.
void processBeatRawData() {
  //Makeing the Array be the differnece between the differnet beats
  for (int i = 0; i < (BEAT_ARRAY_LEN - 1); i++) {
    unsigned long _temp = BeatArray[((currentBPMindexStart + i + 1) % BEAT_ARRAY_LEN)] - BeatArray[((currentBPMindexStart + i) % BEAT_ARRAY_LEN)];
    //only enter data if detection time is above 200 ms. (0,2s between beats = 5beats per second = 300 PPM, whic is useless)
    //Keep in mond that this is doen by the MIN_BEAT_DIST.

    //If BPM is bigger than 120, halve it
    if (_temp < 500) {
      _temp = _temp * 2;
    }
    if (_temp < 500) {
      _temp = _temp * 2;
    }
    //If BPM is smaller than 60, halve it in 4

    //multiply by 2 and devide by 2 to get rounded to even number
    ProcessedBeatArray[i] = _temp * 2;
    ProcessedBeatArray[i] = _temp / 2;
  }

  //Sort Array using cool  Standard function:
  std::sort(ProcessedBeatArray, ProcessedBeatArray + (BEAT_ARRAY_LEN - 1));
}

//Store Last BPM Measures in Ring Buffer
//Indicated the index of the current Starting point of the BPM Array
short BPMstoreIndex = 0;

//BPM Array Initialisation
float BPMArray[BPM_ARRAY_LEN];

//Stores a Beat into the array for error correction and confidence prediction
void storeBPM(float bpm) {
  BPMArray[BPMstoreIndex] = bpm;
  BPMstoreIndex = (BPMstoreIndex + 1) % BPM_ARRAY_LEN;
}

void printBPMArray() {
  //Serial.print("BPMArray: ");
  for (int i = 0; i < BPM_ARRAY_LEN; i++) {
    //Serial.print(BPMArray[i]);
    //Serial.print(" ");
  }
}

float averageConfidence(float number, float tolerance) {
  int closeCount = 0;

  for (int i = 0; i < BPM_ARRAY_LEN; ++i) {
    if (abs(BPMArray[i] - number) <= (float)tolerance) {
      closeCount++;
    }
  }
  //Catch if array is empty with only 0 at the start
  return static_cast<float>(closeCount) / BPM_ARRAY_LEN;
}

unsigned long lastTimeConfidenceTime = 0;
//Acually Calcuklates the BPM
void getBPM() {
  printRawBeatArray();
  processBeatRawData();
  printProcessedBeatArray();
  pair<unsigned int, int> mostCommon = findMostFrequent(ProcessedBeatArray);
  if (mostCommon.first != 0) {
    commonConfidence = (double)mostCommon.second / (double)(BEAT_ARRAY_LEN - 1);
    bpm = (float)60000 / (float)mostCommon.first;
  }

  //Halve Bom if its above 200, just because its annoying
  if (bpm > 200) {
    bpm = bpm / 2;
  }

  //Double BPM if its under 100, cause its annoying as hell
  if (bpm < 100) {
    bpm = bpm * 2;
  }

  storeBPM(bpm);
  //printBPMArray();
  //Calculate Confidence based on previouse beats ()
  //Remove obviousley false beats from being calculated (Fixes high confidence at beginning of initialisation)
  if (bpm > 60) {
    //Define how close a beat is counted to the average. 3BPPM off will be counted towards the average if (bpm, 3)
    avgConfidence = averageConfidence(bpm, 3);
  }

  //Time Confidence, based on how far ago the last beat was detected. up to1 Second ago: 100%

  if ((millis() - lastTimeConfidenceTime) < 2000) {
    timeConfidence = 1;
  } else {
    timeConfidence = (float)((float)1000 / (float)(millis() - lastTimeConfidenceTime));
  }
  lastTimeConfidenceTime = millis();
  confidence = ((commonConfidence + 2 * avgConfidence) / 3) * timeConfidence;
    Serial.print(bpm); // Send the counter value
  Serial.print(","); // Send a delimiter
  Serial.println(confidence); // Send the temperature value
}

/*void printBPM() {
  Serial.println("");
  Serial.print("BPM / Confidence: ");
  Serial.print(bpm);
  Serial.print(" / ");
  Serial.print(confidence);
  Serial.print("  -  (C: ");
  Serial.print(commonConfidence);
  Serial.print(" A: ");
  Serial.print(avgConfidence);
  Serial.print(" T: ");
  Serial.print(timeConfidence);
  Serial.print(")");
}*/



//Calculates the Average Amplitude from the History Ring Buffer
double getAverge(int binID) {
  double average;
  for (int i = 0; i < AVG_AMNT; i++) {
    average = average + freqAverage[binID][i];
  };
  return (average / AVG_AMNT);
}

unsigned long lastTime = 0;
void loop() {
  unsigned long currentTime = millis();
  lastTime = currentTime;  // Update the last time a FFT was done

  size_t bytesIn = 0;

  // Read from I2S
  esp_err_t result = i2s_read(I2S_PORT, &sBuffer, BUFFER_LEN * sizeof(int16_t), &bytesIn, portMAX_DELAY);

  if (result == ESP_OK && bytesIn == BUFFER_LEN * sizeof(int16_t)) {

    // Fill FFT input arrays
    for (int i = 0; i < FFT_SIZE; i++) {
      vReal[i] = sBuffer[i];
      vImag[i] = 0;
    }

    // Perform FFT
    //Reduce Spectral leakage as the signal is not "inifite"
    //FFT.Windowing(vReal, FFT_SIZE, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    // Comput the Actual Furier Transform
    FFT.Compute(vReal, vImag, FFT_SIZE, FFT_FORWARD);
    // Uses sqrt(real^2+imaginary^2) to get the acual magnitude
    FFT.ComplexToMagnitude(vReal, vImag, FFT_SIZE);

    // Calculate amplitudes for each frequency bin
    double amplitudes[5] = { 0 };  // Array to store amplitudes for each bin
    calculateAmplitudes(vReal, FFT_SIZE, SAMPLE_RATE, amplitudes);



    //Save All bins into averages
    for (int i = 0; i < BIN_AMNT; i++) {
      freqAverage[i][averageIndex] = amplitudes[i];
    };
    averageIndex = (averageIndex + 1) % AVG_AMNT;

    //Serial.print(getAverge(0));

    //Print the amplitudes for each frequency bin
    //Serial.print(" - ");
    //Serial.println(amplitudes[0]);
    //Serial.println(" ");
    /*if (getAverge(0) * 2 - amplitudes[0] < 0) {
      //If enough time has passed for another beat detection
      if (millis() - lastValidBeatTime[0] >= MIN_BEAT_DIST) {
        //set Last Valid beat to current time
        lastValidBeatTime[0] = millis();
        Serial.println("Bass Detected!");
        //Store the beat as Valid
        storeBeat(millis());
      }
    }*/
    if (getAverge(4) * 2 - amplitudes[4] < 0) {
      //If enough time has passed for another beat detection
      if (millis() - lastValidBeatTime[4] >= MIN_BEAT_DIST) {
        //set Last Valid beat to current time
        lastValidBeatTime[4] = millis();
        //Serial.println("Snare Detected!");
        //Store the beat as Valid
        storeBeat(millis());
        getBPM();
        //printBPM();
      }
    }


    //add the Amplitudes for all bin into the history Ring Buffer
    for (int i = 0; i < 5; i++) {
      amplitudeHistory[currentHistoryIndex][i] = amplitudes[i];
    }



    //Average Ring Boffer Increment
    currentHistoryIndex = (currentHistoryIndex + 1) % historySize;

  } else {
    //Serial.println("I2S read failed");
  }
}

void calculateAmplitudes(double *vReal, int fftSize, int sampleRate, double *amplitudes) {
  // Define frequency ranges
  int ranges[5][2] = { { 40, 120 }, { 120, 300 }, { 300, 1000 }, { 1000, 6500 }, { 6500, 10000 } };

  for (int range = 0; range < 5; range++) {
    int startFreq = ranges[range][0];
    int endFreq = ranges[range][1];
    int startIndex = (startFreq * fftSize) / sampleRate;
    int endIndex = (endFreq * fftSize) / sampleRate;

    double sumAmplitude = 0;
    int count = 0;

    for (int i = startIndex; i <= endIndex && i < fftSize / 2; i++) {
      sumAmplitude += vReal[i];
      count++;
    }

    amplitudes[range] = (count > 0) ? sumAmplitude / count : 0;
  }
}







void i2s_install() {
  // Set up I2S Processor configuration
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 128,
    .use_apll = false
  };

  // Install and start I2S driver
  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void i2s_setpin() {
  // Set I2S pin configuration
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  // Set I2S pins
  i2s_set_pin(I2S_PORT, &pin_config);
}
