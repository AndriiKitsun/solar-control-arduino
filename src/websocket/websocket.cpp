#include "websocket/websocket.h"

static WebSocketsServer webSocket(81);

void startWebSocket() {
  webSocket.begin();

  Serial.println(F("WebSocket started on:"));
  Serial.print(F("1. IP address: ws://"));
  Serial.print(WiFi.localIP());
  Serial.println(F(":81"));
  Serial.print(F("2. URL: "));
  Serial.println(F("ws://" ESP_DOMAIN_NAME ".local:81"));
}

void tickWebSocket() {
  webSocket.loop();
}

void broadcastPzems() {
  static unsigned long prevMillis;
  JsonDocument doc;

  if (millis() - prevMillis >= BROADCAST_INTERVAL) {
    prevMillis = millis();
    doc = getPzemsPayload();

    if (webSocket.connectedClients()) {
      String payload;

      serializeJson(doc, payload);

      webSocket.broadcastTXT(payload);
    }
  }
}
