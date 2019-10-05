#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

// CONSTANTS

#define LED 15

// const char*
String NETSSID = "<SSID>";         // The SSID (name) of the Wi-Fi network you want to connect to
String NETPASS = "<PASS>";     // The password of the Wi-Fi network

String ACCESS_TOKEN = "<ACCESS_TOKEN>";
String API_KEY = "<API_KEY>";
String PLAYER = "<PLAYER>";


// api.ws.sonos.com SSL fingerprint. Expires 3/16/2020.
const uint8_t SONOS_API_FINGERPRINT[20] = {0x21, 0x38, 0xBA, 0xA9, 0xB8, 0x82, 0x0B, 0x7D, 0x56, 0xA2, 0x4A, 0x9C, 0x05, 0x3A, 0xC7, 0xA8, 0x2D, 0x2F, 0xED, 0x7F};

void setup() {
  pinMode(LED, OUTPUT);
  
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  Serial.println("Welcome to THAONOS v0.0.1");
  Serial.println("-------------------------");
  
  WiFi.begin(NETSSID, NETPASS);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(NETSSID); Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print('.');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}

void loop() {

  int volume = random(50);
  Serial.println("\n===== TEST =====");
  Serial.println("-> Set speaker volume to " + String(volume) + "%");
  
  digitalWrite(LED, HIGH);

  Serial.print("[HTTPS] Initialize...\n");
    
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setFingerprint(SONOS_API_FINGERPRINT);
  HTTPClient https;

  Serial.print("[HTTPS] begin...\n");
  if (https.begin(*client, "https://api.ws.sonos.com/control/api/v1/players/" + PLAYER + "/playerVolume")) {

    Serial.print("[HTTPS] Set headers...\n");

    https.addHeader("Authorization", "Bearer " + ACCESS_TOKEN, false, false);
    https.addHeader("Host", "api.ws.sonos.com", false, false);
    https.addHeader("X-Sonos-Api-Key", API_KEY, false, false);

    Serial.print("[HTTPS] POST...\n");
    // start connection and send HTTP header
    int httpCode = https.POST("{\"volume\":" + String(volume) + "}");

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }

  Serial.println("Wait 10s before next round...");

  digitalWrite(LED, LOW);
  delay(10000);
}
