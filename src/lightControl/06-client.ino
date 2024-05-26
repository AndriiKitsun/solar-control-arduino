unsigned long previousMillis = 0;

void streamPzemValues() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= POST_PZEM_INTERVAL) {
    previousMillis = currentMillis;

    // jsonPOST(POST_PZEM_ENDPOINT, pzemToJson(getPzemValues()));
  }
}

void jsonPOST(String endpoint, String payload) {
  WiFiClient client;
  HTTPClient http;

  Serial.print(F("[HTTP] POST "));
  Serial.println(endpoint);

  http.begin(client, endpoint);
  http.addHeader("Content-Type", "application/json");

  // NB: HttpClient works synchronously
  int httpCode = http.POST(payload);

  if (httpCode > 0) {
    Serial.print(F("[HTTP] POST... status: "));
    Serial.println(httpCode);

    Serial.print(F("[HTTP] POST... response: "));
    Serial.println(http.getString());
  } else {
    Serial.print(F("[HTTP] POST... failed, error: "));
    Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();
}
