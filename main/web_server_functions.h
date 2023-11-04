#ifndef WEB_SERVER_FUNCTIONS_H
#define WEB_SERVER_FUNCTIONS_H

#include <ESP8266WebServer.h>

extern ESP8266WebServer server; // Declare the server object

void setupWebServer();
void handleRoot();
void handleAddWifi();
void handleSkipWifi();
void handleController();

#endif // WEB_SERVER_FUNCTIONS_H