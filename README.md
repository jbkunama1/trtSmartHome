# 💧 trtSmartHome – Wassermelder

![License](https://img.shields.io/badge/Lizenz-MIT-blue.svg)
![Language](https://img.shields.io/badge/Sprache-Arduino-teal.svg)
![Version](https://img.shields.io/badge/Version-1.1-green.svg)

> **trtSmartHome** ist ein pädagogisches Modellprojekt eines Smart Home, aufgebaut mit Arduino.  
> Dieses Modul implementiert einen **Wassermelder** (Water Leak Detection), der bei Wasserkontakt optisch (LED) und akustisch (Buzzer) Alarm schlägt.

---

## 📋 Inhaltsverzeichnis

1. [Versionsverlauf](#versionsverlauf)
2. [Projektbeschreibung](#projektbeschreibung)
3. [Materialliste](#materialliste)
4. [Pinbelegung](#pinbelegung)
5. [Sketch – Version 1.1](#sketch--version-11)
6. [Setup / Inbetriebnahme](#setup--inbetriebnahme)
7. [Hinweise](#hinweise)
8. [Dokumentation](#dokumentation)
9. [Lizenz](#lizenz)

---

## 🗂️ Versionsverlauf

| Version | Datei                                                                         | Highlights                                                      |
|---------|-------------------------------------------------------------------------------|-----------------------------------------------------------------|
| **1.1** | [`src/WaterSensor_v1_1/`](src/WaterSensor_v1_1/WaterSensor_v1_1.ino) ← aktuell | `millis()` statt `delay()`, Debouncing für stabile Erkennung   |
| 1.5     | [`src/WaterSensor_v1_5/`](src/WaterSensor_v1_5/WaterSensor_v1_5.ino)          | Basisversion – einfacher Alarm mit digitalem Sensor             |

---

## 📖 Projektbeschreibung

Das **trtSmartHome**-Projekt ist ein Lernmodell für den schulischen und außerschulischen Einsatz.  
Es simuliert ein intelligentes Zuhause (Smart Home) mithilfe eines Arduino-Mikrocontrollers.

Dieses Modul – der **Wassermelder** – erkennt Wasserkontakt am Sensor und reagiert sofort:

- 🔴 **Rote LED** leuchtet auf
- 🔊 **Summer (Buzzer)** ertönt
- 💻 **Serielle Ausgabe** gibt eine Meldung aus (`⚠ Wasser erkannt!`)

Bei trockenem Sensor bleibt alles ruhig und die Ausgabe lautet `✔ Kein Wasser erkannt.`

### ✨ Neu in Version 1.1

- **Nicht-blockierendes Timing:** `delay(500)` wurde durch `millis()` ersetzt. Der Arduino bleibt jederzeit reaktionsfähig – kein „Einfrieren" mehr während der Wartezeit.
- **Debouncing (Entprellung):** Erst nach **3 aufeinanderfolgenden gleichen Messwerten** wird ein Zustandswechsel akzeptiert. Fehlerhafte Kurzauslösungen durch Vibration oder Schmutz werden so zuverlässig unterdrückt.

---

## 📦 Materialliste

| Nr. | Komponente                       | Anzahl   |
|-----|----------------------------------|----------|
| 1   | Arduino Uno / Nano / Mega        | 1×       |
| 2   | Wasserstandssensor (z. B. YL-83) | 1×       |
| 3   | Summer (Buzzer, aktiv)           | 1×       |
| 4   | Rote LED                         | 1×       |
| 5   | Widerstand 220 Ω                 | 1×       |
| 6   | Jumper-Kabel                     | mehrere  |
| 7   | Breadboard                       | 1×       |
| 8   | USB-Kabel                        | 1×       |

---

## ⚡ Pinbelegung

| Komponente                    | Arduino-Pin | Modus          |
|-------------------------------|-------------|----------------|
| Wasserstandssensor (Signal)   | D2          | INPUT          |
| Summer (Buzzer)               | D8          | OUTPUT         |
| Rote LED                      | D7          | OUTPUT         |
| Wasserstandssensor (VCC)      | 5V          | Stromversorgung|
| Wasserstandssensor (GND)      | GND         | Masse          |

> 💡 **Hinweis:** Die rote LED wird über einen **220-Ω-Widerstand** an Pin D7 angeschlossen, um den Strom zu begrenzen.

---

## 🖥️ Sketch – Version 1.1

Datei: [`src/WaterSensor_v1_1/WaterSensor_v1_1.ino`](src/WaterSensor_v1_1/WaterSensor_v1_1.ino)

```cpp
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
int  letzterStatus    = HIGH;
int  roherStatus      = HIGH;
int  debounceZaehler  = 0;
unsigned long letzteZeit = 0;

void setup() {
  pinMode(wasserSensorPin, INPUT);
  pinMode(summerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("trtSmartHome – Wassermelder v1.1 bereit.");
}

void loop() {
  unsigned long jetzt = millis();

  if (jetzt - letzteZeit >= INTERVAL_MS) {
    letzteZeit = jetzt;

    int aktuellerRohwert = digitalRead(wasserSensorPin);

    if (aktuellerRohwert == roherStatus) {
      debounceZaehler++;
    } else {
      roherStatus     = aktuellerRohwert;
      debounceZaehler = 1;
    }

    if (debounceZaehler >= DEBOUNCE_COUNT && aktuellerRohwert != letzterStatus) {
      letzterStatus   = aktuellerRohwert;
      debounceZaehler = 0;
      alarmAktualisieren(letzterStatus);
    }
  }
}

void alarmAktualisieren(int status) {
  if (status == LOW) {
    digitalWrite(summerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    Serial.println("⚠ Wasser erkannt!");
  } else {
    digitalWrite(summerPin, LOW);
    digitalWrite(ledPin, LOW);
    Serial.println("✔ Kein Wasser erkannt.");
  }
}
```

---

## 🔧 Setup / Inbetriebnahme

1. **Hardware aufbauen:**
   - Wasserstandssensor (YL-83) an Breadboard anschließen
   - Signal-Pin (OUT) des Sensors → **D2** am Arduino
   - VCC des Sensors → **5V** am Arduino
   - GND des Sensors → **GND** am Arduino
   - Summer (Buzzer) → **D8** am Arduino (GND gemeinsam)
   - Rote LED über 220-Ω-Widerstand → **D7** am Arduino

2. **Arduino IDE öffnen:**
   - Datei `src/WaterSensor_v1_1/WaterSensor_v1_1.ino` laden

3. **Board und Port auswählen:**
   - Unter `Werkzeuge > Board` das richtige Arduino-Board wählen (z. B. *Arduino Uno*)
   - Unter `Werkzeuge > Port` den richtigen COM-Port auswählen

4. **Sketch hochladen:**
   - Auf den **Upload**-Button klicken (→ Pfeil-Symbol)

5. **Seriellen Monitor öffnen:**
   - `Werkzeuge > Serieller Monitor` öffnen
   - Baudrate auf **9600** einstellen
   - Status-Meldungen beobachten

6. **Testen:**
   - Sensor trocken → Ausgabe: `✔ Kein Wasser erkannt.`
   - Sensor mit Wasser in Kontakt bringen → Ausgabe: `⚠ Wasser erkannt!`, LED leuchtet, Buzzer ertönt

---

## 💡 Hinweise

- Der Sensor gibt bei **Wasserkontakt LOW** aus (aktiv-niedrig). Dies ist bei der Auswertung im Sketch berücksichtigt.
- Wird ein **passiver Buzzer** verwendet, muss der Sketch angepasst werden (PWM-Signal statt HIGH/LOW).
- Der **220-Ω-Widerstand** vor der LED ist zwingend erforderlich, um die LED nicht zu beschädigen.
- Der Sketch wurde für den Einsatz im Unterricht entwickelt und ist bewusst einfach gehalten.
- Baudrate des Seriellen Monitors muss auf **9600 Baud** eingestellt sein.
- **Debounce-Zähler** und **Intervall** können oben in den Konstanten `DEBOUNCE_COUNT` und `INTERVAL_MS` angepasst werden.

---

## 📄 Dokumentation

Die vollständige PDF-Dokumentation befindet sich im Ordner [`docs/`](docs/).

> Dateiname: `05_ArduinoCode.Pin.Funktion_Smarthome V1 5 Water.pdf`

---

## 📜 Lizenz

Dieses Projekt steht unter der **MIT-Lizenz**.  
Weitere Informationen siehe [LICENSE](LICENSE).
