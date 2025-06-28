#define TASTER_PIN A6

bool letzterTasterZustand = HIGH; 

void setup() {
  pinMode(TASTER_PIN, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  bool aktuellerTasterZustand = analogRead(TASTER_PIN);
  
  // Taster wurde gedrückt
  if (letzterTasterZustand == HIGH && aktuellerTasterZustand == LOW) {
    Serial.println("Taster gedrückt");
    delay(200);
  }
  
  // Taster wurde losgelassen
  if (letzterTasterZustand == LOW && aktuellerTasterZustand == HIGH) {
    delay(200);
  }
  
  letzterTasterZustand = aktuellerTasterZustand; 
}