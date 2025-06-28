#include <Adafruit_NeoPixel.h>

#define LED_PIN 5          
#define LED_ANZAHL 4       

Adafruit_NeoPixel led(LED_ANZAHL, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  led.begin();
  led.show();
  Serial.begin(115200);
  Serial.println("WS2812B Mehrfarben-Wechsler gestartet!");
}

void loop() {
  zeigeFarbe(255, 0, 0, "Rot");       // https://www.rapidtables.com/web/color/RGB_Color.html
  zeigeFarbe(255, 165, 0, "Orange");
  zeigeFarbe(255, 255, 0, "Gelb");
  zeigeFarbe(0, 255, 0, "Grün");
  zeigeFarbe(0, 255, 255, "Cyan");
  zeigeFarbe(0, 0, 255, "Blau");
  zeigeFarbe(128, 0, 128, "Lila");
  zeigeFarbe(255, 105, 180, "Rosa");
  zeigeFarbe(255, 255, 255, "Weiß");
}

void zeigeFarbe(int rot, int gruen, int blau, String farbName) {
  for (int i = 0; i < LED_ANZAHL; i++) {
    led.setPixelColor(i, led.Color(rot, gruen, blau));  // Setzt Farbe für jede LED
  }
  led.show();     // Überträgt die Farbe an den LED-Streifen
  Serial.println("Farbe: " + farbName);
  delay(5000);
}

