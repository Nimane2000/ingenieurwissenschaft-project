#include <Servo.h> 

Servo meinServo;  //  Servo-Objekts zur Steuerung des Servos

const int servoPin = 3;    
const int oeffnungsWinkel = 0;   
const int schliessWinkel = 90;   
const long warteZeit = 5000;     

void setup() {
  meinServo.attach(servoPin);  
  Serial.begin(115200);          // Initialisiert die serielle Kommunikation 
}

void loop() {
  // Tür öffnen
  Serial.println("Tür wird geöffnet..."); 
  meinServo.write(oeffnungsWinkel);  
  delay(warteZeit);                  

  // Tür schließen
  Serial.println("Tür wird geschlossen..."); 
  meinServo.write(schliessWinkel);   
  delay(warteZeit);                  
}