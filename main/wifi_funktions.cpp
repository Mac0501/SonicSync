#include "wifi_funktions.h"
#include "storage_functions.h"
#include <Arduino.h>

String newHostname = "SonicSync";

void setupWifi() {
    String ssid, password;

    if (readCredentials(ssid, password)) {
        if (!connectToWiFi(ssid.c_str(), password.c_str(), 20)) {
            Serial.println("Setting up Access Point...");
            closeAccessPoint();
            setupAccessPoint();
        }
    } else {
        Serial.println("No credentials found. Setting up Access Point...");
        closeAccessPoint();
        setupAccessPoint();
    }
}

void setupAccessPoint() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("SonicSync AP", "password123");
    Serial.println("Access Point set up");
}

void closeAccessPoint() {
    WiFi.softAPdisconnect(true);
    Serial.println("Closed Access Point");
}

bool connectToWiFi(const char* ssid, const char* password, int tries, IPAddress* ipAddress) {
    int attempt = 0;
    WiFi.hostname(newHostname.c_str());

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && attempt < tries) {
        delay(1000);
        attempt++;
        Serial.println("Connecting to WiFi...");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected to WiFi");
        if (ipAddress != nullptr) {
            *ipAddress = WiFi.localIP();
        }
        return true;
    } else {
        Serial.println("Failed to connect to WiFi");
        return false;
    }
}

void disconnectFromWiFi() {
    WiFi.disconnect();
    Serial.println("Disconnected from WiFi");
}
