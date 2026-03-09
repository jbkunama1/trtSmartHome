# Pinbelegung – trtSmartHome Wasserstandssensor V1.5

## Übersicht der Pinbelegung

| Komponente                | Arduino Pin | Typ             |
|---------------------------|-------------|-----------------|
| Wassersensor (Signal OUT) | D2          | INPUT (Digital) |
| LED (Status-Anzeige)      | D7          | OUTPUT (Digital)|
| Buzzer (Alarm)            | D8          | OUTPUT (Digital)|
| Wassersensor VCC          | 5V          | Stromversorgung |
| Wassersensor GND          | GND         | Masse           |
| LED GND (über 220 Ω)      | GND         | Masse           |
| Buzzer GND                | GND         | Masse           |

## Komponentenbeschreibung

### Wassersensor-Modul
- **Pin:** D2 (Digital Input)
- Das Modul gibt bei erkanntem Wasser ein digitales Signal aus (typisch `LOW`).
- Wird mit 5V und GND des Arduino versorgt.

### LED (Status-Anzeige)
- **Pin:** D7 (Digital Output)
- Leuchtet, wenn Wasser erkannt wird.
- **Wichtig:** Immer einen Vorwiderstand (220 Ω) zwischen Arduino-Pin und LED einbauen, um die LED vor Überstrom zu schützen.

### Buzzer (Alarm)
- **Pin:** D8 (Digital Output)
- Ertönt, wenn Wasser erkannt wird.
- Für aktive Buzzer reicht `HIGH`/`LOW`; bei passiven Buzzern ggf. `tone()` verwenden.

## Verkabelungshinweise

1. **Wassersensor:**
   - `VCC` → Arduino **5V**
   - `GND` → Arduino **GND**
   - `OUT` (Signal) → Arduino **D2**

2. **LED:**
   - Pluspol (langer Draht / Anode) → Widerstand 220 Ω → Arduino **D7**
   - Minuspol (kurzer Draht / Kathode) → Arduino **GND**

3. **Buzzer:**
   - `+` (Pluspol) → Arduino **D8**
   - `-` (Minuspol) → Arduino **GND**

> **Hinweis:** Alle GND-Verbindungen können auf die gleiche GND-Schiene des Steckbretts geführt werden, die dann mit einem einzigen Kabel mit dem Arduino GND verbunden wird.

## Version

**V1.5** – Stand: 2026-03-09
