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
  sendCommand("AT+CWMODE=1", 5, "OK");
  
  // <ssid>, <password>
  sendCommand("AT+CWJAP=\"" + SSID + "\",\"" + PASS + "\"", 10, "OK");
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

      String data = "GET /9f9ba9bf-485d-4b84-9b6d-5720af9f2caa";
      sendCommand("AT+CIPSTART=\"TCP\",\"webhook.site\",80", 1, "OK");
      sendCommand("AT+CIPSEND=" + String(data.length() + 4), 1, ">");
      esp8266.println(data);
      Serial.println(data);
      sendCommand("AT+CIPCLOSE=0", 1, "OK");

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

void sendCommand(String command, int tries, char readReplay[]) {
  bool found = false;
  
  Serial.print("SEND: " + command);

  for(int t = 0; t < tries; t++) {
    Serial.print(" |");
    esp8266.println(command);

    break;
    
//    for(int k = 0; k < 10; k++) {      
//      if (esp8266.find(readReplay))
//      {
//        found = true;
//        break;
//      } else {
//        Serial.print(" .");
//      }
//
//      delay(100);
//    }
//
//    if(found) break;
  }

  String buffer = "";
  while(esp8266.available()) {
    char in = esp8266.read();
    buffer += in;
  }
  Serial.println(buffer);
  
  Serial.println("\n\n----------------");
  
//  if(found == true)
//  {
//    Serial.println(" -> SUCCESS");
//  } else {
//    Serial.println(" -> FAIL");
//  }


  delay(1000);
}
