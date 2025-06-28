#include <SPI.h>
#include <MFRC522.h> // Bibliothek für den RC522 RFID-Leser

// Definition der Pins für den RC522 
#define RST_PIN  9  
#define SS_PIN  10  

//  MFRC522 Objekt
MFRC522 mfrc522(SS_PIN, RST_PIN);  

void setup() {
  Serial.begin(115200);  //  Serielle Kommunikation mit 115200 Baud starten 
  SPI.begin();        // SPI-Kommunikation starten
  mfrc522.PCD_Init();  // RFID-Modul initialisieren
  Serial.println("RFID RC522 bereit."); 
}

void loop() {
   // Prüfe, ob eine neue Karte vorhanden ist und ob deren UID gelesen werden kann
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;  // Nur reagieren, wenn eine Karte da ist und gelesen werden kann
  }
 
  // Ausgabe der Kartennummer (UID) auf dem seriellen Monitor
  Serial.print("Kartennummer: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i], DEC); // Verwende DEC für die einfache Kartennummer
  }
  Serial.println();

  // Kommunikation mit der Karte beenden
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

}