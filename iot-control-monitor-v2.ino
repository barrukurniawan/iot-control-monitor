#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Parameters
const char* ssid = "gigel.id";
const char* password = "cisanggiri14";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
}

void loop() {
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
      monitoring();
      controlling();
  }
}

void monitoring(){
  Serial.println("start send data");
  HTTPClient http;
  int nilai = random(1,6);
  String data = "BARRU276/" + (String) nilai;
  String link = "https://kurniawanbarru.pythonanywhere.com/sensor_eog/" + data;
  Serial.println(link);
  http.begin(link,"16 BA B9 9E 77 4C FF 0B 25 07 DE F9 AC 1A 23 E3 1A B2 AE EB");
  int httpMonitoring = http.GET();
  http.end();
  delay(500);
    
}

void controlling(){
    HTTPClient http;  //Object of class HTTPClient
    http.begin("https://kurniawanbarru.pythonanywhere.com/api_control/","16 BA B9 9E 77 4C FF 0B 25 07 DE F9 AC 1A 23 E3 1A B2 AE EB");
    int httpCode = http.GET();
    //Check the returning code                                                                  
    if (httpCode > 0) {
      // Parsing
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + 50;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      // Parameters
      const char* time = root["time"]; // "2019-03-03T10:22:35Z"
      float eog_value = root["eog_value"];
      Serial.print("nilai eog :");
      if (eog_value == 0.0 ) {
        Serial.println(eog_value);
        Serial.println("lampu mati");
        }
      else if (eog_value == 1.0 ) {
        Serial.println(eog_value); 
        Serial.println("lampu nyala");
        }
      else {
        Serial.println(eog_value);
        Serial.println("lampu kedap kedip");
      }
    }
    http.end();   //Close connection
    delay(500);
}
