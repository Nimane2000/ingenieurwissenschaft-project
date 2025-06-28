#define buzzer 15

void setup() {
  pinMode(buzzer, OUTPUT);
  Serial.begin(115200);
  Serial.println("Buzzer-Steuerung gestartet");
}

void loop() {
  Serial.println("Buzzer EIN (10 Sekunden)");
  digitalWrite(buzzer, HIGH);
  delay(10000);
  
  Serial.println("Buzzer AUS (20 Sekunden)");
  digitalWrite(buzzer, LOW);
  delay(20000);
}
