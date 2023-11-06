#include "web_server_functions.h"
#include <ESP8266WebServer.h>
#include "storage_functions.h"
#include "wifi_funktions.h"
#include "led_functions.h"
#include "pages.h"

ESP8266WebServer server(80); // Define the server object here

void setupWebServer() {
    // Set up web server

    //pages
    server.on("/", HTTP_GET, handleRoot);
    server.on("/controller", HTTP_GET, handleController);

    //api
    server.on("/add-wifi", HTTP_POST, handleAddWifi);
    server.on("/skip-wifi", HTTP_POST, handleSkipWifi);
    server.on("/setColor", HTTP_POST, handleSetColor);
    server.on("/setEffect", HTTP_POST, handleSetEffect);
    server.on("/on_off", HTTP_POST, handelLedTurnOnOff);
    server.on("/brightness", HTTP_POST, handelSetBrightness);

    server.begin();
}


void handleRoot() {
    // Check the flag
    if (!readFlag()) {
        // Serve the HTML form for input

        server.send(200, "text/html", rootPage);
    } else {
        // Redirect to /controller
        server.sendHeader("Location", "/controller", true);
        server.send(302, "text/plain", "");
    }
}

void handleController() {
    // HTML content for the controller page

    server.send(200, "text/html", controllerPage);
}


void handleAddWifi() {
    // Handle form submission and set up Wi-Fi
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    // Wait for connection
    bool success = connectToWiFi(ssid.c_str(), password.c_str());

    String response;
    if (success) {
        saveCredentials(ssid, password);
        saveFlag(true);
        response = "Okay, Connected to Wi-Fi: " + ssid;
        server.send(200, "text/plain", response);
        delay(2000);
        closeAccessPoint();
    } else {
        response = "Error, Unable to connect to Wi-Fi: " + ssid;
        server.send(400, "text/plain", response);
    }
    // You can add additional logic here, for example, redirect to a success page
}

void handleSkipWifi() {
    Serial.println("saveFlag");
    saveFlag(true);
    Serial.println("saveFlag");
    String response = "Skiped Wifi";
    server.send(200, "text/plain", response);

    // You can add additional logic here, for example, redirect to a success page
}

void handleSetColor() {
    String color = server.arg("color"); // Get the color value from the request

    // Extract RGB components from the string
    int startIndex = color.indexOf('(') + 1;
    int endIndex = color.indexOf(')');
    String rgbValues = color.substring(startIndex, endIndex);
    int commaIndex1 = rgbValues.indexOf(',');
    int commaIndex2 = rgbValues.lastIndexOf(',');

    // Parse individual RGB components
    int red = rgbValues.substring(0, commaIndex1).toInt();
    int green = rgbValues.substring(commaIndex1 + 1, commaIndex2).toInt();
    int blue = rgbValues.substring(commaIndex2 + 1).toInt();

    // Convert RGB components to 24-bit color value (0xRRGGBB)
    uint32_t parsedColor = (red << 16) | (green << 8) | blue;

    // Set the color using the parsed color value (assuming setColor function expects 24-bit color)
    setColor(parsedColor);

    // Send a response back to the client
    server.send(200, "text/plain", "Color set successfully");
}

void handleSetEffect() {
    String effect = server.arg("effect"); // Get the effect value from the request
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

    // Send a response back to the client
    server.send(200, "text/plain", "Effect set successfully");
}

void handelLedTurnOnOff() {

    turn_led_on_off();

    // Send a response back to the client
    server.send(200, "text/plain", "Turn On Off successfully");
}

void handelSetBrightness() {
    String brightness = server.arg("brightness");

    setBrightness(brightness.toInt());

    // Send a response back to the client
    server.send(200, "text/plain", "Turn On Off successfully");
}
