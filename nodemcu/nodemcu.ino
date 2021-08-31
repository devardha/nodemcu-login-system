#include <ESP8266WiFi.h>
#include <MFRC522.h>
#include <WebSocketsClient.h>
#include <SPI.h>

#define RST_PIN 5
#define SDA_PIN 4

WebSocketsClient webSocket;
MFRC522 mfrc522(SDA_PIN, RST_PIN);

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
  
  delay(1000);

  webSocket.begin("192.168.1.6", 8000, "/ws");

  webSocket.onEvent(webSocketEvent);

  webSocket.setReconnectInterval(5000);
  
  webSocket.enableHeartbeat(15000, 3000, 2);
  
   mfrc522.PCD_Init();
  Serial.println("mfrc522 ready");
}

void loop() {
  webSocket.loop();
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
    
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String content= "";
  byte letter;
  
  for (byte i = 0; i < mfrc522.uid.size; i++){
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();

  content.toUpperCase();
  
  if (content.substring(1) == "04 5C 71 BA 8F 61 80"){
    Serial.println("Authorized access");
    webSocket.sendTXT("RFID_SUCCESS");
  } else  {
    Serial.println("Access denied");
    webSocket.sendTXT("RFID_FAILED");
  } 
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
