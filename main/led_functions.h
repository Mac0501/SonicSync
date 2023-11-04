#ifndef LED_FUNCTIONS_H
#define LED_FUNCTIONS_H

#include <Adafruit_NeoPixel.h>

#define PIN D4
#define NUM_LEDS 120

enum LedEffect {
    RAINBOW_CYCLE,
    COLOR_WIPE,
    THEATER_CHASE,
    SPARKLE,
    // Add more effects here if needed
    NO_EFFECT // Special value for no effect
};

extern Adafruit_NeoPixel strip;
extern LedEffect currentLedEffect;

void initializeLEDs();
void setLedEffect(LedEffect effect);
void setColor(uint32_t color);
void rainbowCycle(uint8_t wait);
void colorWipe(uint32_t color, uint8_t wait);
void theaterChase(uint32_t color, uint8_t wait);
void sparkle(uint32_t color, uint8_t density);
uint32_t Wheel(byte WheelPos);
void handelLedEffects();
// Declare other LED control functions here

#endif /* LED_FUNCTIONS_H */