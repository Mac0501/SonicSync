#include "led_functions.h"
#include "wifi_funktions.h"
#include "web_server_functions.h"
#include "storage_functions.h"

void setup() {
    // Initialize serial communication for debugging
    Serial.begin(9500);
    delay(2000);
    Serial.flush();
    Serial.println("Setup starts");

    InitializeFileSystem();

    // Set up ESP8266 as an Access Point and start the web server
    setupAccessPoint();
    setupWebServer();

    // Initialize LED strip and set initial configurations
    initializeLEDs();
    Serial.println("Setup finished");
}

void loop() {
    // Handle web server requests
    server.handleClient();

    // Run led effects
    handelLedEffects();
}