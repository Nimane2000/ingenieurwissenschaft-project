#include <Servo.h>              
#include <Adafruit_NeoPixel.h>  // Bibliothek für LED steuerung durch TOUCH-Sensor
#include <DHT.h>                
#include <Wire.h>               // Bibliothek für I2C-Kommunikation
#include <LiquidCrystal_I2C.h>  // Bibliothek für LCD-Display
#include <SPI.h>                
#include <MFRC522.h>            // Bibliothek für RC522 RFID-Leser

// Pin-Definitionen
// Gas Alarmsystem Steuerung durch Lüfter_Buzzer_Taster_LED
#define LUEFTER_PIN 2     
#define RELAIS_PIN 16     
#define BUZZER_PIN 15     
#define LED_PIN 8         
#define TASTER_PIN A6     
#define GAS_SENSOR A0     

// LED steuerung durch TOUCH-Sensor
#define TOUCH_PIN 7       
#define LED_NEO_PIN 5     
#define LED_ANZAHL 4      

// Fenstersteuerungssystem durch Regensensor
#define rainSensorPin A3  
#define servoPinFenster 6 

// Temperature und Feuchtigkeitsystem steuerung durch LCD
#define DHTPIN 4          
#define DHTTYPE DHT11     

// Türsteuerungssystem durch RFID und Servo 
#define RST_PIN 9         
#define SS_PIN 10         
#define servoPinTuer 3    

// Schwellenwerte und Konstanten 
// Gas Alarmsystem Steuerung durch Lüfter_Buzzer_Taster_LED
const int GAS_ALARM_LEVEL = 400;  // Gas-Schwellwert für Alarm
const int LED_INTERVALL = 5000;   // LED Blinkintervall in ms

// LED steuerung durch TOUCH-Sensor
const int farbIntervall = 1000;   // Farbwechsel alle 1000ms
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

// Fenstersteuerungssystem durch Regensensor
const int offenWinkel = 0;         
const int geschlossenWinkel = 180; 
const int regenSchwelle = 500;     
const long warteZeitFenster = 100;

// Türsteuerungssystem durch RFID und Servo 
const int oeffnungsWinkel = 0;   
const int schliessWinkel = 90;     
const long warteZeitTuer = 3000;   
const String blaueChipCode = "112155927";
const String studentenkarteCode = "4307817024027144";
const String weisserkarteCode = "1761411334";

// Objekte 
// Gas Alarmsystem Steuerung durch Lüfter_Buzzer_Taster_LED
bool alarmAktiv = false;
bool tasterGedrueckt = false;
bool alarmManuellDeaktiviert = false; 
unsigned long letzteLedAenderung = 0;
bool ledZustand = false;

// LED steuerung durch TOUCH-Sensor
Adafruit_NeoPixel led(LED_ANZAHL, LED_NEO_PIN, NEO_GRB + NEO_KHZ800);
bool ledsAn = false;
unsigned long letzteAenderung = 0;
int aktuelleFarbe = 0;

// Fenstersteuerungssystem durch Regensensor
Servo meinServo;  

// Temperature und Feuchtigkeitsystem steuerung durch LCD
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD mit 16 Zeichen, 2 Zeilen

// Türsteuerungssystem durch RFID und Servo 
MFRC522 mfrc522(SS_PIN, RST_PIN);  // RFID-Modul
Servo tuerServo;                   

void setup() {
  // Serielle Kommunikation starten
  Serial.begin(115200);

  // Gas Alarmsystem Steuerung durch Lüfter_Buzzer_Taster_LED 
  pinMode(LUEFTER_PIN, OUTPUT);
  pinMode(RELAIS_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TASTER_PIN, INPUT_PULLUP);
  Serial.println("Sicherheitssystem initialisiert");

  // LED steuerung durch TOUCH-Sensor 
  pinMode(TOUCH_PIN, INPUT);
  led.begin();
  led.show();
  Serial.println("Bereit. Berühren Sie den Sensor.");

  // Fenstersteuerungssystem durch Regensensor
  meinServo.attach(servoPinFenster);
  meinServo.write(offenWinkel);  // Fenster auf beim Start
  Serial.println("Regensensor & Fenstersteuerung gestartet...");

  // Temperature und Feuchtigkeitsystem steuerung durch LCD  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Starte...");
  dht.begin();
  delay(100);

  // Türsteuerungssystem durch RFID und Servo 
  SPI.begin();
  mfrc522.PCD_Init();
  tuerServo.attach(servoPinTuer);
  tuerServo.write(schliessWinkel);  // Tür zu beim Start
  Serial.println("RFID RC522 bereit.");
}

void loop() {
  GasSystem();
  LedTouchSystem();
  RainSensorSystem();
  DHTSystem();
  RFIDSystem();
}

