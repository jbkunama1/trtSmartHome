[![Buy me a coffee](https://cdn.buymeacoffee.com/buttons/default-orange.png)](https://www.buymeacoffee.com/highfish)

# trtSmartHome – Arduino Smarthome Water Sensor V1.5

Ein Arduino-basiertes Smarthome-Projekt zur Wasserstandserkennung mit LED-Statusanzeige und Buzzer-Alarm.

## Beschreibung

Dieses Projekt überwacht mithilfe eines Wassersensor-Moduls, ob Wasser vorhanden ist (z. B. Überflutung, Leck). Bei Wasserentdeckung leuchtet eine LED auf und ein Buzzer gibt einen Alarm aus. Der Status wird zusätzlich über die serielle Schnittstelle ausgegeben.

## Pinbelegung

| Komponente             | Arduino Pin | Typ           |
|------------------------|-------------|---------------|
| Wassersensor (Signal OUT) | D2       | INPUT         |
| LED (Status)           | D7          | OUTPUT        |
| Buzzer                 | D8          | OUTPUT        |
| Wassersensor VCC       | 5V          | Stromversorgung |
| Wassersensor GND       | GND         | Masse         |
| LED GND                | GND         | Masse         |
| Buzzer GND             | GND         | Masse         |

## Materialliste

- 1× Arduino Uno (oder Nano/Mega)
- 1× Wassersensor-Modul (z. B. YL-83 oder vergleichbar)
- 1× Buzzer (aktiv oder passiv)
- 1× LED (rot oder gelb)
- 1× Widerstand 220 Ω (für LED)
- Jumperkabel
- Steckbrett (optional)
- USB-Kabel
- Netzteil (optional)

## Installation und Nutzung

1. **Arduino IDE** installieren (https://www.arduino.cc/en/software).
2. Dieses Repository klonen oder herunterladen:
   ```bash
   git clone https://github.com/jbkunama1/trtSmartHome.git
   ```
3. Die Datei `sketch/Water_Sensor_V1_5.ino` in der Arduino IDE öffnen.
4. Komponenten gemäß der Pinbelegungstabelle verkabeln.
5. Den Sketch auf das Arduino-Board hochladen (Board: **Arduino Uno**, Port auswählen).
6. Den seriellen Monitor öffnen (Baudrate: **9600**), um Statusmeldungen zu sehen.

### Hinweise

- Manche Wassersensoren melden `LOW` bei Wasserdetection, andere `HIGH`.  
  Falls das Verhalten umgekehrt erscheint, bitte den Vergleichswert im Sketch anpassen (siehe Kommentar in der `.ino`-Datei).
- Ein Vorwiderstand (220 Ω) für die LED verhindert deren Beschädigung.
- Aktive Buzzer benötigen nur HIGH/LOW; bei passiven Buzzern ggf. `tone()` verwenden.

## Projektstruktur

```
trtSmartHome/
├── README.md
├── LICENSE
├── sketch/
│   └── Water_Sensor_V1_5.ino
└── docs/
    ├── Beschreibung.md
    ├── Pinbelegung.md
    └── Materialliste.md
```

## Version

**V1.5** – Stand: 2026-03-09

## Lizenz

Dieses Projekt steht unter der [MIT-Lizenz](LICENSE).
# 💧 trtSmartHome – Wassermelder

![License](https://img.shields.io/badge/Lizenz-MIT-blue.svg)
![Language](https://img.shields.io/badge/Sprache-Arduino-teal.svg)
![Version](https://img.shields.io/badge/Version-1.5-green.svg)

> **trtSmartHome** ist ein pädagogisches Modellprojekt eines Smart Home, aufgebaut mit Arduino.  
> Dieses Modul implementiert einen **Wassermelder** (Water Leak Detection), der bei Wasserkontakt optisch (LED) und akustisch (Buzzer) Alarm schlägt.

---

## 📋 Inhaltsverzeichnis

1. [Projektbeschreibung](#projektbeschreibung)
2. [Materialliste](#materialliste)
3. [Pinbelegung](#pinbelegung)
4. [Sketch – Version 1.5](#sketch--version-15)
5. [Setup / Inbetriebnahme](#setup--inbetriebnahme)
6. [Hinweise](#hinweise)
7. [Dokumentation](#dokumentation)
8. [Lizenz](#lizenz)

---

## 📖 Projektbeschreibung

Das **trtSmartHome**-Projekt ist ein Lernmodell für den schulischen und außerschulischen Einsatz.  
Es simuliert ein intelligentes Zuhause (Smart Home) mithilfe eines Arduino-Mikrocontrollers.

Dieses Modul – der **Wassermelder** – erkennt Wasserkontakt am Sensor und reagiert sofort:

- 🔴 **Rote LED** leuchtet auf
- 🔊 **Summer (Buzzer)** ertönt
- 💻 **Serielle Ausgabe** gibt eine Meldung aus (`⚠ Wasser erkannt!`)

Bei trockenem Sensor bleibt alles ruhig und die Ausgabe lautet `✔ Kein Wasser erkannt.`

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

## 🖥️ Sketch – Version 1.5

Datei: [`src/WaterSensor_v1_5/WaterSensor_v1_5.ino`](src/WaterSensor_v1_5/WaterSensor_v1_5.ino)

```cpp
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
   - Datei `src/WaterSensor_v1_5/WaterSensor_v1_5.ino` laden

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

---

## 📄 Dokumentation

Die vollständige PDF-Dokumentation befindet sich im Ordner [`docs/`](docs/).

> Dateiname: `05_ArduinoCode.Pin.Funktion_Smarthome V1 5 Water.pdf`

---

## 📜 Lizenz

Dieses Projekt steht unter der **MIT-Lizenz**.  
Weitere Informationen siehe [LICENSE](LICENSE).
