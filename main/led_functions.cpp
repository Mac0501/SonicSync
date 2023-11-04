#include "led_functions.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
LedEffect currentLedEffect = NO_EFFECT;

void initializeLEDs() {
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void setLedEffect(LedEffect effect) {
    currentLedEffect = effect;
}

void setColor(uint32_t color) {
    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

void rainbowCycle(uint8_t wait) {
    static uint16_t j = 0; // Static variable to maintain state between function calls
    static uint32_t lastMillis = 0; // Variable to store the last millis value
    
    if (millis() - lastMillis >= wait) {
        for (uint16_t i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        j++;
        if (j >= 256 * 5) {
            j = 0; // Reset the rainbow cycle counter
        }
        lastMillis = millis(); // Update the lastMillis value
    }
}

void colorWipe(uint32_t color, uint8_t wait) {
    static int index = 0;
    static uint32_t lastMillis = 0;

    if (millis() - lastMillis >= wait) {
        strip.setPixelColor(index, color);
        strip.show();
        index++;
        if (index >= strip.numPixels()) {
            index = 0;
        }
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

void sparkle(uint32_t color, uint8_t density) {
    static uint32_t lastMillis = 0;

    if (millis() - lastMillis >= 20) {
        int numPixelsToLight = (strip.numPixels() * density) / 100;
        for (int i = 0; i < numPixelsToLight; i++) {
            int pixel = random(strip.numPixels());
            strip.setPixelColor(pixel, color);
        }
        strip.show();
        lastMillis = millis();
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
    switch (currentLedEffect) {
        case RAINBOW_CYCLE:
            rainbowCycle(20); // You can adjust the wait time (in milliseconds) as needed
            break;
        case COLOR_WIPE:
            colorWipe(strip.Color(255, 0, 0), 50); // Color wipe with red color and 50ms delay
            break;
        case THEATER_CHASE:
            theaterChase(strip.Color(0, 0, 255), 100); // Theater chase with blue color and 100ms delay
            break;
        case SPARKLE:
            sparkle(strip.Color(255, 255, 0), 10); // Sparkle effect with yellow color and 10% density
            break;
        default:
            // No effect or unknown effect, do nothing
            break;
    }
}