#include "led_functions.h"
#include <utility>
#include <driver/i2s.h>
#include <arduinoFFT.h>
#include <limits>
#include <iostream>

using namespace std;

#define RXD_PIN 3  // Define RXD pin

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
LedEffect currentLedEffect = NO_EFFECT;
uint32_t globalColor = strip.Color(0, 0, 0);

void initializeLEDs() {
  strip.begin();
  strip.show();
  Serial.begin(9600);  // Start the serial communication
}

void setLedEffect(LedEffect effect) {
  currentLedEffect = effect;
  if (effect == NO_EFFECT) {
    setColor(globalColor);
  }
}

void turn_led_on_off() {
  if (globalColor == strip.Color(0, 0, 0) && currentLedEffect != RAINBOW_CYCLE) {
    currentLedEffect = NO_EFFECT;
    setColor(strip.Color(255, 255, 255));
  } else {
    currentLedEffect = NO_EFFECT;
    setColor(strip.Color(0, 0, 0));
  }
}

void setBrightness(uint8_t brightness) {
  brightness = constrain(brightness, 0, 255);

  strip.setBrightness(brightness);
  strip.show();
}

void setColor(uint32_t color) {

  if (currentLedEffect == NO_EFFECT) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
  }
  globalColor = color;
  strip.show();
}

void rainbowCycle(uint8_t wait) {
  static uint16_t j = 0;
  static uint32_t lastMillis = 0;

  if (millis() - lastMillis >= wait) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    j++;
    if (j >= 256 * 5) {
      j = 0;
    }
    lastMillis = millis();
  }
}

void runningLights(uint32_t color, uint8_t wait) {
  static int startIndex = 0;
  static int endIndex = 4;
  static uint32_t lastMillis = 0;

  if (millis() - lastMillis >= wait) {
    strip.setPixelColor(startIndex, 0);

    startIndex = (startIndex + 1) % strip.numPixels();
    endIndex = (endIndex + 1) % strip.numPixels();

    for (int i = startIndex; i != endIndex; i = (i + 1) % strip.numPixels()) {
      strip.setPixelColor(i, color);
    }
    strip.show();

    lastMillis = millis();
  }
}

void theaterChase(uint32_t color, uint8_t wait) {
  static int index = 0;
  static uint32_t lastMillis = 0;

  if (millis() - lastMillis >= wait) {
    for (int i = 0; i < strip.numPixels(); i++) {
      if ((i + index) % 3 == 0) {
        strip.setPixelColor(i, color);
      } else {
        strip.setPixelColor(i, 0);
      }
    }
    strip.show();
    index++;
    if (index >= 3) {
      index = 0;
    }
    lastMillis = millis();
  }
}











float bpm = 0;
float confidence = 0;

unsigned long lastUpdate = 0;
int ledState = LOW;







unsigned long spreadLast = millis();
int spreadStartPosition = 0;
uint32_t spreadColorColor;
const int fadeAmount = 5;                 // Adjust this value to control the speed of fading
bool spreading = false;                   // A flag to indicate if spreading is active
float lastBPM = 0.0;                      // To store the last BPM value
unsigned long lastTriggerTime = 0;        // To store the last time the effect was triggered
unsigned long lastConfidentUpdate = 0;    // To store the last time a confident BPM update was received
const unsigned long fadeOffDelay = 3000;  // Time in milliseconds to start fading off if no confident BPM is received

// Global variables for mode switching
bool modeSwitch = false;  // false for spreadColor mode, true for beatColorChange mode
unsigned long lastModeSwitchTime = 0;
const unsigned long modeSwitchInterval = 10000;  // 10 seconds

// Function for changing color on beat
void beatColorChange() {
  if (millis() - lastTriggerTime >= (60.0 / bpm) * 1000) {
    uint32_t randomColor = strip.Color(random(255), random(255), random(255));
    for (int i = 0; i < strip.numPixels(); ++i) {
      strip.setPixelColor(i, randomColor);
    }
    strip.show();
    lastTriggerTime = millis();
  }
}

// Function to fade the brightness of a pixel
uint32_t fadeColor(uint32_t color, int fadeValue) {
  int red = (color >> 16) & 0xFF;
  int green = (color >> 8) & 0xFF;
  int blue = color & 0xFF;

  red = max(0, red - fadeValue);
  green = max(0, green - fadeValue);
  blue = max(0, blue - fadeValue);

  return strip.Color(red, green, blue);
}

