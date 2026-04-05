// ============================================================
// trtSmartHome – Wassermelder (Smarthome Modell)
// Version: 1.5
// Beschreibung: Erkennt Wasser und löst Alarm aus.
// ============================================================

// --- Pin-Belegung ---
const int wasserSensorPin = 2;  // Digitales Signal vom Sensor
const int summerPin       = 8;  // Summer (Buzzer)
const int ledPin          = 7;  // Rote LED

void setup() {
  pinMode(wasserSensorPin, INPUT);
  pinMode(summerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("trtSmartHome – Wassermelder bereit.");
}

void loop() {
  int wasserStatus = digitalRead(wasserSensorPin);

  if (wasserStatus == LOW) {
    // Wasser erkannt
    digitalWrite(summerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    Serial.println("⚠ Wasser erkannt!");
  } else {
    // Kein Wasser
    digitalWrite(summerPin, LOW);
    digitalWrite(ledPin, LOW);
    Serial.println("✔ Kein Wasser erkannt.");
  }

  delay(500);
}
