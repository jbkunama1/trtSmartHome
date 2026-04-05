/*
 * Datei:        Water_Sensor_V1_5.ino
 * Version:      1.5
 * Datum:        2026-03-09
 * Autor:        trtSmartHome
 * Beschreibung: Arduino Sketch zur Wasserstandserkennung.
 *               Bei erkanntem Wasser leuchtet eine LED und ein Buzzer ertönt.
 *               Der Status wird über den seriellen Monitor ausgegeben.
 *
 * Pinbelegung:
 *   D2 – Wassersensor Signalausgang (OUT)
 *   D7 – LED (Status-Anzeige)
 *   D8 – Buzzer (Alarm)
 */

// ---- Pin-Definitionen ----
const int waterSensorPin = 2;  // Digitaler Eingang: Wassersensor Signal
const int ledPin         = 7;  // Digitaler Ausgang: LED
const int buzzerPin      = 8;  // Digitaler Ausgang: Buzzer

// ---- Setup ----
void setup() {
  // Serielle Kommunikation starten (Baudrate 9600)
  Serial.begin(9600);

  // Pins konfigurieren
  pinMode(waterSensorPin, INPUT);   // Wassersensor als Eingang
  pinMode(ledPin,         OUTPUT);  // LED als Ausgang
  pinMode(buzzerPin,      OUTPUT);  // Buzzer als Ausgang

  Serial.println("trtSmartHome – Wasserstandssensor V1.5 gestartet.");
}

// ---- Hauptschleife ----
void loop() {
  // Sensorwert auslesen
  int sensorWert = digitalRead(waterSensorPin);

  /*
   * Hinweis: Viele Wassersensoren melden LOW, wenn Wasser erkannt wird.
   * Sollte das Verhalten umgekehrt sein, ersetze LOW durch HIGH (und umgekehrt).
   */
  if (sensorWert == LOW) {
    // Wasser erkannt: LED und Buzzer einschalten
    digitalWrite(ledPin,    HIGH);
    digitalWrite(buzzerPin, HIGH);
    Serial.println("Wasser erkannt!");
  } else {
    // Kein Wasser: LED und Buzzer ausschalten
    digitalWrite(ledPin,    LOW);
    digitalWrite(buzzerPin, LOW);
    Serial.println("Kein Wasser erkannt.");
  }

  // Wartezeit vor dem nächsten Messdurchgang
  delay(500);
}