uint32_t reduceOpacity(uint32_t color, int percentage) {
  int red = ((color >> 16) & 0xFF) * percentage / 100;
  int green = ((color >> 8) & 0xFF) * percentage / 100;
  int blue = (color & 0xFF) * percentage / 100;

  return strip.Color(red, green, blue);
}

void spreadColor() {
  unsigned long spreadRate = spreading ? 5 : 100;  // 20 times slower for lower confidence (4 * 5)
  unsigned long distance = (millis() - spreadLast) / spreadRate;

  if (distance < strip.numPixels()) {
    uint32_t color = spreading ? spreadColorColor : reduceOpacity(spreadColorColor, 20);  // Reduced opacity for lower confidence
    strip.setPixelColor(spreadStartPosition + distance, color);
    strip.setPixelColor(spreadStartPosition - distance, color);
  }

  // Fade all pixels
  for (int i = 0; i < strip.numPixels(); ++i) {
    uint32_t currentColor = strip.getPixelColor(i);
    strip.setPixelColor(i, fadeColor(currentColor, fadeAmount));
  }

  strip.show();

  // Stop spreading when the effect reaches the end of the strip
  if (distance >= strip.numPixels()) {
    spreading = false;
  }
}


// Rest of the functions (fadeColor, spreadColor) remain the same...

void audioEffect(uint32_t color) {
  unsigned long currentMillis = millis();

  // Non-blocking Serial reading
  if (Serial.available() > 0) {
    String serialInput = Serial.readStringUntil('\n');  // Read the complete line
    int commaIndex = serialInput.indexOf(',');
    if (commaIndex != -1) {
      String bpmString = serialInput.substring(0, commaIndex);
      String confidenceString = serialInput.substring(commaIndex + 1);
      float newBPM = bpmString.toFloat();
      float newConfidence = confidenceString.toFloat();

      // Check for new input with sufficient confidence
      if (newConfidence >= 0.6) {
        bpm = newBPM;
        confidence = newConfidence;
        lastTriggerTime = 0;                  // Reset the trigger time
        lastConfidentUpdate = currentMillis;  // Update the last confident update time

        // Output BPM and confidence to the console
        Serial.print("BPM: ");
        Serial.print(bpm);
        Serial.print(" - Confidence: ");
        Serial.println(confidence);
      }
    }
  }
  // Mode switching logic
  if (currentMillis - lastModeSwitchTime > modeSwitchInterval) {
    modeSwitch = !modeSwitch;
    lastModeSwitchTime = currentMillis;
  }

  // Mode execution
  if (modeSwitch) {
    beatColorChange();  // Random color change on beat
  } else {
    double interval = (60.0 / bpm) * 1000;  // Convert BPM to milliseconds

    // Trigger effect based on BPM interval and confidence level
    if (confidence >= 0.6) {
      if (currentMillis - lastTriggerTime >= interval) {
        lastTriggerTime = currentMillis;
        spreadStartPosition = strip.numPixels() / 2;
        spreadColorColor = strip.Color(random(255), random(255), random(255));
        spreadLast = millis();
        spreading = true;
      }
    } else {
      // For lower confidence, trigger a much slower and less frequent spread
      if (random(1000) < 5) {  // 2.5% chance to start a new spread
        spreadStartPosition = random(strip.numPixels());
        spreadColorColor = strip.Color(random(255), random(255), random(255));
        spreadLast = millis();
        spreading = false;
      }
    }

    // Check if there has been no confident BPM update for more than 5 seconds
    if (currentMillis - lastConfidentUpdate > fadeOffDelay) {
      spreading = false;
    }

    spreadColor();  // Call spreadColor to handle the fading and spreading effect
  }
}





uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void handelLedEffects() {

  if ((currentLedEffect == RUNNING_LIGHTS || currentLedEffect == THEATER_CHASE || currentLedEffect == AUDIO_EFFECT) && globalColor == strip.Color(0, 0, 0)) {
    globalColor = strip.Color(255, 255, 255);
  }

  switch (currentLedEffect) {
    case RAINBOW_CYCLE:
      rainbowCycle(20);
      break;
    case RUNNING_LIGHTS:
      runningLights(globalColor, 50);
      break;
    case THEATER_CHASE:
      theaterChase(globalColor, 100);
      break;
    case AUDIO_EFFECT:
      audioEffect(globalColor);
      spreadColor();
      break;
    default:
      break;
  }
}
