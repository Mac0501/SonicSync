#include "web_server_functions.h"
#include <ESP8266WebServer.h>
#include "storage_functions.h"
#include "wifi_funktions.h"

ESP8266WebServer server(80); // Define the server object here

void setupWebServer() {
    // Set up web server

    //pages
    server.on("/", HTTP_GET, handleRoot);
    server.on("/controller", HTTP_GET, handleController);

    //api
    server.on("/add-wifi", HTTP_POST, handleAddWifi);
    server.on("/skip-wifi", HTTP_POST, handleSkipWifi);

    server.begin();
}


void handleRoot() {
    // Check the flag
    if (!readFlag()) {
        // Serve the HTML form for input
        String html = "<!DOCTYPE html><html><head>";
        html += "<style>body{display:flex;flex-direction:column;align-items:center;justify-content:center;height:100vh;margin:0;}.container{text-align:center;margin-bottom:20px;}input[type='text'],input[type='password']{width:200px;padding:10px;margin-bottom:10px;font-size:16px;}.container input[type='submit'],.container input[type='button']{background-color:#4CAF50;color:white;padding:14px 20px;margin:8px 0;border:none;cursor:pointer;font-size:16px;border-radius:12px;}.container input[type='submit']:hover,.container input[type='button']:hover{background-color:#45a049;}</style>";
        html += "</head><body>";
        html += "<div class='container'>";
        html += "<h1>Enter Wi-Fi Credentials</h1>";
        html += "<form method='post' action='/add-wifi' id='wifiForm'>";
        html += "SSID: <input type='text' name='ssid'><br>";
        html += "Password: <input type='password' name='password'><br>";
        html += "<input type='submit' value='Connect to Wifi'>";
        html += "</form></div>";
        html += "<div class='container'><input type='button' value='Continue without Wifi' onclick='skipWifi()'></div>";
        html += "<script>document.getElementById('wifiForm').onsubmit = function(event) {event.preventDefault();var formData = new FormData(this);fetch('/add-wifi', {method: 'POST',body: formData}).then(response => {if (response.ok) {return response.text();} throw new Error('Network response was not ok.');}).then(data => {alert(data);}).catch(error => {console.error('Error:', error);});};function skipWifi() {fetch('/skip-wifi', {method: 'POST'}).then(response => {if (response.ok) {return response.text();} throw new Error('Network response was not ok.');}).then(data => {alert(data);}).catch(error => {console.error('Error:', error);});}</script>";
        html += "</body></html>";

        server.send(200, "text/html", html);
    } else {
        // Redirect to /controller
        server.sendHeader("Location", "/controller", true);
        server.send(302, "text/plain", "");
    }
}

void handleController() {
    // HTML content for the controller page
    String html = "<!DOCTYPE html><html><head>";
    html += "<title>Controller Page</title>";
    html += "</head><body>";
    html += "<h1>Welcome to the Controller Page</h1>";
    html += "<p>This is the content of the controller page.</p>";
    html += "</body></html>";

    server.send(200, "text/html", html);
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
