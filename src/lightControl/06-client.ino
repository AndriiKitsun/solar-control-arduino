unsigned long previousMillis = 0;

void streamPzemValues() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= POST_PZEM_INTERVAL) {
    previousMillis = currentMillis;

    jsonPOST("/sensors/pzem", getJsonPzemValues());
  }
}

void jsonPOST(String endpoint, String payload) {
  String url = SERVER_BASE_URL + endpoint;
  WiFiClient client;
  HTTPClient http;

  Serial.print(F("[HTTP] POST "));
  Serial.println(url);

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  // NB: HttpClient works synchronously
  int httpCode = http.POST(payload);

  if (httpCode > 0) {
    Serial.print(F("[HTTP] POST... status: "));
    Serial.println(httpCode);

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
      Serial.println(http.getString());
    }
  } else {
    Serial.print(F("[HTTP] POST... failed, error: "));
    Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();
}