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
  httpGet("webhook.site", 80, "/588c8963-21d7-479b-9f27-3b5906319e31");
  delay(2000);
}

bool sendCommand(String command, int tries, bool wait, long timeout, char token[]) {
  Serial.print("-> SEND: " + command);

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

bool httpGet(String host, int port, String url) {

  Serial.println("GET " + host + ":" + String(port) + " " + String(url));
  
  String data = "GET " + url + " HTTP/1.0";
  
  if(!sendCommand("AT+CIPSTART=\"TCP\",\""+ host + "\"," + String(port), 3, true, 3000, "OK")) return false;
  if(!sendCommand("AT+CIPSEND=" + String(data.length() + 4), 3, true, 1000, ">")) return false;
  esp8266.println(data + "\r\n");
  Serial.println("> " + data);
  Serial.println("> ");  
  
  if(!sendCommand("AT+CIPCLOSE=0", 10, true, 3000, "OK")) return false;

  return true;
}
