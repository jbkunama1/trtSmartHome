# Materialliste – trtSmartHome Wasserstandssensor V1.5

## Benötigte Komponenten

| Komponente              | Anzahl | Typ / Beschreibung                        | Bezugshinweis                        |
|-------------------------|--------|-------------------------------------------|--------------------------------------|
| Arduino Uno             | 1×     | Mikrocontroller-Board (Uno, Nano oder Mega) | Arduino-Store, Amazon, Conrad, Reichelt |
| Wassersensor-Modul      | 1×     | z. B. YL-83, FC-28 oder vergleichbar      | Amazon, AliExpress, Reichelt         |
| Buzzer                  | 1×     | Aktiver Buzzer (5V, empfohlen)            | Amazon, AliExpress, Conrad           |
| LED                     | 1×     | Rot oder gelb, 5 mm, 2V / 20 mA          | Conrad, Reichelt, Amazon             |
| Widerstand              | 1×     | 220 Ω (für LED-Schutz)                    | Conrad, Reichelt, Amazon             |
| Jumperkabel             | ~10×   | Male-to-Male oder Male-to-Female          | Amazon, AliExpress                   |
| Steckbrett (Breadboard) | 1×     | 400 oder 830 Punkte (optional)            | Amazon, Conrad, AliExpress           |
| USB-Kabel               | 1×     | USB-A auf USB-B (für Arduino Uno)         | Im Arduino-Lieferumfang / Amazon     |
| Netzteil                | 1×     | 5V DC (optional, für Dauerbetrieb)        | Conrad, Amazon                       |

## Hinweise zu Alternativen

### Arduino-Board
Statt des Arduino Uno können auch folgende Boards verwendet werden:
- **Arduino Nano** – kleiner, günstig, gleiche Pinbelegung
- **Arduino Mega** – mehr Pins, für größere Projekte
- **ESP8266 / ESP32** – mit WLAN-Funktionalität für IoT-Erweiterungen

### Wassersensor
Gängige kompatible Sensoren:
- **YL-83** – einfacher Resistenz-basierter Sensor
- **FC-28** – ähnlich, mit digitalem und analogem Ausgang
- **Capacitive Soil Moisture Sensor** – korrosionsbeständiger durch kapazitives Messprinzip

### Buzzer
- **Aktiver Buzzer:** Erzeugt selbstständig Ton bei angelegter Spannung → einfacher zu verwenden.
- **Passiver Buzzer:** Benötigt ein PWM-Signal (z. B. `tone()`-Funktion) → flexiblere Tonsteuerung.

### LED
- Farbe ist frei wählbar (rot für Alarm empfohlen).
- Der Vorwiderstand (220 Ω) ist bei 5V obligatorisch, um die LED nicht zu beschädigen.

## Version

**V1.5** – Stand: 2026-03-09
