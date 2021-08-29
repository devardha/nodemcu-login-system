#include <ESP8266WiFi.h>
#include <SPI.h>

const char* SSID = "";
const char* PASSWORD = ""; 

void setup() {
  Serial.begin(115200);
  SPI.begin();
  Serial.println();
  Serial.printf("Connecting to %s", SSID);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("WiFi connected");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
}

void loop() {
  
}
