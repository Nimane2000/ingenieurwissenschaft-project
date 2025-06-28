#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define DHTPIN 4     
#define DHTTYPE DHT11   

// DHT Objekt
DHT dht(DHTPIN, DHTTYPE);

// LCD Adresse (normalerweise 0x27 oder 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16 Zeichen pro Zeile, 2 Zeilen

void setup() {
  Serial.begin(115200);
  
  // Initialisiere das LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Starte..."); 
  dht.begin();
  delay(1000);
}

void loop() {
  ShowHumiture();   // Werte auslesen und anzeigen
  delay(2000); // Warte 2 Sekunden vor der nächsten Messung
}


void ShowHumiture() {
    // Das Auslesen von Temperatur oder Luftfeuchtigkeit dauert etwa 250 Millisekunden
    float h = dht.readHumidity();   
    float t = dht.readTemperature(); 

    // Gib die Feuchtigkeit auf dem seriellen Monitor aus
    Serial.print("Feuchtigkeit: "); 
    Serial.print(h, 1);
    Serial.println(" %");  

    // Gib die Temperatur auf dem seriellen Monitor aus
    Serial.print("Temperatur: "); 
    Serial.print(t, 1);
    Serial.println(" °C");  

    // Zeige die Feuchtigkeit auf dem LCD an
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Feucht: "); 
    lcd.print(h, 1);
    lcd.print(" %"); 

    // Zeige die Temperatur auf dem LCD an
    lcd.setCursor(0, 1);
    lcd.print("Temp: "); 
    lcd.print(t, 1);
    lcd.print(" *C");  
}