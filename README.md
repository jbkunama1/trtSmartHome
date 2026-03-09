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
