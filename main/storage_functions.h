#ifndef STORAGE_FUNCTIONS_H
#define STORAGE_FUNCTIONS_H

#include <Arduino.h>

#ifdef ESP8266
#include <FS.h>
#define FILESYSTEM_TYPE SPIFFS
#endif

#ifdef ESP32
#include <SPIFFS.h>
#define FILESYSTEM_TYPE SPIFFS
#endif

bool saveCredentials(const String& ssid, const String& password);
bool readCredentials(String& ssid, String& password);
bool saveFlag(bool flag);
bool readFlag();
bool InitializeFileSystem();
void deleteFiles();

#endif
