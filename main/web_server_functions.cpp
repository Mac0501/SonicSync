#include "web_server_functions.h"
#include "storage_functions.h"
#include "wifi_funktions.h"
#include "led_functions.h"
#include "pages.h"

#ifdef ESP8266
ESP8266WebServer server(80);
#endif

#ifdef ESP32
WebServer server(80);
#endif

void setupWebServer() {

    server.on("/", HTTP_GET, handleRoot);
    server.on("/controller", HTTP_GET, handleController);

    server.on("/add-wifi", HTTP_POST, handleAddWifi);
    server.on("/skip-wifi", HTTP_POST, handleSkipWifi);
    server.on("/setColor", HTTP_POST, handleSetColor);
    server.on("/setEffect", HTTP_POST, handleSetEffect);
    server.on("/on_off", HTTP_POST, handelLedTurnOnOff);
    server.on("/brightness", HTTP_POST, handelSetBrightness);
    server.on("/reset", HTTP_POST, resetSettings);

    server.begin();
}


void handleRoot() {
    if (!readFlag()) {
        server.send(200, "text/html", rootPage);
    } else {
        server.sendHeader("Location", "/controller", true);
        server.send(302, "text/plain", "");
    }
}

void handleController() {
    server.send(200, "text/html", controllerPage);
}


void handleAddWifi() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    IPAddress deviceIP;
    bool success = connectToWiFi(ssid.c_str(), password.c_str(), 10, &deviceIP);
    Serial.println(deviceIP);

    String response;
    if (success) {
        saveCredentials(ssid, password);
        saveFlag(true);
        response = "http://" + deviceIP.toString();
        server.send(200, "text/plain", response);
        delay(2000);
        closeAccessPoint();
    } else {
        response = "Error, Unable to connect to Wi-Fi: " + ssid;
        server.send(400, "text/plain", response);
    }
}

void handleSkipWifi() {
    Serial.println("saveFlag");
    saveFlag(true);
    Serial.println("saveFlag");
    String response = "Skiped Wifi";
    server.send(200, "text/plain", response);
}

void handleSetColor() {
    String color = server.arg("color");

    int startIndex = color.indexOf('(') + 1;
    int endIndex = color.indexOf(')');
    String rgbValues = color.substring(startIndex, endIndex);
    int commaIndex1 = rgbValues.indexOf(',');
    int commaIndex2 = rgbValues.lastIndexOf(',');

    int red = rgbValues.substring(0, commaIndex1).toInt();
    int green = rgbValues.substring(commaIndex1 + 1, commaIndex2).toInt();
    int blue = rgbValues.substring(commaIndex2 + 1).toInt();

    uint32_t parsedColor = (red << 16) | (green << 8) | blue;
    setColor(parsedColor);

    server.send(200, "text/plain", "Color set successfully");
}

void handleSetEffect() {
    String effect = server.arg("effect");
    if (effect == "RAINBOW_CYCLE") {
        setLedEffect(RAINBOW_CYCLE);
    } else if (effect == "RUNNING_LIGHTS") {
        setLedEffect(RUNNING_LIGHTS);
    } else if (effect == "THEATER_CHASE") {
        setLedEffect(THEATER_CHASE);
    } else if (effect == "AUDIO_EFFECT") {
        setLedEffect(AUDIO_EFFECT);
    } else {
        setLedEffect(NO_EFFECT);
    }

    server.send(200, "text/plain", "Effect set successfully");
}

void handelLedTurnOnOff() {

    turn_led_on_off();

    server.send(200, "text/plain", "Turn On Off successfully");
}

void handelSetBrightness() {
    String brightness = server.arg("brightness");

    setBrightness(brightness.toInt());

    server.send(200, "text/plain", "Turn On Off successfully");
}

void resetSettings() {
    deleteFiles();
    server.send(200, "text/plain", "Reseted Settings");
    disconnectFromWiFi();
    setupAccessPoint();
}
