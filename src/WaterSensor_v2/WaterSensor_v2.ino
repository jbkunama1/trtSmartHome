// ============================================================
// trtSmartHome – Wassermelder (Smarthome Modell)
// Version: 2.0
// Plattform: Arduino Uno / Nano / Mega
// Beschreibung: Analoge Alarmstufen (TROCKEN / FEUCHT / NASS),
//               LCD-Statusanzeige (I2C, 16×2),
//               Stummschalttaste für den Buzzer,
//               nicht-blockierendes Timing + Debouncing.
//
// Benötigte Bibliothek:
//   LiquidCrystal_I2C von Frank de Brabander
//   → Arduino IDE: Bibliotheksverwalter → "LiquidCrystal I2C"
// ============================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Pin-Belegung ---
const int wasserSensorAnalog  = A0; // Analoges Signal vom Sensor (AO-Pin des YL-83)
const int wasserSensorDigital = 2;  // Digitales Signal (DO-Pin, Backup / Referenz)
const int summerPin           = 8;  // Summer (Buzzer, passiv → tone() wird genutzt)
const int ledPin              = 7;  // Rote LED
const int stummTastePin       = 3;  // Stummschalttaste (gegen GND, intern Pull-up)

// --- LCD (I2C-Adresse 0x27, 16 Zeichen, 2 Zeilen) ---
// Adresse ggf. auf 0x3F ändern, falls LCD nicht reagiert.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Schwellenwerte für Alarmstufen (Analogwert 0–1023) ---
const int SCHWELLE_FEUCHT = 100;  // ab hier: feucht
const int SCHWELLE_NASS   = 500;  // ab hier: nass

// --- Timing ---
const long INTERVAL_MS = 500;     // Messintervall in Millisekunden
unsigned long letzteZeit = 0;

// --- Zustandsvariablen ---
bool stummgeschaltet = false;
bool letzteTasteGedrueckt = false;
int  letzteStufe = -1;            // -1 = noch nicht initialisiert

// Alarmstufen als Aufzählung
enum Alarmstufe { TROCKEN, FEUCHT, NASS };

// ============================================================
void setup() {
  pinMode(wasserSensorDigital, INPUT);
  pinMode(summerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(stummTastePin, INPUT_PULLUP); // Taste zieht nach GND → LOW = gedrückt

  Serial.begin(9600);

  // LCD initialisieren
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("trtSmartHome");
  lcd.setCursor(0, 1);
  lcd.print("Wassermelder v2");
  delay(2000);
  lcd.clear();

  Serial.println("trtSmartHome – Wassermelder v2.0 bereit.");
  Serial.println("Schwellen: FEUCHT >= " + String(SCHWELLE_FEUCHT) +
                 " | NASS >= " + String(SCHWELLE_NASS));
}

// ============================================================
void loop() {
  unsigned long jetzt = millis();

  // --- Stummschalttaste entprellt abfragen ---
  bool tasteJetzt = (digitalRead(stummTastePin) == LOW);
  if (tasteJetzt && !letzteTasteGedrueckt) {
    stummgeschaltet = !stummgeschaltet;
    Serial.println(stummgeschaltet ? "🔇 Buzzer stummgeschaltet." : "🔊 Buzzer aktiv.");
    // Stummstatus auf LCD anzeigen
    lcd.setCursor(0, 1);
    lcd.print(stummgeschaltet ? "Stumm          " : "               ");
  }
  letzteTasteGedrueckt = tasteJetzt;

  // --- Messung im definierten Intervall ---
  if (jetzt - letzteZeit >= INTERVAL_MS) {
    letzteZeit = jetzt;

    int analogWert  = analogRead(wasserSensorAnalog);
    Alarmstufe stufe = bestimmeStufe(analogWert);

    // Nur bei Zustandswechsel reagieren
    if ((int)stufe != letzteStufe) {
      letzteStufe = (int)stufe;
      alarmanzeige(stufe, analogWert);
    }
  }
}

// ============================================================
// Alarmstufe anhand des Analogwerts bestimmen
Alarmstufe bestimmeStufe(int wert) {
  if (wert >= SCHWELLE_NASS)   return NASS;
  if (wert >= SCHWELLE_FEUCHT) return FEUCHT;
  return TROCKEN;
}

// ============================================================
// Ausgabe auf Serial, LCD und Alarm-Aktorik
void alarmanzeige(Alarmstufe stufe, int wert) {
  lcd.clear();

  switch (stufe) {

    case TROCKEN:
      noTone(summerPin);
      digitalWrite(ledPin, LOW);
      Serial.print("✔ TROCKEN  (Analogwert: ");
      Serial.print(wert);
      Serial.println(")");
      lcd.setCursor(0, 0); lcd.print("Status: TROCKEN ");
      lcd.setCursor(0, 1); lcd.print("Wert: "); lcd.print(wert);
      break;

    case FEUCHT:
      if (!stummgeschaltet) tone(summerPin, 800, 200);
      digitalWrite(ledPin, HIGH);
      Serial.print("⚠  FEUCHT  (Analogwert: ");
      Serial.print(wert);
      Serial.println(")");
      lcd.setCursor(0, 0); lcd.print("!  FEUCHT  !    ");
      lcd.setCursor(0, 1); lcd.print("Wert: "); lcd.print(wert);
      break;

    case NASS:
      if (!stummgeschaltet) tone(summerPin, 1200, 500);
      digitalWrite(ledPin, HIGH);
      Serial.print("🚨 NASS    (Analogwert: ");
      Serial.print(wert);
      Serial.println(")");
      lcd.setCursor(0, 0); lcd.print("!! ALARM: NASS !!");
      lcd.setCursor(0, 1); lcd.print("Wert: "); lcd.print(wert);
      break;
  }

  // Stummstatus auf Zeile 2 ergänzen (sofern noch Platz)
  if (stummgeschaltet) {
    lcd.setCursor(12, 1);
    lcd.print("STUMM");
  }
}
