#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#include "parameters.h"
#include "handlers.h"

void setup() {
  mIRsend.begin();
  Serial.begin(115200);
  Serial.println("\nSetup");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());

  for (int counter = 0; (WiFi.status() != WL_CONNECTED) && (counter < 50); counter++) {
    delay(600);
    Serial.println("Trying WiFi");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected\nIP: ");
    Serial.println(WiFi.localIP());

    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());

    if (MDNS.begin(SERVER_NAME.c_str())) {
      Serial.print("MDNS started: http://");
      Serial.println(SERVER_NAME);
    }
  }

  setupHandlers();
  mServer.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  mServer.handleClient();
}

