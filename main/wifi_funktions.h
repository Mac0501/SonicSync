#ifndef WIFI_FUNCTIONS_H
#define WIFI_FUNCTIONS_H

extern String newHostname; // Declare the variable as extern

void setupAccessPoint();
void closeAccessPoint();
bool connectToWiFi(const char* ssid, const char* password, int tries = 10);
void disconnectFromWiFi();

#endif // WIFI_FUNCTIONS_H
