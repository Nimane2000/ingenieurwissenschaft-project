#include <Servo.h>

Servo meinServo; // Servo-Objekts

const int servoPin = 6;       
const int offenWinkel = 0;    
const int geschlossenWinkel = 360; 
const long warteZeit = 5000;   

void setup() {
  Serial.begin(115200);
  meinServo.attach(servoPin);  
  Serial.println("Fenstersteuerung initialisiert.");
}

void loop() {
  // Fenster öffnen
  Serial.println("Öffne Fenster...");
  meinServo.write(offenWinkel);
  delay(warteZeit);

  // Fenster schließen
  Serial.println("Schließe Fenster...");
  meinServo.write(geschlossenWinkel);
  delay(warteZeit);
}