#include <Servo.h>

const int rainSensorPin = A3;      
const int servoPin = 6;            

const int offenWinkel = 0;         
const int geschlossenWinkel = 360;  
const int regenSchwelle = 500;     // Schwellenwert für Regen, nahe 1023 -> kein Wasser, Nahe 0 -> viel Wasser
const long warteZeit = 5000;       // Zeit zwischen den Messungen (5 Sekunde)

Servo meinServo;                   // Servo-Objekt

void setup() {
  Serial.begin(115200);
  meinServo.attach(servoPin);
  Serial.println("Regensensor & Fenstersteuerung gestartet...");
}

void loop() {
  int regenWert = analogRead(rainSensorPin);

  if (regenWert < regenSchwelle) {
    Serial.println("Regen erkannt! Fenster wird geschlossen.");
    Serial.println(regenWert);
    meinServo.write(geschlossenWinkel);  // Fenster schließen
  } else {
    Serial.println("Kein Regen! Fenster wird geöffnet.");
    Serial.println(regenWert);
    meinServo.write(offenWinkel);        // Fenster öffnen
  }

  delay(warteZeit);
}
