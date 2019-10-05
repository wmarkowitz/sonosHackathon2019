#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

// CONSTANTS

#define LED 15
#define PIR 4

// const char*
String NETSSID = "<SSID>";         // The SSID (name) of the Wi-Fi network you want to connect to
String NETPASS = "<PASS>";     // The password of the Wi-Fi network

String ACCESS_TOKEN = "<ACCESS_TOKEN>";
String API_KEY = "<API_KEY>";
String PLAYER = "<PLAYER>";

// api.ws.sonos.com SSL fingerprint. Expires 3/16/2020.
const uint8_t SONOS_API_FINGERPRINT[20] = {0x21, 0x38, 0xBA, 0xA9, 0xB8, 0x82, 0x0B, 0x7D, 0x56, 0xA2, 0x4A, 0x9C, 0x05, 0x3A, 0xC7, 0xA8, 0x2D, 0x2F, 0xED, 0x7F};

typedef struct {
  String header_name;
  String header_value;
} header_t;

void setup() {
  // Setup pin I/O
  pinMode(LED, OUTPUT);
  pinMode(PIR, INPUT);
  
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

bool activated = false;
void loop() {
  int pir_val = digitalRead(PIR);
  
  if(pir_val == HIGH && !activated) {
    activated = true;
    Serial.println("\n===== MOTION DETECTED =====");
    Serial.println("Activating speaker.");

    digitalWrite(LED, HIGH);
    setVolume(50);    
  } else if (pir_val == LOW && activated) {
    activated = false;
    Serial.println("\n===== TIMEOUT =====");
    Serial.println("Deactivating speaker.");

    digitalWrite(LED, LOW);
    setVolume(0);
  }
  
  delay(1000);
}

bool setVolume(int volume) {
  Serial.println("[THAONOS] Set volume to " + String(volume) + "%");

  header_t http_header_names[] = {
    {"Authorization", "Bearer " + ACCESS_TOKEN},
    {"X-Sonos-Api-Key", API_KEY}
  };

  String data = "{\"volume\":" + String(volume) + "}";
  
  return httpPost(
    "https://api.ws.sonos.com/control/api/v1/players/" + PLAYER + "/playerVolume",
    SONOS_API_FINGERPRINT,
    http_header_names, 2,
    data
  );
}

bool httpPost(String url, const uint8_t fingerprint[20], header_t header_list[], int n_headers, String data) {
  Serial.print("[HTTPS] Initialize...\n");
  
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setFingerprint(fingerprint);
  HTTPClient https;

  Serial.print("[HTTPS] Begin " + url + "...\n");
  if (https.begin(*client, url)) {

    Serial.print("[HTTPS] Set headers...\n");
    for(int i = 0; i < n_headers; i++) {
      Serial.println("[HTTPS] - " + header_list[i].header_name + ": " + header_list[i].header_value);
      https.addHeader(header_list[i].header_name, header_list[i].header_value, false, false);
    }

    Serial.print("[HTTPS] POST...\n");
    // start connection and send HTTP header
    int httpCode = https.POST(data);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] POST: Success: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
        Serial.println("[HTTPS] Response: \n" + payload + "\n");
        Serial.println("[HTTPS] End Response.");

        return true;
      }
    } else {
      Serial.printf("[HTTPS] POST: Failed. Error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }

  return false;
}
