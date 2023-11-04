#include <ESP8266WiFi.h>
#include "wifi_funktions.h"
#include "storage_functions.h"
#include <Arduino.h>

String newHostname = "SonicSync";

void setupAccessPoint() {
    String ssid, password;

    // Check if credentials exist
    if (readCredentials(ssid, password)) {
        // Try to connect to Wi-Fi network
        // If connection fails, set up Access Point
        if (!connectToWiFi(ssid.c_str(), password.c_str(), 20)) {
            Serial.println("Setting up Access Point...");
            closeAccessPoint(); // Close any existing Access Point
            WiFi.mode(WIFI_AP);
            WiFi.softAP("MyESP8266AP", "password123");
            Serial.println("Access Point set up");
        }
    } else {
        // If no credentials exist, set up Access Point
        Serial.println("No credentials found. Setting up Access Point...");
        closeAccessPoint(); // Close any existing Access Point
        WiFi.mode(WIFI_AP);
        WiFi.softAP("MyESP8266AP", "password123");
        Serial.println("Access Point set up");
    }
}

void closeAccessPoint() {
    // Close Access Point
    WiFi.softAPdisconnect(true);
    Serial.println("Closed Access Point");
}

bool connectToWiFi(const char* ssid, const char* password, int tries) {
    int attempt = 0;
    // Connect to Wi-Fi network
    WiFi.hostname(newHostname.c_str());

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && attempt < tries) {
        delay(1000);
        attempt++;
        Serial.println("Connecting to WiFi...");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected to WiFi");
        return true;
    } else {
        Serial.println("Failed to connect to WiFi");
        return false;
    }
}

void disconnectFromWiFi() {
    // Disconnect from Wi-Fi network
    WiFi.disconnect();
    Serial.println("Disconnected from WiFi");
}