#include <SoftwareSerial.h>
#define RX 10
#define TX 11
#define PIR 7
#define LED 13


SoftwareSerial esp8266(RX,TX);

String SSID = "<SSID>";
String PASS = "<PASS>";

void setup() {
  // Setup PIR sensor
  pinMode(PIR, INPUT);
  
  // Setup debug LED
  pinMode(LED, OUTPUT);

  // Setup Serial
  Serial.begin(9600);
  esp8266.begin(115200);
  
  // Initialize esp8266 and connect to network
  sendCommand("AT+CWMODE=1", 5, true, "OK");
  
  // <ssid>, <password>
  sendCommand("AT+CWJAP=\"" + SSID + "\",\"" + PASS + "\"", 10, true, "IP");
}

bool speaker_active = false;

void loop() {  
  Serial.println("-");
  
  int pirval = digitalRead(PIR);
  if(pirval == HIGH) {
    if(!speaker_active) {
      Serial.println("MOTION DETECTED!");
      digitalWrite(LED, HIGH);
  
      Serial.println("-> Activate speaker.");

      sendCommand("AT+CIPSTART=\"TCP\",\"webhook.site\",80", 1, true, "OK");
      sendCommand("AT+CIPSEND=54", 1, false, "");
      esp8266.println("GET /9f9ba9bf-485d-4b84-9b6d-5720af9f2caa HTTP/1.0");
      Serial.println("> GET /9f9ba9bf-485d-4b84-9b6d-5720af9f2caa HTTP/1.0");
      esp8266.println();
      //esp8266.println("Host: webhook.site");
      //Serial.println("> Host: webhook.site");
      
      
      sendCommand("AT+CIPCLOSE=0", 10, true, "OK");

      speaker_active = true;
    }
  } else {
    if(speaker_active) {
      Serial.println("motion timeout");
      digitalWrite(LED, LOW);
      
      Serial.println("-> Deactivate speaker.");

      speaker_active = false;
    }
  }
  delay(1000);
}

void sendCommand(String command, int tries, bool wait, char token[]) {
  bool found = false;
  
  Serial.print("SEND: " + command);

  for(int t = 0; t < tries; t++) {
      esp8266.println(command);

      if(wait) {
        delay(100);
//        if(buffer.indexOf(token) > -1) {
        if(esp8266.find(token)) {
          found = true;
          Serial.println(" Success");
          return;
        } else {
          Serial.print(" .");
        }
      } else {
        Serial.println(" Pass");
        return;
      }
  }

  Serial.println(" FAIL");
}
