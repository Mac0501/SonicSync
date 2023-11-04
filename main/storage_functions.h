#ifndef STORAGE_FUNCTIONS_H
#define STORAGE_FUNCTIONS_H

#include <Arduino.h>

bool saveCredentials(const String& ssid, const String& password);
bool readCredentials(String& ssid, String& password);
bool saveFlag(bool flag);
bool readFlag();
bool InitializeFileSystem();

#endif // STORAGE_FUNCTIONS_H