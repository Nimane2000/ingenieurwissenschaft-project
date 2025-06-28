#define LUEFTER_PIN  2    
#define RELAIS_PIN   16   

// Zeitsteuerung
const int BETRIEBSDAUER =8000; 
unsigned long startZeit;         
bool luefterAktiv = false;       

void setup() {
  Serial.begin(115200);
  
  // Pin-Modi konfigurieren
  pinMode(LUEFTER_PIN, OUTPUT);
  pinMode(RELAIS_PIN, OUTPUT);
  
  // Lüfter und Relais einschalten
  digitalWrite(RELAIS_PIN, HIGH);  
  digitalWrite(LUEFTER_PIN, HIGH); 
  luefterAktiv = true;
  
  
  startZeit = millis();
  
  Serial.println("Lüfter und Relais wurden eingeschaltet");
}

void loop() {
  
  unsigned long aktuelleLaufzeit = millis() - startZeit;
  
  if (aktuelleLaufzeit >= BETRIEBSDAUER && luefterAktiv) {
    ausschalten();
  }
  
  if (luefterAktiv) {
    anzeigeRestzeit(aktuelleLaufzeit);
  }
  
  delay(1000); 
}

// Funktion zum Ausschalten von Lüfter und Relais
void ausschalten() {
  digitalWrite(LUEFTER_PIN, LOW);
  digitalWrite(RELAIS_PIN, LOW);
  luefterAktiv = false;
  Serial.println("System wurde nach 5 Sekunden ausgeschaltet");
}

// Funktion zur Anzeige der verbleibenden Zeit
void anzeigeRestzeit(unsigned long laufzeit) {
  int restzeit = (BETRIEBSDAUER - laufzeit) / 1000;
  Serial.print("Verbleibende Laufzeit: ");
  Serial.print(restzeit);
  Serial.println(" Sekunden");
}