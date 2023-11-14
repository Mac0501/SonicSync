#ifndef WEB_SERVER_FUNCTIONS_H
#define WEB_SERVER_FUNCTIONS_H

#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WebServer.h>
extern ESP8266WebServer server;
#endif

#ifdef ESP32
#include <WebServer.h>
extern WebServer server;
#endif

void setupWebServer();

void handleRoot();
void handleController();
void handleAddWifi();
void handleSkipWifi();
void handleSetColor();
void handleSetEffect();
void handelLedTurnOnOff();
void handelSetBrightness();
void resetSettings();

#endif
