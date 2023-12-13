#include "led_functions.h"
#include "wifi_funktions.h"
#include "web_server_functions.h"
#include "storage_functions.h"
#include <utility>
#include <Arduino.h>
using namespace std;

TaskHandle_t Task1;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.flush();
  Serial.println("Setup starts");

  InitializeFileSystem();
  setupWifi();
  setupWebServer();
  initializeLEDs();

}



void loop() {
  server.handleClient();
    handelLedEffects();
}
