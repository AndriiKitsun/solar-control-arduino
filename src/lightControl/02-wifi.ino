void connectToWiFi() {
  Serial.print(F("Connecting to "));
  Serial.print(STASSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);

  while (!isWiFiConnected()) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println();
  Serial.print(F("Connected to WiFi with MAC: "));
  Serial.println(WiFi.macAddress());
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}
