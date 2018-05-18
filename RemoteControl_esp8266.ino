#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer mServer(80);
int buttonPressDurationMillis = 400;

#include "parameters.h"
#include "handlers.h"

const short LED_PIN = D4;

void setup() {
  pinMode(10, OUTPUT);
  Serial.begin(115200);
  Serial.println("\nSetup");
  pinMode(LED_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());

  for (int counter = 0; (WiFi.status() != WL_CONNECTED) && (counter < 50); counter++) {
    delay(600);
    Serial.println("Trying WiFi");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected\nIP: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin(SERVER_NAME.c_str())) {
      Serial.println("MDNS started");
    }
  }

  addAllHandlers();

  mServer.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  mServer.handleClient();
}

