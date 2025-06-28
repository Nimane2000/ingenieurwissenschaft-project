
const int rainSensorPin = A3;  // Pin des Regensensors

void setup() {
  Serial.begin(115200);
  Serial.println("Regensensor gestartet...");
}

void loop() {
  int rainValue = analogRead(rainSensorPin);

  if (rainValue < 500) {  // Schwellenwert: je niedriger, desto mehr Wasser erkannt
    Serial.println("Regen erkannt!");
  } else {
    Serial.println("Kein Regen");
  }

  delay(1000);  // 1 Sekunde warten
}
