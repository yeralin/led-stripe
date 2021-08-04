#include "ESPmDNS.h"
#include "WiFi.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include <ArduinoJson.h>

//const char* ssid = "Dan’s iPhone";
const char* ssid = "IveSeenYouNaked";
const char* password = "15975345622d";
const char* hostname = "led-strip";

char payload[512];
StaticJsonDocument<512> json;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");            // access at ws://[esp ip]/ws
AsyncEventSource events("/events");  // event source (Server-Sent events)

extern void execute(const char* op);

void onNotFound(AsyncWebServerRequest* request) {
  // Handle Unknown Request
  request->send(404);
}

void sendData() {
  serializeJson(json, payload);
  ws.textAll(payload);
}

void processRequest(const char* data) {
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(json, data);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    // TODO: Send error back
  }
}

void handleWebSocketMessage(void* arg, uint8_t* data, size_t len) {
  AwsFrameInfo* info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len &&
      info->opcode == WS_TEXT) {
        processRequest((char *) data);
        const char* op = json["op"];
        Serial.println(op);
        execute(op);
  }
}

void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
             AwsEventType type, void* arg, uint8_t* data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.println("Connect");
      break;
    case WS_EVT_DISCONNECT:
      Serial.println("Disconnect");
      break;
    case WS_EVT_DATA:
      Serial.println("Received data");
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
      Serial.println("WebSocket pong");
      break;
    case WS_EVT_ERROR:
      Serial.println("WebSocket error");
      break;
  }
}

void setupServer() {
  int tries = 10;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && tries != 0) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    delay(1000);
    tries--;
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Failed to connect to Wi-Fi, changing to AP mode");
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP("LED_CHOPAN", password);
  }
  if (!MDNS.begin(hostname)) {
    delay(1000);
    Serial.println("Failed to set mDNS name");
  }
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  server.addHandler(&events);
  server.onNotFound(onNotFound);
  server.begin();
}