# 💧 trtSmartHome – Wassermelder

![License](https://img.shields.io/badge/Lizenz-MIT-blue.svg)
![Language](https://img.shields.io/badge/Sprache-Arduino-teal.svg)
![Version](https://img.shields.io/badge/Version-2.0-green.svg)

> **trtSmartHome** ist ein pädagogisches Modellprojekt eines Smart Home, aufgebaut mit Arduino.  
> Dieses Modul implementiert einen **Wassermelder** (Water Leak Detection), der bei Wasserkontakt optisch (LED) und akustisch (Buzzer) Alarm schlägt.

---

## 📋 Inhaltsverzeichnis

1. [Versionsverlauf](#versionsverlauf)
2. [Projektbeschreibung](#projektbeschreibung)
3. [Materialliste](#materialliste)
4. [Pinbelegung](#pinbelegung)
5. [Sketch – Version 2.0](#sketch--version-20)
6. [Setup / Inbetriebnahme](#setup--inbetriebnahme)
7. [Hinweise](#hinweise)
8. [Dokumentation](#dokumentation)
9. [Lizenz](#lizenz)

---

## 🗂️ Versionsverlauf

| Version | Datei                                                                         | Highlights                                                                   |
|---------|-------------------------------------------------------------------------------|------------------------------------------------------------------------------|
| **2.0** | [`src/WaterSensor_v2/`](src/WaterSensor_v2/WaterSensor_v2.ino) ← aktuell     | Analoge Alarmstufen, LCD-Display (I2C), Stummschalttaste                     |
| 1.1     | [`src/WaterSensor_v1_1/`](src/WaterSensor_v1_1/WaterSensor_v1_1.ino)          | `millis()` statt `delay()`, Debouncing für stabile Erkennung                |
| 1.5     | [`src/WaterSensor_v1_5/`](src/WaterSensor_v1_5/WaterSensor_v1_5.ino)          | Basisversion – einfacher Alarm mit digitalem Sensor                          |

---

## 📖 Projektbeschreibung

Das **trtSmartHome**-Projekt ist ein Lernmodell für den schulischen und außerschulischen Einsatz.  
Es simuliert ein intelligentes Zuhause (Smart Home) mithilfe eines Arduino-Mikrocontrollers.

Dieses Modul – der **Wassermelder** – erkennt Wasserkontakt am Sensor und reagiert sofort:

- 🔴 **Rote LED** leuchtet auf
- 🔊 **Summer (Buzzer)** ertönt
- 💻 **Serielle Ausgabe** gibt eine Meldung aus (`⚠ Wasser erkannt!`)

Bei trockenem Sensor bleibt alles ruhig und die Ausgabe lautet `✔ Kein Wasser erkannt.`

### ✨ Neu in Version 2.0

- **Analoge Alarmstufen:** Statt nur EIN/AUS werden drei Stufen unterschieden:
  - ✅ **TROCKEN** – kein Alarm
  - ⚠️ **FEUCHT** – kurzer Warnton (800 Hz), LED leuchtet
  - 🚨 **NASS** – Dauerton (1200 Hz), LED leuchtet
- **LCD-Display (16×2, I2C):** Zeigt Alarmstufe und aktuellen Analogwert direkt am Gerät an – kein Serial Monitor nötig.
- **Stummschalttaste:** Drücken deaktiviert den Buzzer, LED und LCD bleiben aktiv. Erneutes Drücken reaktiviert ihn.
- Aufgebaut auf v1.1 (weiterhin `millis()` + Debouncing enthalten)

---

## 📦 Materialliste

| Nr. | Komponente                         | Anzahl   |
|-----|------------------------------------|----------|
| 1   | Arduino Uno / Nano / Mega          | 1×       |
| 2   | Wasserstandssensor (z. B. YL-83)   | 1×       |
| 3   | Summer (Buzzer, **passiv**)        | 1×       |
| 4   | Rote LED                           | 1×       |
| 5   | Widerstand 220 Ω                   | 1×       |
| 6   | LCD-Display 16×2 (I2C, z. B. PCF8574) | 1×    |
| 7   | Taster / Drucktaste                | 1×       |
| 8   | Jumper-Kabel                       | mehrere  |
| 9   | Breadboard                         | 1×       |
| 10  | USB-Kabel                          | 1×       |

---

## ⚡ Pinbelegung

| Komponente                    | Arduino-Pin | Modus          |
|-------------------------------|-------------|----------------|
| Wasserstandssensor (AO)       | A0          | ANALOG INPUT   |
| Wasserstandssensor (DO)       | D2          | INPUT          |
| Summer (Buzzer, passiv)       | D8          | OUTPUT (PWM)   |
| Rote LED                      | D7          | OUTPUT         |
| Stummschalttaste              | D3          | INPUT_PULLUP   |
| LCD SDA                       | A4 (SDA)    | I2C Daten      |
| LCD SCL                       | A5 (SCL)    | I2C Takt       |
| Wasserstandssensor (VCC)      | 5V          | Stromversorgung|
| Wasserstandssensor (GND)      | GND         | Masse          |
| LCD VCC                       | 5V          | Stromversorgung|
| LCD GND                       | GND         | Masse          |

> 💡 **Hinweis:** Die rote LED wird über einen **220-Ω-Widerstand** an Pin D7 angeschlossen.  
> 💡 **LCD-Adresse:** Standard ist `0x27`. Falls das Display nicht reagiert, `0x3F` im Sketch probieren.  
> 💡 **Buzzer:** In v2.0 wird `tone()` verwendet → **passiver Buzzer** erforderlich (nicht aktiv).

---

## 🖥️ Sketch – Version 2.0

Datei: [`src/WaterSensor_v2/WaterSensor_v2.ino`](src/WaterSensor_v2/WaterSensor_v2.ino)

```cpp
// ============================================================
// trtSmartHome – Wassermelder v2.0
// Analoge Alarmstufen, LCD (I2C), Stummschalttaste
// ============================================================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int wasserSensorAnalog  = A0;
const int summerPin           = 8;
const int ledPin              = 7;
const int stummTastePin       = 3;

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int SCHWELLE_FEUCHT = 100;
const int SCHWELLE_NASS   = 500;
const long INTERVAL_MS    = 500;

unsigned long letzteZeit = 0;
bool stummgeschaltet = false;
bool letzteTasteGedrueckt = false;
int  letzteStufe = -1;

enum Alarmstufe { TROCKEN, FEUCHT, NASS };

void setup() { /* ... */ }
void loop()  { /* ... */ }
```

> Den vollständigen Sketch findest du in [`src/WaterSensor_v2/WaterSensor_v2.ino`](src/WaterSensor_v2/WaterSensor_v2.ino).

---

## 🔧 Setup / Inbetriebnahme

1. **Bibliothek installieren:**
   - Arduino IDE öffnen → `Sketch > Bibliothek einbinden > Bibliotheken verwalten`
   - Nach **"LiquidCrystal I2C"** suchen (Autor: Frank de Brabander)
   - Installieren

2. **Hardware aufbauen:**
   - Wasserstandssensor (YL-83): AO → **A0**, VCC → **5V**, GND → **GND** am Arduino
   - Summer (passiv) → **D8** am Arduino
   - Rote LED über 220-Ω-Widerstand → **D7** am Arduino
   - Stummschalttaste zwischen **D3** und **GND** (kein Widerstand nötig, intern Pull-up)
   - LCD (I2C): VCC → **5V**, GND → **GND**, SDA → **A4**, SCL → **A5**

3. **Arduino IDE öffnen:**
   - Datei `src/WaterSensor_v2/WaterSensor_v2.ino` laden

4. **Board und Port auswählen:**
   - Unter `Werkzeuge > Board` das richtige Arduino-Board wählen
   - Unter `Werkzeuge > Port` den richtigen COM-Port auswählen

5. **Sketch hochladen:**
   - Auf den **Upload**-Button klicken

6. **Seriellen Monitor öffnen (optional):**
   - `Werkzeuge > Serieller Monitor`, Baudrate **9600**

7. **Testen:**
   - Sensor trocken → LCD: `Status: TROCKEN`, kein Alarm
   - Sensor leicht feucht → LCD: `! FEUCHT !`, kurzer Warnton, LED leuchtet
   - Sensor stark nass → LCD: `!! ALARM: NASS !!`, Dauerton, LED leuchtet
   - Taste drücken → Buzzer stummgeschaltet (LCD zeigt „STUMM")

---

## 💡 Hinweise

- Der Sensor gibt bei **Wasserkontakt niedrige Analogwerte** aus. Die Schwellen `SCHWELLE_FEUCHT` und `SCHWELLE_NASS` können im Sketch individuell angepasst werden.
- Ab v2.0 wird ein **passiver Buzzer** benötigt (aktiver Buzzer liefert mit `tone()` keinen definierten Ton).
- Der **220-Ω-Widerstand** vor der LED ist zwingend erforderlich.
- LCD-I2C-Adresse: Standard `0x27` – falls kein Bild erscheint, `0x3F` testen oder einen I2C-Scanner-Sketch verwenden.
- Baudrate des Seriellen Monitors muss auf **9600 Baud** eingestellt sein.

---

## 📄 Dokumentation

Die vollständige PDF-Dokumentation befindet sich im Ordner [`docs/`](docs/).

> Dateiname: `05_ArduinoCode.Pin.Funktion_Smarthome V1 5 Water.pdf`

---

## 📜 Lizenz

Dieses Projekt steht unter der **MIT-Lizenz**.  
Weitere Informationen siehe [LICENSE](LICENSE).