// Systemfunktionen 
// Gas Alarmsystem Steuerung durch Lüfter_Buzzer_Taster_LED
void GasSystem() {
  int gasWert = analogRead(GAS_SENSOR);
  bool tasterStatus = analogRead(TASTER_PIN) < 50; // Analoger Schwellwert
  if (tasterStatus && !tasterGedrueckt) {
    tasterGedrueckt = true;
    alarmAktiv = false;
    alarmManuellDeaktiviert = true;
    Serial.println("Alarm manuell deaktiviert");
    allesAusschalten();
    delay(200); 
  } else if (!tasterStatus) {
    tasterGedrueckt = false;
    if (gasWert < GAS_ALARM_LEVEL) {
      alarmManuellDeaktiviert = false;
    }
    if (gasWert >= GAS_ALARM_LEVEL && !alarmAktiv && !alarmManuellDeaktiviert) {
      alarmAktiv = true;
      Serial.println("Gas-Alarm aktiviert!");
    }
  }
  if (alarmAktiv) {
    digitalWrite(RELAIS_PIN, HIGH);
    digitalWrite(LUEFTER_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    if (millis() - letzteLedAenderung >= LED_INTERVALL) {
      ledZustand = !ledZustand;
      digitalWrite(LED_PIN, ledZustand);
      letzteLedAenderung = millis();
      Serial.println(ledZustand ? "LED eingeschaltet" : "LED ausgeschaltet");
    }
  } else {
    allesAusschalten();
  }
  static unsigned long letzteAusgabe = 0;
  if (millis() - letzteAusgabe >= 1000) {
    Serial.print("Gaswert: ");
    Serial.print(gasWert);
    Serial.println(alarmAktiv ? " - ALARM!" : " - Normal");
    letzteAusgabe = millis();
  }
}

// LED steuerung durch TOUCH-Sensor
void LedTouchSystem() {
  int touchWert = analogRead(TOUCH_PIN);
  bool beruehrung = (touchWert > 50);
  if (beruehrung) {
    ledsAn = !ledsAn;
    if (ledsAn) {
      aktuelleFarbe = 0;
      farbeAnzeigen(aktuelleFarbe);
      Serial.println("LEDs AN");
    } else {
      ausschalten();
    }
    delay(300); 
    while (analogRead(TOUCH_PIN) > 50) {} 
  }
  if (ledsAn && millis() - letzteAenderung >= farbIntervall) {
    aktuelleFarbe = (aktuelleFarbe + 1) % 9;
    farbeAnzeigen(aktuelleFarbe);
    letzteAenderung = millis();
  }
}

// Fenstersteuerungssystem durch Regensensor
void RainSensorSystem() {
  int regenWert = analogRead(rainSensorPin);
  if (regenWert < regenSchwelle) {
    Serial.println("Regen erkannt! Fenster wird geschlossen.");
    Serial.println(regenWert);
    meinServo.write(geschlossenWinkel);
  } else {
    Serial.println("Kein Regen! Fenster wird geöffnet.");
    Serial.println(regenWert);
    meinServo.write(offenWinkel);
  }
  delay(warteZeitFenster);
}

// Temperature und Feuchtigkeitsystem steuerung durch LCD
void DHTSystem() {
  ShowHumiture();
  delay(100);
}

// Türsteuerungssystem durch RFID und Servo
void RFIDSystem() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uid = getUID();
    Serial.print("Karte erkannt: ");
    Serial.println(uid);
    if (uid == studentenkarteCode) {
      Serial.println("Zugriff erlaubt: Student erkannt – Tür öffnet sich.");
      tuerOeffnen();
    } else if (uid == blaueChipCode) {
      Serial.println("Zugriff erlaubt: Professor erkannt – Tür öffnet sich");
      tuerOeffnen();
    } else if (uid == weisserkarteCode) {
      Serial.println("Zugriff verweigert: Weiße Karte erkannt – Kein Zugang.");
    } else {
      Serial.println("Unbekannte Karte – Kein Zugang.");
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

// Hilfsfunktionen
// Gas Alarmsystem Steuerung durch Lüfter_Buzzer_Taster_LED
void allesAusschalten() {
  digitalWrite(RELAIS_PIN, LOW);
  digitalWrite(LUEFTER_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

// LED steuerung durch TOUCH-Sensor
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

// Temperature und Feuchtigkeitsystem steuerung durch LCD
void ShowHumiture() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Feuchtigkeit: ");
  Serial.print(h, 1);
  Serial.println(" %");
  Serial.print("Temperatur: ");
  Serial.print(t, 1);
  Serial.println(" °C");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Feucht: ");
  lcd.print(h, 1);
  lcd.print(" %");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(t, 1);
  lcd.print(" *C");
}

// Türsteuerungssystem durch RFID und Servo
String getUID() {
  String uidStr = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidStr += String(mfrc522.uid.uidByte[i], DEC);
  }
  return uidStr;
}

void tuerOeffnen() {
  tuerServo.write(oeffnungsWinkel);
  delay(warteZeitTuer);
  tuerServo.write(schliessWinkel);
}