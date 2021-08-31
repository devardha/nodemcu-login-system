#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <SPI.h>

WebSocketsClient webSocket;

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

  webSocket.begin("192.168.1.6", 8000, "/ws");

  webSocket.onEvent(webSocketEvent);

  webSocket.setReconnectInterval(5000);
  
  webSocket.enableHeartbeat(15000, 3000, 2);
}

void loop() {
  webSocket.loop();

  webSocket.send("Hello, wordl!");
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED: {
      Serial.printf("[WSc] Connected to url: %s\n", payload);
      webSocket.sendTXT("Connected");
    }
    break;
  }
}
