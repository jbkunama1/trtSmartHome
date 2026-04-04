# 💧 trtSmartHome – Wassermelder

![License](https://img.shields.io/badge/Lizenz-MIT-blue.svg)
![Language](https://img.shields.io/badge/Sprache-Arduino-teal.svg)
![Version](https://img.shields.io/badge/Version-3.0-green.svg)

> **trtSmartHome** ist ein pädagogisches Modellprojekt eines Smart Home, aufgebaut mit Arduino.  
> Dieses Modul implementiert einen **Wassermelder** (Water Leak Detection), der bei Wasserkontakt optisch (LED) und akustisch (Buzzer) Alarm schlägt.

---

## 📋 Inhaltsverzeichnis

1. [Versionsverlauf](#versionsverlauf)
2. [Projektbeschreibung](#projektbeschreibung)
3. [Materialliste](#materialliste)
4. [Pinbelegung](#pinbelegung)
5. [Sketch – Version 3.0](#sketch--version-30)
6. [Konfiguration](#konfiguration)
7. [Setup / Inbetriebnahme](#setup--inbetriebnahme)
8. [Webinterface](#webinterface)
9. [Telegram-Benachrichtigung](#telegram-benachrichtigung)
10. [Hinweise](#hinweise)
11. [Dokumentation](#dokumentation)
12. [Lizenz](#lizenz)

---

## 🗂️ Versionsverlauf

| Version | Datei                                                                         | Highlights                                                                   |
|---------|-------------------------------------------------------------------------------|------------------------------------------------------------------------------|
| **3.0** | [`src/WaterSensor_v3/`](src/WaterSensor_v3/WaterSensor_v3.ino) ← aktuell     | ESP32, WLAN, Telegram-Alarm, Webinterface, NTP-Zeitstempel                  |
| 2.0     | [`src/WaterSensor_v2/`](src/WaterSensor_v2/WaterSensor_v2.ino)               | Analoge Alarmstufen, LCD-Display (I2C), Stummschalttaste                    |
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

### ✨ Neu in Version 3.0

- **ESP32-Plattform:** Leistungsstärkerer Mikrocontroller mit eingebautem WLAN/Bluetooth.
- **WLAN-Anbindung:** Verbindet sich automatisch mit dem konfigurierten Netzwerk.
- **Telegram-Benachrichtigung:** Sendet bei Alarm sofort eine Nachricht an einen Telegram-Bot (mit konfigurierter Mindestpause, um Flut-Nachrichten zu vermeiden).
- **Webinterface:** Einfache Statusseite im Browser – zeigt Alarmstufe, Buzzer-Status und das Ereignisprotokoll. Aktualisiert sich alle 5 Sekunden automatisch.
- **NTP-Zeitstempel:** Synchronisiert die Uhrzeit über das Internet; jedes Ereignis wird mit Datum und Uhrzeit protokolliert.
- **Ereignisprotokoll:** Die letzten 10 Ereignisse werden im Speicher gehalten und im Webinterface angezeigt.
- Aufgebaut auf v2.0 (weiterhin analoge Alarmstufen + Stummschalttaste enthalten)

---

## 📦 Materialliste

| Nr. | Komponente                              | Anzahl   |
|-----|-----------------------------------------|----------|
| 1   | ESP32 DevKit V1 (oder kompatibel)       | 1×       |
| 2   | Wasserstandssensor (z. B. YL-83)        | 1×       |
| 3   | Summer (Buzzer, **passiv**)             | 1×       |
| 4   | Rote LED                                | 1×       |
| 5   | Widerstand 220 Ω                        | 1×       |
| 6   | Taster / Drucktaste                     | 1×       |
| 7   | Jumper-Kabel                            | mehrere  |
| 8   | Breadboard                              | 1×       |
| 9   | USB-Kabel (Micro-USB oder USB-C)        | 1×       |
| 10  | WLAN-Router (2,4 GHz)                   | 1×       |

---

## ⚡ Pinbelegung

| Komponente                    | ESP32-Pin   | Modus          |
|-------------------------------|-------------|----------------|
| Wasserstandssensor (AO)       | GPIO34      | ANALOG INPUT   |
| Summer (Buzzer, passiv)       | GPIO26      | OUTPUT (PWM)   |
| Rote LED                      | GPIO27      | OUTPUT         |
| Stummschalttaste              | GPIO25      | INPUT_PULLUP   |
| Wasserstandssensor (VCC)      | 3V3 oder 5V | Stromversorgung|
| Wasserstandssensor (GND)      | GND         | Masse          |

> 💡 **GPIO34** ist auf dem ESP32 nur als Eingang nutzbar (kein OUTPUT, kein PWM) – ideal für den Sensor.  
> 💡 Der ESP32 hat eingebautes WLAN – kein Shield oder Zusatzmodul nötig.  
> 💡 **Passiver Buzzer** erforderlich (wird mit `tone()` / PWM angesteuert).

---

## 🖥️ Sketch – Version 3.0

Datei: [`src/WaterSensor_v3/WaterSensor_v3.ino`](src/WaterSensor_v3/WaterSensor_v3.ino)

Den vollständigen, gut kommentierten Sketch findest du direkt in der Datei oben.  
Hier die wichtigsten Konfigurationszeilen:

```cpp
const char* WIFI_SSID      = "DEIN_WLAN_NAME";
const char* WIFI_PASSWORT  = "DEIN_WLAN_PASSWORT";
const char* BOT_TOKEN      = "DEIN_TELEGRAM_BOT_TOKEN";
const char* CHAT_ID        = "DEINE_TELEGRAM_CHAT_ID";
```

---

## ⚙️ Konfiguration

### Telegram-Bot einrichten
1. In Telegram [@BotFather](https://t.me/BotFather) öffnen
2. `/newbot` eingeben und den Anweisungen folgen
3. Den ausgegebenen **Bot-Token** in `BOT_TOKEN` eintragen
4. Den Bot anschreiben, dann über `https://api.telegram.org/bot<TOKEN>/getUpdates` die `chat.id` ermitteln und in `CHAT_ID` eintragen

### Schwellenwerte anpassen
```cpp
const int SCHWELLE_FEUCHT = 500;   // ESP32 ADC: 0–4095
const int SCHWELLE_NASS   = 2000;
```
Diese Werte im Seriellen Monitor bei trockenem und nassem Sensor ablesen und anpassen.

---

## 🔧 Setup / Inbetriebnahme

1. **ESP32-Board-Package installieren:**
   - Arduino IDE → `Datei > Voreinstellungen > Zusätzliche Boardverwalter-URLs`:
     `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Dann: `Werkzeuge > Board > Boardverwalter` → nach **"esp32"** suchen → installieren

2. **Bibliotheken installieren:**
   - `Sketch > Bibliothek einbinden > Bibliotheken verwalten`
   - **UniversalTelegramBot** (von Brian Lough) installieren
   - **ArduinoJson** (von Benoit Blanchon, Version 6.x) installieren

3. **WLAN und Telegram konfigurieren:**
   - `src/WaterSensor_v3/WaterSensor_v3.ino` öffnen
   - `WIFI_SSID`, `WIFI_PASSWORT`, `BOT_TOKEN` und `CHAT_ID` eintragen (siehe [Konfiguration](#konfiguration))

4. **Hardware aufbauen:**
   - Wasserstandssensor: AO → **GPIO34**, VCC → **3V3** oder 5V, GND → **GND**
   - Summer (passiv) → **GPIO26**
   - Rote LED über 220-Ω-Widerstand → **GPIO27**
   - Stummschalttaste zwischen **GPIO25** und **GND**

5. **Board auswählen:**
   - `Werkzeuge > Board > ESP32 Arduino > ESP32 Dev Module`
   - `Werkzeuge > Port` → richtigen COM-Port wählen

6. **Sketch hochladen und testen:**
   - Upload-Button klicken
   - Seriellen Monitor auf **115200 Baud** öffnen
   - IP-Adresse ablesen, im Browser öffnen: `http://<IP-Adresse>`
   - Sensor mit Wasser in Kontakt bringen → Telegram-Nachricht erscheint, Webinterface aktualisiert sich

---

---

## 🌐 Webinterface

Nach dem Start ist das Webinterface im lokalen Netzwerk erreichbar:

```
http://<IP-Adresse>/        → Statusseite (auto-refresh alle 5 s)
http://<IP-Adresse>/stumm   → Buzzer ein-/ausschalten
```

Die IP-Adresse wird beim Start im Seriellen Monitor ausgegeben.

---

## 📱 Telegram-Benachrichtigung

Bei Übergang in Stufe **FEUCHT** oder **NASS** wird automatisch eine Nachricht gesendet.  
Um Nachrichten-Flut zu vermeiden, beträgt die Mindestpause zwischen zwei Nachrichten **60 Sekunden** (konfigurierbar via `TELEGRAM_PAUSE_MS`).

Beispielnachricht:
```
🏠 trtSmartHome Wassermelder
ALARM – Wasser erkannt (NASS), Wert: 2345
04.04.2026 14:32:11
```

---

## 💡 Hinweise

- **WLAN:** Der ESP32 unterstützt nur **2,4-GHz**-Netzwerke (kein 5 GHz).
- **Serieller Monitor:** Baudrate auf **115200 Baud** einstellen.
- **Schwellenwerte:** Der ESP32-ADC arbeitet mit 0–4095 (12 Bit). Werte im trockenen und nassen Zustand im Seriellen Monitor ablesen und `SCHWELLE_FEUCHT` / `SCHWELLE_NASS` entsprechend anpassen.
- **Telegram-Token:** Niemals öffentlich teilen oder in ein öffentliches Repository einchecken.
- **Offline-Modus:** Ist kein WLAN verfügbar, arbeitet der Sketch als lokaler Alarm-Melder (kein Web, kein Telegram, aber LED + Buzzer funktionieren weiterhin).

---

## 📄 Dokumentation

Die vollständige PDF-Dokumentation befindet sich im Ordner [`docs/`](docs/).

> Dateiname: `05_ArduinoCode.Pin.Funktion_Smarthome V1 5 Water.pdf`

---

## 📜 Lizenz

Dieses Projekt steht unter der **MIT-Lizenz**.  
Weitere Informationen siehe [LICENSE](LICENSE).
