#include "storage_functions.h"
#include <FS.h>
#include <SPI.h>

bool InitializeFileSystem() {
  SPI.begin();
  bool initok = false;
  initok = SPIFFS.begin();
  
  if (!initok) { // Format SPIFS, if not formatted. - Try 1
    Serial.println("SPIFFS Dateisystem formatiert.");
    SPIFFS.format();
    initok = SPIFFS.begin();
  }

  if (!initok) { // Format SPIFS. - Try 2
    SPIFFS.format();
    initok = SPIFFS.begin();
  }

  if (initok) {
    Serial.println("SPIFFS ist  OK");
  } else {
    Serial.println("SPIFFS ist nicht OK");
  }
  
  return initok;
}

bool saveCredentials(const String& ssid, const String& password) {
    File file = SPIFFS.open("/credentials.txt", "w");
    if (!file) {
        return false;
    }
    file.println(ssid);
    file.println(password);
    file.close();
    return true;
}

bool readCredentials(String& ssid, String& password) {
    File file = SPIFFS.open("/credentials.txt", "r");
    if (!file) {
        return false;
    }
    ssid = file.readStringUntil('\n');
    password = file.readStringUntil('\n');
    file.close();
    ssid.trim();
    password.trim();
    return true;
}

bool saveFlag(bool flag) {
    File file = SPIFFS.open("/flag.txt", "w");
    if (!file) {
        return false;
    }
    file.println(flag);
    file.close();
    return true;
}

bool readFlag() {
    File file = SPIFFS.open("/flag.txt", "r");
    if (!file) {
        return false;
    }
    String flagString = file.readStringUntil('\n');
    file.close();
    return flagString.toInt() != 0;
}
