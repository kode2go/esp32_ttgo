/*
Reference: https://www.youtube.com/watch?v=57JWlSQ6-v4
*/

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

WiFiMulti WiFiMulti;
HTTPClient ask;

// Blynk credentials
const char* BLYNK_AUTH_TOKEN = "_Tx2yYYTCFm4Q0tzfLZmc_87QBkEdxYt";
const int VIRTUAL_PIN = 4;

// Replace with your Bearer token and plant ID
const char* bearer_token = "7ZYCllL3hZkFkvKCZV6BeS9CNAY";
const char* plant_id = "258314";

char ssid[] = "x";
char pass[] = "x";

const char* code_version = "esp32_ttgo.cpp";
const char* device_num = "ttgo01";

TFT_eSPI tft=TFT_eSPI();

// Function to connect to WiFi
void connectToWiFi() {
  Serial.println("********** Connect ESP32 to WiFi.");
  Serial.println("Wait for WiFi... ");
  WiFiMulti.addAP(ssid, pass);
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Hello World",30,30,2);

  Serial.begin(115200);
  Serial.println("*****************************************************");
  Serial.println("Code version: ");
  Serial.println(code_version);
  Serial.println("Device Number: ");
  Serial.println(device_num);


}

// Function to extract SOC from JSON response
int extractSOC(const String& payload) {
  const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 50;
  DynamicJsonDocument doc(capacity);

  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return NAN;
  }

  // float soc = doc["data"]["soc"];
  int soc = static_cast<int>(doc["data"]["soc"]);
  return soc;
}

int getEnergyFlow(const char* bearer_token, const char* plant_id) {
  // Replace with the actual API endpoint for energy flow information
  String energy_flow_endpoint = "https://api.sunsynk.net/api/v1/plant/energy/" + String(plant_id) + "/flow";

  // Headers
  HTTPClient http;
  http.begin(energy_flow_endpoint.c_str());
  http.addHeader("Content-type", "application/json");
  http.addHeader("Accept", "application/json");
  http.addHeader("Authorization", "Bearer " + String(bearer_token));

  // Make GET request
  int httpCode = http.GET();

  // Check if the request was successful (status code 200)
  if (httpCode == HTTP_CODE_OK) {
    // Parse and extract SOC from the response
    String payload = http.getString();
    return extractSOC(payload);
  } else {
    // Print the error message
    Serial.println("Error: " + http.getString());
    return NAN;
  }

  http.end();
}

void loop() {
  // put your main code here, to run repeatedly:
    if (WiFiMulti.run() != WL_CONNECTED) {
    connectToWiFi();
  }

  int energy_flow_soc = getEnergyFlow(bearer_token, plant_id);

  if (!isnan(energy_flow_soc)) {
    // Add your logic or actions based on the energy flow information
    // For example: digitalWrite(LED_PIN, HIGH);
    Serial.print("Your battery State of Charge (SOC) is: ");
    Serial.print(energy_flow_soc);
    Serial.println("%");
  }
    // Perform HTTP GET request
  HTTPClient http;

  String BLYNK_API_ENDPOINT = "https://blynk.cloud/external/api/get?token=" + String(BLYNK_AUTH_TOKEN) + "&v" + String(VIRTUAL_PIN);

  http.begin(BLYNK_API_ENDPOINT);

  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    Serial.println(payload);
        // Display payload on TFT
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(10, 30);
    tft.print("G: ");
    tft.setCursor(10, 60);
    tft.print(payload);

    int middleX = tft.width() / 2;
    tft.drawFastVLine(middleX, 0, tft.height(), TFT_WHITE);

    tft.setCursor(170, 30);
    tft.print("S: ");
    tft.setCursor(170, 60);
    tft.print(energy_flow_soc);  // Replace randomValue with your actual random value

    // Parse and use the payload as needed
  } else {
    Serial.println("Error: " + String(httpCode));
  }

  http.end();

  delay(1000);
}

