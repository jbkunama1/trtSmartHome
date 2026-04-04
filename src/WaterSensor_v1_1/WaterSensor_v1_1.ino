// ============================================================
// trtSmartHome – Wassermelder (Smarthome Modell)
// Version: 1.1
// Beschreibung: Erkennt Wasser und löst Alarm aus.
//               Verbesserungen gegenüber v1.5:
//               - Nicht-blockierendes Timing mit millis()
//               - Entprellung (Debouncing) für stabile Erkennung
// ============================================================

// --- Pin-Belegung ---
const int wasserSensorPin = 2;  // Digitales Signal vom Sensor
const int summerPin       = 8;  // Summer (Buzzer)
const int ledPin          = 7;  // Rote LED

// --- Debouncing-Konfiguration ---
const int  DEBOUNCE_COUNT = 3;    // Anzahl gleicher Messungen zur Bestätigung
const long INTERVAL_MS    = 500;  // Messintervall in Millisekunden

// --- Zustandsvariablen ---
int  letzterStatus    = HIGH; // Letzter bestätigter Sensorstatus
int  roherStatus      = HIGH; // Zuletzt gelesener Rohwert
int  debounceZaehler  = 0;    // Zähler für aufeinanderfolgende gleiche Messungen
unsigned long letzteZeit = 0; // Zeitstempel der letzten Messung (millis)

void setup() {
  pinMode(wasserSensorPin, INPUT);
  pinMode(summerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("trtSmartHome – Wassermelder v1.1 bereit.");
}

void loop() {
  unsigned long jetzt = millis();

  // Messung nur im definierten Intervall (nicht-blockierend, kein delay())
  if (jetzt - letzteZeit >= INTERVAL_MS) {
    letzteZeit = jetzt;

    int aktuellerRohwert = digitalRead(wasserSensorPin);

    // Debouncing: gleichen Wert DEBOUNCE_COUNT-mal hintereinander bestätigen
    if (aktuellerRohwert == roherStatus) {
      debounceZaehler++;
    } else {
      roherStatus      = aktuellerRohwert;
      debounceZaehler  = 1;
    }

    // Zustand nur wechseln wenn stabil und neu
    if (debounceZaehler >= DEBOUNCE_COUNT && aktuellerRohwert != letzterStatus) {
      letzterStatus   = aktuellerRohwert;
      debounceZaehler = 0;
      alarmAktualisieren(letzterStatus);
    }
  }
}

// --- Alarm ein- oder ausschalten ---
void alarmAktualisieren(int status) {
  if (status == LOW) {
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
}
