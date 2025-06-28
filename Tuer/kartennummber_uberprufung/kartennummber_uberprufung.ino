#include <SPI.h>
#include <MFRC522.h> // Bibliothek für den RC522 RFID-Leser

// Definition der Pins für den RC522 
#define RST_PIN  9  
#define SS_PIN  10  

// MFRC522 Objekt
MFRC522 mfrc522(SS_PIN, RST_PIN);  


// Kartencodes
const String blaueChipCode = "112155927";
const String studentenkarteCode = "4307817024027144";
const String weisserkarteCode = "1761411334";

void setup() {
  Serial.begin(115200);  //  Serielle Kommunikation mit 115200 Baud starten 
  SPI.begin();        // SPI-Kommunikation starten
  mfrc522.PCD_Init();  // RFID-Modul initialisieren
  Serial.println("RFID RC522 bereit."); 
}

void loop() {

 // Nur reagieren, wenn eine Karte da ist und gelesen werden kann
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }
// UID (Kartennummer) auslesen und als String speichern
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i], DEC);
  }

  // UID ausgeben
  Serial.print("Karte erkannt – UID: ");
  Serial.println(uid);

  // Zugriff prüfen
  if (uid == studentenkarteCode) {
    Serial.println("Zugriff erlaubt: Student");
  } else if (uid == blaueChipCode) {
    Serial.println("Zugriff erlaubt: Prof");
  } else if (uid == weisserkarteCode ) {
    Serial.println("Zugriff verweigert: Weiße Karte");
  } else {
    Serial.println("Unbekannte Karte – Zugriff verweigert.");
  }
  // Kommunikation mit der Karte beenden
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
   
  
}