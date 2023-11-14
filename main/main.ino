#include "led_functions.h"
#include "wifi_funktions.h"
#include "web_server_functions.h"
#include "storage_functions.h"

void setup() {
    Serial.begin(9500);
    delay(2000);
    Serial.flush();
    Serial.println("Setup starts");

    InitializeFileSystem();

    setupWifi();
    setupWebServer();

    initializeLEDs();
    Serial.println("Setup finished");
}

void loop() {
    server.handleClient();

    handelLedEffects();

    delay(1);
}