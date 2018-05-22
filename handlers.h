#include <ArduinoJson.h>

StaticJsonBuffer<384> buttonBuffer;
JsonObject& mButtons = buttonBuffer.createObject();

void setupButtonMappings() {
  mButtons["POWER"] = 0xE0E040BF;
  mButtons["VOLUME_UP"] = 0xE0E0E01F;
  mButtons["VOLUME_DOWN"] = 0xE0E0D02F;
  mButtons["CHANNEL_UP"] = 0xE0E048B7;
  mButtons["CHANNEL_DOWN"] = 0xE0E008F7;
  mButtons["ARROW_UP"] = 0xE0E006F9;
  mButtons["ARROW_DOWN"] = 0xE0E08679;
  mButtons["ARROW_LEFT"] = 0xE0E0A659;
  mButtons["ARROW_RIGHT"] = 0xE0E046B9;
  mButtons["ENTER"] = 0xE0E016E9;
  mButtons["RETURN"] = 0xE0E01AE5;
  mButtons["EXIT"] = 0xE0E0B44B;
}

const int mButtonsLength = mButtons.size();

String buildHelpResponse() {
  String response = "\
<h2>Bem vindo ao servidor de controle " + SERVER_NAME + "</h2>\n\
<h3>Para enviar comandos para o controle, \
    fa&ccedil;a um POST a esse endere&ccedil;o com um json no seguinte formato:\n\
</h3>\n\
\n\
<pre>\n\
  {\n\
    \'commands\': [\n\
      \'COMMAND_0\',\n\
      \'COMMAND_1\',\n\
      \'COMMAND_2\',\n\
      ...\n\
    ]\n\
  }\n\
</pre>\n\
<h3>Onde <code>[ \'COMMAND_0\', \'COMMAND_1\', \'COMMAND_2\', ... ]</code> &eacute; uma lista com os seguintes poss&iacute;veis comandos:\
</h3>\n";

  response += "<ul>";
  for (auto button : mButtons) {
    response += "<li>" + String(button.key) + " </li>\n";
  }
  response += " </ul>";

  return response;
}

const String helpResponse = buildHelpResponse();

void handleNotFound() {
  String errorMessage = "<h2>404: " + mServer.uri() + " not found on this server</h2>\n\n";
  errorMessage += helpResponse;
  mServer.send(404, "text/html", errorMessage);
}

void handleRootGet() {
  mServer.send(200, "text/html", helpResponse);
}

void handleRootPost() {
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(mServer.arg("plain"));

  if (!root.success()) {
    mServer.send(500, "text/plain", String("invalid json parameter"));
  }

  const int jsonPressDuration = root["pressDuration"];
  if (jsonPressDuration) {
    buttonPressDurationMillis = jsonPressDuration;
  }

  JsonArray& commands = root["commands"];
  for (auto cmd : commands) {
    JsonVariant code = mButtons[cmd.as<String>()];
    if (code.success()) {
      mIRsend.sendSAMSUNG(code.as<int>(), 32, 4);
      delay(buttonPressDurationMillis);
    }
  }

  mServer.send(200, "text/plain", String("success"));
}

void addAllHandlers() {
  setupButtonMappings();
  mServer.on("/", HTTP_GET, handleRootGet);
  mServer.on("/", HTTP_POST, handleRootPost);
  mServer.onNotFound(handleNotFound);
}

