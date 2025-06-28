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
const long warteZeitFenster = 1000;

// Türsteuerungssystem durch RFID und Servo 
const int oeffnungsWinkel = 0;     
const int schliessWinkel = 90;     
const long warteZeitTuer = 3000;   
const String blaueChipCode = "112155927";
const String studentenkarteCode = "4307817024027144";
const String weisserkarteCode = "1761411334";
int rfidZugang = 0;  // Standardwert: kein Zugang

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

// Serial Datenübertragung 
unsigned long letzteDatenSendung = 0;
const long datenIntervall = 500;  

void setup() {
  // Serielle Kommunikation starten
  Serial.begin(115200);

  // Gas Alarmsystem Steuerung durch Lüfter_Buzzer_Taster_LED 
  pinMode(LUEFTER_PIN, OUTPUT);
  pinMode(RELAIS_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TASTER_PIN, INPUT_PULLUP);

  // LED steuerung durch TOUCH-Sensor 
  pinMode(TOUCH_PIN, INPUT);
  led.begin();
  led.show();

  // Fenstersteuerungssystem durch Regensensor 
  meinServo.attach(servoPinFenster);
  meinServo.write(offenWinkel);  // Fenster auf beim Start

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
}

void loop() {
  GasSystem();
  LedTouchSystem();
  RainSensorSystem();
  DHTSystem();
  RFIDSystem();
  sendSensorData();
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
    allesAusschalten();
    delay(200); 
  } else if (!tasterStatus) {
    tasterGedrueckt = false;
    if (gasWert < GAS_ALARM_LEVEL) {
      alarmManuellDeaktiviert = false;
    }
    if (gasWert >= GAS_ALARM_LEVEL && !alarmAktiv && !alarmManuellDeaktiviert) {
      alarmAktiv = true;
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
    }
  } else {
    allesAusschalten();
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
    meinServo.write(geschlossenWinkel);
  } else {
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
    if (uid == studentenkarteCode || uid == blaueChipCode) {
      tuerOeffnen();
      rfidZugang = 1;  //  Zugang erlaubt
    } else {
      rfidZugang = 0;  //  Zugang verweigert
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

// Neue Funktion für Datenübertragung 
void sendSensorData() {
  if (millis() - letzteDatenSendung < datenIntervall) return;

  // Sensorwerte lesen
  int gasWert = analogRead(GAS_SENSOR);
  int regenWert = analogRead(rainSensorPin);
  int touchWert = analogRead(TOUCH_PIN);
  bool beruehrung = (touchWert > 50);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  bool tasterStatus = analogRead(TASTER_PIN) < 50;
  String rfidUID = mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() ? getUID() : "none";

  // Daten senden
  Serial.print("gaswert, ");
  Serial.println(gasWert);
  Serial.print("taste, ");
  Serial.println(tasterStatus ? "1" : "0");
  Serial.print("regenwert, ");
  Serial.println(regenWert);
  Serial.print("touch, ");
  Serial.println(beruehrung ? "1" : "0");
  Serial.print("temperatur, ");
  Serial.println(t, 1);
  Serial.print("feuchtigkeit, ");
  Serial.println(h, 1);
  Serial.print("rfid, ");
  Serial.println(rfidZugang);

  letzteDatenSendung = millis();
  rfidZugang = 0;  // Status zurücksetzen
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
}

void farbeAnzeigen(int index) {
  for (int i = 0; i < LED_ANZAHL; i++) {
    led.setPixelColor(i, led.Color(farben[index][0], farben[index][1], farben[index][2]));
  }
  led.show();
}

// Temperature und Feuchtigkeitsystem steuerung durch LCD
void ShowHumiture() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
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