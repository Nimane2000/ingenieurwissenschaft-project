// Pin-Definitionen
#define LUEFTER_PIN 2     
#define RELAIS_PIN 16     
#define BUZZER_PIN 15     
#define LED_PIN 8         
#define TASTER_PIN A6     
#define GAS_SENSOR A0     

// Schwellenwerte
const int GAS_ALARM_LEVEL = 400;  
const int LED_INTERVALL = 5000;   

// Systemvariablen
bool alarmAktiv = false;
bool tasterGedrueckt = false;
unsigned long letzteLedAenderung = 0;
bool alarmManuellDeaktiviert = false; 
bool ledZustand = false;

void setup() {
  // Initialisierung der Pins
  pinMode(LUEFTER_PIN, OUTPUT);
  pinMode(RELAIS_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TASTER_PIN, INPUT_PULLUP);
  
  Serial.begin(115200);
  Serial.println("Sicherheitssystem initialisiert");
}

void loop() {
  // Gassensor 
  int gasWert = analogRead(GAS_SENSOR);
  
  // Tasterstatus prüfen
  bool tasterStatus = analogRead(TASTER_PIN) < 50; 
  
  // Alarmlogik
  if (tasterStatus && !tasterGedrueckt) {
    tasterGedrueckt = true;
    alarmAktiv = false;
    alarmManuellDeaktiviert = true; 
    Serial.println("Alarm manuell deaktiviert");
    allesAusschalten();
    delay(200); 
  }
  else if (!tasterStatus) {
    tasterGedrueckt = false;
    // Prüfen, ob der Gaswert sicher ist, um den Alarmstatus zurückzusetzen
    if (gasWert < GAS_ALARM_LEVEL) {
      alarmManuellDeaktiviert = false; // Alarm kann wieder aktiviert 
    }

    if (gasWert >= GAS_ALARM_LEVEL && !alarmAktiv && !alarmManuellDeaktiviert) {
      alarmAktiv = true;
      Serial.println("Gas-Alarm aktiviert!");
    }
  }
  // Alarmsteuerung
  if (alarmAktiv) {
    // Lüfter einschalten
    digitalWrite(RELAIS_PIN, HIGH);
    digitalWrite(LUEFTER_PIN, HIGH);
    
    // Summer einschalten
    digitalWrite(BUZZER_PIN, HIGH);
    
    // LED blinken lassen
    if (millis() - letzteLedAenderung >= LED_INTERVALL) {
      ledZustand = !ledZustand;
      digitalWrite(LED_PIN, ledZustand);
      letzteLedAenderung = millis();
      Serial.println(ledZustand ? "LED eingeschaltet" : "LED ausgeschaltet");
    }
  }
  else {
    allesAusschalten();
  }

  // Statusausgabe
  static unsigned long letzteAusgabe = 0;
  if (millis() - letzteAusgabe >= 1000) {
    Serial.print("Gaswert: ");
    Serial.print(gasWert);
    Serial.println(alarmAktiv ? " - ALARM!" : " - Normal");
    letzteAusgabe = millis();
  }
}

void allesAusschalten() {
  digitalWrite(RELAIS_PIN, LOW);
  digitalWrite(LUEFTER_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}