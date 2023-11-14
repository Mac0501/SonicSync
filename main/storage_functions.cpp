#include "storage_functions.h"
#include <FS.h>
#include <SPI.h>

bool InitializeFileSystem() {
  bool initok = false;

  #ifdef ESP8266
    SPI.begin();
    initok = SPIFFS.begin();
    
    if (!initok) {
      Serial.println("SPIFFS Dateisystem formatiert.");
      SPIFFS.format();
      initok = SPIFFS.begin();
    }

    if (!initok) {
      SPIFFS.format();
      initok = SPIFFS.begin();
    }
  #endif

  #ifdef ESP32
    if (SPIFFS.begin(true)) {
      Serial.println("SPIFFS Dateisystem formatiert.");
    }

    initok = SPIFFS.begin();
  #endif

  if (initok) {
    Serial.println("SPIFFS ist OK");
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

void deleteFiles() {
  // Delete credentials file
  if (SPIFFS.exists("/credentials.txt")) {
    SPIFFS.remove("/credentials.txt");
    Serial.println("Deleted credentials.txt");
  } else {
    Serial.println("credentials.txt not found");
  }

  // Delete flag file
  if (SPIFFS.exists("/flag.txt")) {
    SPIFFS.remove("/flag.txt");
    Serial.println("Deleted flag.txt");
  } else {
    Serial.println("flag.txt not found");
  }
}
