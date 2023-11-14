#ifndef WIFI_FUNCTIONS_H
#define WIFI_FUNCTIONS_H

#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

#ifdef ESP32
#include <WiFi.h>
#endif

extern String newHostname;

void setupWifi();
void setupAccessPoint();
void closeAccessPoint();
bool connectToWiFi(const char* ssid, const char* password, int tries = 10, IPAddress* ipAddress = nullptr);
void disconnectFromWiFi();

#endif