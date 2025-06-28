#include <Adafruit_NeoPixel.h>

#define TOUCH_PIN 7       
#define LED_PIN 5         
#define LED_ANZAHL 4      

Adafruit_NeoPixel led(LED_ANZAHL, LED_PIN, NEO_GRB + NEO_KHZ800);

// Variablen
bool ledsAn = false;
unsigned long letzteAenderung = 0;
int aktuelleFarbe = 0;

// Alle Farben 
const int farben[][4] = {
  {255, 0, 0, "Rot"},         
  {255, 165, 0, "Orange"},    
  {255, 255, 0, "Gelb"},      
  {0, 255, 0, "Grün"},        
  {0, 255, 255, "Cyan"},      
  {0, 0, 255, "Blau"},        
  {128, 0, 128, "Lila"},      
  {255, 105, 180, "Rosa"},    
  {255, 255, 255, "Weiß"}     
};
const int farbIntervall = 1000; // Farbwechsel alle Sekunde

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);
  led.begin();
  led.show();
  Serial.println("Bereit. Berühren Sie den Sensor.");
}

void loop() {
  // Touch-Sensor lesen
  int touchWert = analogRead(TOUCH_PIN);
  bool beruehrung = (touchWert > 50);   // Schwellenwert
  
  // Bei Berührung umschalten
  if (beruehrung) {
    ledsAn = !ledsAn;
    if (ledsAn) {
      aktuelleFarbe = 0;
      farbeAnzeigen(aktuelleFarbe);
      Serial.println("LEDs AN");
    } else {
      ausschalten();
    }
    delay(300); // Entprellung
    while (analogRead(TOUCH_PIN) > 50) {} // Warten bis Loslassen
  }

  // Automatischer Farbwechsel
  if (ledsAn && millis() - letzteAenderung >= farbIntervall) {
    aktuelleFarbe = (aktuelleFarbe + 1) % 9; // 9 Farben insgesamt
    farbeAnzeigen(aktuelleFarbe);
    letzteAenderung = millis();
  }
}

void ausschalten() {
  for (int i = 0; i < LED_ANZAHL; i++) {
    led.setPixelColor(i, 0);
  }
  led.show();
  Serial.println("LEDs AUS");
}

void farbeAnzeigen(int index) {
  for (int i = 0; i < LED_ANZAHL; i++) {
    led.setPixelColor(i, led.Color(farben[index][0], farben[index][1], farben[index][2]));
  }
  led.show();
  Serial.print("Farbe: ");
  Serial.println(farben[index][3]);
}