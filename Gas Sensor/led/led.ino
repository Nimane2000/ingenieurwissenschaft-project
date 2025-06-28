#define led_pin 8

void setup() {
  pinMode(led_pin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Led-Steuerung gestartet");
}

void loop() {
  Serial.println("Led EIN (10 Sekunden)");
  digitalWrite(led_pin, HIGH);
  delay(10000);
  
  Serial.println("Led AUS (20 Sekunden)");
  digitalWrite(led_pin, LOW);
  delay(20000);
}