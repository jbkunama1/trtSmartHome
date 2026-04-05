# Projektbeschreibung – trtSmartHome Wasserstandssensor V1.5

## Was macht das Projekt?

Das Projekt **trtSmartHome – Wasserstandssensor V1.5** ist ein Arduino-basiertes Smarthome-Modul, das Wasser erkennt und bei Detektion sofort optisch (LED) und akustisch (Buzzer) Alarm schlägt. Gleichzeitig wird eine Statusmeldung über den seriellen Monitor ausgegeben, sodass der Zustand jederzeit nachverfolgt werden kann.

## Wie funktioniert der Wasserstandssensor?

Ein Wassersensor-Modul (z. B. YL-83) besteht aus einer Platine mit einer Reihe leitfähiger Leiterbahnen. Befinden sich diese Leiterbahnen im Wasser, schließt das Wasser einen elektrischen Stromkreis. Das Modul gibt daraufhin an seinem digitalen Ausgang (OUT) ein Signal aus – je nach Sensortyp `LOW` oder `HIGH`. Der Arduino liest dieses Signal an Pin D2 aus und reagiert entsprechend.

### Signalerklärung

| Sensor-Zustand | Ausgangssignal (typisch) | Arduino-Reaktion |
|----------------|--------------------------|------------------|
| Kein Wasser    | HIGH                     | LED aus, Buzzer aus |
| Wasser erkannt | LOW                      | LED an, Buzzer an |

> **Hinweis:** Manche Sensoren verhalten sich umgekehrt. In diesem Fall muss der Vergleichswert im Sketch von `LOW` auf `HIGH` geändert werden.

## Anwendungsfälle

- **Keller-Wasseralarm:** Frühzeitige Erkennung von einlaufendem Wasser bei Starkregen oder Rohrbruch.
- **Aquarium-Überlaufschutz:** Alarm bei zu hohem Wasserstand im Aquarium.
- **Waschmaschinen-Leckage:** Detektion von auslaufendem Wasser unter der Waschmaschine.
- **Regensammler-Füllstandsanzeige:** Überwachung des Füllstands eines Regensammlers.
- **Gewächshaus-Bewässerung:** Überprüfung, ob Bewässerungsanlagen lecken.

## Erweiterungsmöglichkeiten

| Erweiterung             | Beschreibung |
|-------------------------|--------------|
| **WLAN-Anbindung**      | Mit einem ESP8266 oder ESP32 kann der Alarm über WLAN gesendet werden. |
| **App-Benachrichtigung**| Via MQTT oder HTTP-Webhook können Push-Nachrichten an ein Smartphone gesendet werden. |
| **Datenspeicherung**    | Ereignisse können auf einer SD-Karte protokolliert werden. |
| **Display-Anzeige**     | Ein OLED- oder LCD-Display zeigt den Status direkt vor Ort an. |
| **Mehrere Sensoren**    | Durch Erweiterung des Sketches können mehrere Bereiche gleichzeitig überwacht werden. |
| **Energiesparmodus**    | Mit dem Arduino-Schlafmodus kann der Stromverbrauch reduziert werden. |

## Version

**V1.5** – Stand: 2026-03-09
