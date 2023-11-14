#include "led_functions.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
LedEffect currentLedEffect = NO_EFFECT;
uint32_t globalColor = strip.Color(0, 0, 0);

void initializeLEDs() {
    strip.begin();
    strip.show();
}

void setLedEffect(LedEffect effect) {
    currentLedEffect = effect;
    if (effect == NO_EFFECT){
      setColor(globalColor);
    }
}

void turn_led_on_off() {
    if (globalColor == strip.Color(0, 0, 0) && currentLedEffect != RAINBOW_CYCLE){
      currentLedEffect = NO_EFFECT;
      setColor(strip.Color(255, 255, 255));
    }
    else{
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

    if(currentLedEffect == NO_EFFECT){
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

void audioEffect(uint32_t color, uint8_t density) {
    return;
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
            audioEffect(globalColor, 10);
            break;
        default:
            break;
    }
}
