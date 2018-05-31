#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include "parameters.h"
#include "handlers.h"

void setup() {
  Serial.begin(115200);
  Serial.println("\nSetup");

  WiFiManager wifiManager;
  wifiManager.setAPStaticIPConfig(IPAddress(10, 0, 1, 1), IPAddress(10, 0, 1, 1), IPAddress(255, 255, 255, 0));
  wifiManager.autoConnect("ESPAP");

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

  mIRsend.begin();
  setupHandlers();
  mServer.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  mServer.handleClient();
}

