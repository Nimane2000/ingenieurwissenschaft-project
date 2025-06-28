#define TOUCH_PIN 7

void setup() {
  Serial.begin(115200);  
  pinMode(TOUCH_PIN, INPUT); 
  Serial.println("Touchsensor bereit! Bitte berühren...");
}

void loop() {
  int touchWert = analogRead(TOUCH_PIN);  // liefert Werte zwischen 0-1023 (Nichtberührung liegt der Wert meist nahe 0, bei Berührung springt der Wert deutlich höher (oft 100-1000))

  bool beruehrung = (touchWert > 50);// 50 ist hoch genug um zufällige Schwankungen zu ignorieren
  
  if (beruehrung) { 
    Serial.println("Berührung erkannt!"); 
  } else {
    Serial.println("Keine Berührung.");
  }

  delay(1000);
}
