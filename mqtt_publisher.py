import paho.mqtt.client as mqtt
import serial
import json
import time

# MQTT-Einstellungen
MQTT_BROKER_IP = "192.168.87.169"
MQTT_PORT = 1883
MQTT_TOPIC = "arduino/sensors"       # Darunter die Daten veröffentlicht
MQTT_USERNAME = "admin"
MQTT_PASSWORD = "admin"

# Serielle Schnittstellen-Einstellungen (Arduino)
SERIAL_PORT = "/dev/ttyUSB0"
BAUDRATE = 115200

# Serielle Verbindung öffnen
ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)

# MQTT-Client konfigurieren
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, "arduino_publisher")
client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
client.connect(MQTT_BROKER_IP, MQTT_PORT, 60)      # Stelle Verbindung zum MQTT-Broker her
client.loop_start()

# Funktion zum Einlesen eines vollständigen Datenblocks vom Arduino
def read_sensor_block():
    """Liest einen kompletten Satz Sensordaten vom Arduino ein und gibt ein Dictionary zurück"""
    data = {}
    expected_keys = [
        "gaswert", "taste", "regenwert", "touch",
        "temperatur", "feuchtigkeit", "rfid"
    ]
    while True:
        line = ser.readline().decode('utf-8').strip()  # Zeile einlesen, dekodieren und bereinigen
        if not line:
            continue
        if ',' in line:
            key, value = [x.strip() for x in line.split(',', 1)]  # Trennen in Schlüssel und Wert

             # Typumwandlung je nach Schlüsseltyp
            if key in ["gaswert", "regenwert", "temperatur", "feuchtigkeit"]:
                try:
                    data[key] = float(value)  # numerisch speichern
                except ValueError:
                    data[key] = None  # Falls Wert ungültig ist
            elif key in ["taste", "touch", "rfid"]:
                try:
                    data[key] = int(value)  # 0 oder 1 speichern
                except ValueError:
                    data[key] = 0  
            else:
                # Alles andere bleibt als String
                data[key] = value
        # Prüfen, ob alle erwarteten Daten da sind
        if all(k in data for k in expected_keys):
            return data
        
# Hauptprogramm: Daten einlesen und an MQTT senden
try:
    while True:
        sensor_data = read_sensor_block()    # Sensordaten vom Arduino lesen
        payload = json.dumps(sensor_data)    # In JSON-String umwandeln
        client.publish(MQTT_TOPIC, payload)     # An MQTT-Topic senden
        print(f"Published: {payload}")
        # time.sleep(2)


# Wenn Nutzer das Programm mit STRG+C abbricht
except KeyboardInterrupt:
    print("Vom Benutzer beendet.")

# Aufräumen am Ende
finally:
    client.loop_stop()
    client.disconnect()
    ser.close()