#include <SoftwareSerial.h>
#define RX 10
#define TX 11


SoftwareSerial esp8266(RX,TX);

String SSID = "<SSID>";
String PASS = "<PASS>";

void setup() {
  // Setup Serial
  Serial.begin(9600);
  esp8266.begin(115200);
  
  // Initialize esp8266 and connect to network
  sendCommand("AT+CWMODE=1", 10, true, 3000, "OK");
  
  // <ssid>, <password>
  sendCommand("AT+CWJAP=\"" + SSID + "\",\"" + PASS + "\"", 10, true, 10000, "IP");
}

bool speaker_active = false;

void loop() {    
  Serial.println("\n\n===== POLL =====\n");

  if(!sendCommand("AT+CIPSTART=\"TCP\",\"webhook.site\",80", 3, true, 3000, "OK")) return;
  if(!sendCommand("AT+CIPSEND=54", 3, true, 1000, ">")) return;
  esp8266.println("GET /588c8963-21d7-479b-9f27-3b5906319e31 HTTP/1.0\r\n");
  Serial.println("> GET /588c8963-21d7-479b-9f27-3b5906319e31 HTTP/1.0");
  Serial.println(">");  
  
  if(!sendCommand("AT+CIPCLOSE=0", 10, true, 3000, "OK")) return;

  delay(2000);
}

bool sendCommand(String command, int tries, bool wait, long timeout, char token[]) {
  bool found = false;
  
  Serial.print("SEND: " + command);

  for(int t = 0; t < tries; t++) {
      esp8266.println(command);
      Serial.print(" #");

      if(token != NULL) {
        if(wait) {
          long start = millis();
          while(millis() - start < timeout) {
            if(esp8266.find(token)) {
              Serial.println(" Success");
              return true;
            }
            Serial.print("~");
            delay(100);
          }
        } else {
          if(esp8266.find(token)) {
            found = true;
            Serial.println(" Success");
            return true;
          }
        }
      } else {
        Serial.println(" Pass");
        return true;
      }
  }

  Serial.println(" FAIL");
  return false;
}
