const int sensorPin = A0;  

void setup() {
  Serial.begin(115200);      
  Serial.println("MQ-2 Gas Sensor Wertemonitor");
  Serial.println("----------------------------");
  Serial.println("Sensorwert (0-1023)");
  Serial.println("----------------------------");
}

void loop() {
  int sensorWert = analogRead(sensorPin);  
  
  // Rohwert des Sensors 
  Serial.println(sensorWert);
  
  
  if (sensorWert < 200) {
    Serial.println("(Saubere Luft)");
  } 
  else if (sensorWert < 400) {
    Serial.println("(Geringe Gasbelastung)");
  }
  else {
    Serial.println("(Starke Gas-/Rauchbelastung)");
  }
  delay(1000);  // Wartezeit zwischen den Messungen (1s)
}