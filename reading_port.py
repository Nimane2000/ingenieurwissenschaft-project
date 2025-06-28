import serial
import csv
from datetime import datetime


# Serielle Verbindung einrichten: '/dev/ttyUSB0' Der Port, an dem das Arduino angeschlossen ist , Baudrate 115200, mit 1 Sekunde Wartezeit 
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
# Öffnen der CSV-Datei 'sensor_data.csv'  (keine zusätzlichen Leerzeilen eingefügt mit newline)
with open('sensor_data.csv', 'a', newline='') as f:   
    # Erstellen eines CSV-Writer-Objekts, um Daten in die Datei zu schreiben
    writer = csv.writer(f)
    writer.writerow(["Timestamp", "Sensor1", "Sensor2"])  # Spaltenüberschriften 

     # Endlosschleife zum kontinuierlichen Einlesen der Daten
    while True:
        if ser.in_waiting > 0:       # Prüfen, ob Daten im seriellen Schnittstelle
            # Lese eine Zeile von der seriellen Schnittstelle und dekodieren von Byte zu String und entferne Zeilenumbruch
            line = ser.readline().decode('utf-8').rstrip() 
            # Erzeuge einen Zeitstempel mit Datum und Uhrzeit
            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            sensor_values = line.split(',')  # Trenne die eingelesenen Werte
            print("sensor_values", sensor_values)
             # Zeile: Kombiniert Zeitstempel und Sensorwerte in eine Liste
            writer.writerow([timestamp] + sensor_values)



