void setup() {
  pinMode(15, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  Serial.println("===== POLL =====");
  digitalWrite(15, HIGH);  // put your setup code here, to run once:
  delay(1000);
  digitalWrite(15, LOW);
  delay(1000);
}
