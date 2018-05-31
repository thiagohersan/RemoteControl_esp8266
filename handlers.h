#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "codes.h"

const short IR_PIN = D2;
const short LED_PIN = D4;

ESP8266WebServer mServer(80);
IRsend mIRsend(IR_PIN);
const int COMMAND_DELAY_MILLIS = 500;

String buildHelpResponse() {
  DynamicJsonBuffer buttonBuffer(3072);
  JsonObject& mButtons = buttonBuffer.parseObject(codesJson);
  JsonObject& commands = mButtons["samsung"];

  String response = " com um json no seguinte formato:\n\
</h3>\n\
\n\
<pre>\n\
  {\n\
    \'type\': \'(LG | SAMSUNG)\',\n\
    \'commands\': [\n\
      \'COMMAND_0\',\n\
      \'COMMAND_1\',\n\
      \'COMMAND_2\',\n\
      ...\n\
    ]\n\
  }\n\
</pre>\n\
<h3>onde:<br>\
    <code style=\"color: #8A2BE2\">type</code> &eacute; o tipo de TV sendo testada (LG ou SAMSUNG)<br>\
    <code style=\"color: #8A2BE2\">commands</code> &eacute; uma lista com os seguintes poss&iacute;veis comandos:\
</h3>\n";

  response += "<ul>";
  for (auto command : commands) {
    response += "<li>" + String(command.key) + " </li>\n";
  }
  response += " </ul>";

  return response;
}

const String helpResponse = buildHelpResponse();

void handleNotFound() {
  String errorMessage = "<h2>404: " + mServer.uri() + " not found on this server</h2>\n\n";

  errorMessage += "<h3>Para enviar comandos para o controle, \
                       fa&ccedil;a um <code style=\"color: #d80000\">POST</code> &agrave; ";
  errorMessage += WiFi.localIP().toString();
  errorMessage += helpResponse;

  mServer.send(404, "text/html", errorMessage);
}

void handleRootGet() {
  String rootHelpResponse = "<h3>Para enviar comandos para o controle, \
                                 fa&ccedil;a um <code style=\"color: #d80000\">POST</code> &agrave; ";
  rootHelpResponse += WiFi.localIP().toString();
  rootHelpResponse += helpResponse;

  mServer.send(200, "text/html", rootHelpResponse);
}

void handleRootPost() {
  DynamicJsonBuffer buttonBuffer(3072);
  JsonObject& mButtons = buttonBuffer.parseObject(codesJson);

  DynamicJsonBuffer jsonBuffer(4096);
  JsonObject& root = jsonBuffer.parseObject(mServer.arg("plain"));

  if (!root.success()) {
    mServer.send(500, "text/plain", String("invalid json parameter"));
  }

  String remoteType = "lg";
  if (root["type"].as<String>() != "") {
    remoteType = root["type"].as<String>();
    remoteType.toLowerCase();
  }

  JsonArray& commands = root["commands"];
  for (auto cmd : commands) {
    JsonVariant code = mButtons[remoteType][cmd.as<String>()];
    if (code.success()) {
      digitalWrite(LED_PIN, LOW);
      if (remoteType == "samsung") {
        mIRsend.sendSAMSUNG(code.as<int>(), 32, 1);
      } else {
        mIRsend.sendNEC(code.as<int>(), 32, 1);
      }
      delay(COMMAND_DELAY_MILLIS);
      digitalWrite(LED_PIN, HIGH);
    }
  }

  mServer.send(200, "text/plain", String("success"));
}

void setupHandlers() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  mServer.on("/", HTTP_GET, handleRootGet);
  mServer.on("/", HTTP_POST, handleRootPost);
  mServer.onNotFound(handleNotFound);
}

