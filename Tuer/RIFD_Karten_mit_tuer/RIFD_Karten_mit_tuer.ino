#include <SPI.h>
#include <MFRC522.h> // Bibliothek für den RC522 RFID-Leser
#include <Servo.h>   

// Definition der Pins für den RC522
#define RST_PIN  9  
#define SS_PIN  10  

// Erstelle das MFRC522 Objekt
MFRC522 mfrc522(SS_PIN, RST_PIN);  
Servo tuerServo;                // Servo-Objekt


const int servoPin = 3;    
const int oeffnungsWinkel = 0;   
const int schliessWinkel = 90;   
const long warteZeit = 3000;    


// Kartencodes
const String blaueChipCode = "112155927";
const String studentenkarteCode = "4307817024027144";
const String weisserkarteCode = "1761411334";


void setup() {
  Serial.begin(115200);//  Serielle Kommunikation mit 115200 Baud starten 
  SPI.begin();        // SPI-Kommunikation starten
  mfrc522.PCD_Init();  // RFID-Modul initialisieren
  tuerServo.attach(servoPin);
  tuerServo.write(schliessWinkel);  // Tür zu beim Start
  Serial.println("RFID RC522 bereit."); 
}

void loop() {
  // Prüfen ob eine Karte vorhanden ist
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  String uid = getUID();

  Serial.print("Karte erkannt: ");
  Serial.println(uid);

    // Zugriff prüfen
  if (uid == studentenkarteCode) {
    Serial.println("Zugriff erlaubt: Student erkannt – Tür öffnet sich.");
    tuerOeffnen();
  } else if (uid == blaueChipCode) {
    Serial.println("Zugriff erlaubt: Professor erkannt – Tür öffnet sich");
    tuerOeffnen();
  } else if (uid == weisserkarteCode ) {
    Serial.println("Zugriff verweigert: Weiße Karte erkannt – Kein Zugang.");
  } else {
    Serial.println("Unbekannte Karte – Kein Zugang.");
  }
  // Kommunikation mit der Karte beenden
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}   

   // UID als String speichern
String getUID() {
  String uidStr = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidStr += String(mfrc522.uid.uidByte[i], DEC);
  }
   return uidStr;
}

// Funktion zum Öffnen & Schließen der Tür
void tuerOeffnen() {
  tuerServo.write(oeffnungsWinkel);
  delay(warteZeit);
  tuerServo.write(schliessWinkel);
}

