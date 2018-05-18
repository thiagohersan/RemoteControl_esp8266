#include <ArduinoJson.h>

const String mButtons[] = {
  "POWER",
  "VOLUME_UP",
  "VOLUME_DOWN",
  "CHANNEL_UP",
  "CHANNEL_DOWN",
  "ARROW_UP",
  "ARROW_DOWN",
  "ARROW_LEFT",
  "ARROW_RIGHT"
};
const int mButtonsLength = sizeof(mButtons) / sizeof(String);

int getCommandIndex(String cmd) {
  for (int i = 0; i < mButtonsLength; i++) {
    if (cmd == mButtons[i]) return i;
  }
  return -1;
}

String buildHelpResponse() {
  String response = "\
<h2>Bem vindo ao servidor de controle " + SERVER_NAME + "</h2>\n\
<h3>Para enviar comandos para o controle, \
    fa&ccedil;a um POST a esse endere&ccedil;o com um json no seguinte formato:\n\
</h3>\n\
\n\
<pre>\n\
  {\n\
    \'pressDuration\': NNN,\n\
    \'commands\': [\n\
      \'COMMAND_0\',\n\
      \'COMMAND_1\',\n\
      \'COMMAND_2\',\n\
      ...\n\
    ]\n\
  }\n\
</pre>\n\
<h3>Onde <code>NNN</code> &eacute; a dura&ccedil;&atilde;o dos apertos de bot&otilde;es em milissegundos e\
    <code>[ \'COMMAND_0\', \'COMMAND_1\', \'COMMAND_2\', ... ]</code> &eacute; uma lista com os seguintes poss&iacute;veis comandos:\
</h3>\n";

  response += "<ul>";
  for (int i = 0; i < mButtonsLength; i++) {
    response += "<li>" + mButtons[i] + " </li>\n";
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
  String response = "{ ";

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(mServer.arg("plain"));

  if (!root.success()) {
    mServer.send(500, "text/plain", String("invalid json parameter"));
  }

  const int jsonPressDuration = root["pressDuration"];
  if (jsonPressDuration) {
    buttonPressDurationMillis = jsonPressDuration;
    response += "duration: " + String(buttonPressDurationMillis);
  }

  response += " commands: [";
  JsonArray& commands = root["commands"];
  for (auto cmd : commands) {
    int cmdIndex = getCommandIndex(cmd);
    if (cmdIndex > -1 && cmdIndex < mButtonsLength) {
      response += String(cmdIndex) + ", ";
    }
  }
  response += "] }";
  Serial.println(response);

  mServer.send(200, "text/plain", response);
}

void addAllHandlers() {
  mServer.on("/", HTTP_GET, handleRootGet);
  mServer.on("/", HTTP_POST, handleRootPost);
  mServer.onNotFound(handleNotFound);
}

