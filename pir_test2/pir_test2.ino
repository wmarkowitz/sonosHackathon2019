#define PIR 7
#define LED 2

void setup() {
  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  digitalWrite(LED, digitalRead(PIR));
  delay(100);
}
