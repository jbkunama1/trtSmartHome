// ============================================================
// trtSmartHome – Wassermelder (Smarthome Modell)
// Version: 3.0
// Plattform: ESP32 (z. B. ESP32 DevKit V1)
//
// Beschreibung:
//   - Analoge Alarmstufen (TROCKEN / FEUCHT / NASS)
//   - WLAN-Anbindung (WiFi)
//   - Telegram-Benachrichtigung bei Alarm
//   - Webinterface (Port 80) mit Live-Status und Ereignisprotokoll
//   - NTP-Zeitstempel (automatische Zeitsynchronisation)
//   - Stummschalttaste (Hardware) und per Web (/stumm)
//
// Benötigte Bibliotheken (Arduino IDE – Bibliotheksverwalter):
//   1. UniversalTelegramBot  (von Brian Lough)
//   2. ArduinoJson           (ab Version 6.x, von Benoit Blanchon)
//   3. ESP32 Board-Package   (Espressif Systems, via Boardverwalter)
//
// WLAN, Bot-Token und Chat-ID in den Konstanten unten eintragen.
// ============================================================

#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <time.h>

// ============================================================
// *** KONFIGURATION – hier anpassen ***
// ============================================================
const char* WIFI_SSID      = "DEIN_WLAN_NAME";
const char* WIFI_PASSWORT  = "DEIN_WLAN_PASSWORT";
const char* BOT_TOKEN      = "DEIN_TELEGRAM_BOT_TOKEN";
const char* CHAT_ID        = "DEINE_TELEGRAM_CHAT_ID";

// NTP-Zeitzone: UTC+1 (Winterzeit) / UTC+2 (Sommerzeit → +3600 Offset)
const long  GMT_OFFSET_SEC    = 3600;
const int   SOMMER_OFFSET_SEC = 3600;
const char* NTP_SERVER        = "pool.ntp.org";
// ============================================================

// --- Pin-Belegung (ESP32 DevKit) ---
const int wasserSensorPin = 34;  // Analoger Eingang (GPIO34, nur Input, kein PWM)
const int summerPin       = 26;  // Summer (Buzzer, passiv)
const int ledPin          = 27;  // Rote LED
const int stummTastePin   = 25;  // Stummschalttaste (gegen GND)

// --- Schwellenwerte (ESP32 ADC: 0–4095) ---
const int SCHWELLE_FEUCHT = 500;
const int SCHWELLE_NASS   = 2000;

// --- Timing ---
const long INTERVAL_MS        = 500;
const long TELEGRAM_PAUSE_MS  = 60000; // Mindestpause zwischen Telegram-Nachrichten

unsigned long letzteZeit           = 0;
unsigned long letzteTelegramZeit   = 0;

// --- Zustandsvariablen ---
bool stummgeschaltet       = false;
bool letzteTasteGedrueckt  = false;
int  letzteStufe           = -1;

enum Alarmstufe { TROCKEN, FEUCHT, NASS };

// --- Ereignisprotokoll (ringförmig, letzte 10 Einträge) ---
const int  MAX_LOG = 10;
String     ereignisLog[MAX_LOG];
int        logIndex = 0;

// --- Web-Server auf Port 80 ---
WebServer server(80);

// --- Telegram ---
WiFiClientSecure securedClient;
UniversalTelegramBot bot(BOT_TOKEN, securedClient);

// ============================================================
// Hilfsfunktion: Zeitstempel als String
// ============================================================
String getZeitstempel() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "--:--:--";
  char buf[20];
  strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", &timeinfo);
  return String(buf);
}

// ============================================================
// Hilfsfunktion: Log-Eintrag hinzufügen
// ============================================================
void logEreignis(const String& text) {
  ereignisLog[logIndex] = getZeitstempel() + "  " + text;
  logIndex = (logIndex + 1) % MAX_LOG;
}

// ============================================================
// Webserver – Startseite
// ============================================================
void handleRoot() {
  String stufeText;
  switch (letzteStufe) {
    case 0: stufeText = "&#10004; TROCKEN";  break;
    case 1: stufeText = "&#9888; FEUCHT";    break;
    case 2: stufeText = "&#128680; NASS";    break;
    default: stufeText = "Unbekannt";        break;
  }

  String html =
    "<!DOCTYPE html><html><head>"
    "<meta charset='UTF-8'>"
    "<meta http-equiv='refresh' content='5'>"
    "<title>trtSmartHome Wassermelder</title>"
    "<style>"
    "body{font-family:Arial,sans-serif;margin:24px;background:#f5f5f5;color:#333}"
    "h1{color:#1a73e8}h2{color:#444;margin-top:32px}"
    "table{border-collapse:collapse;width:100%;max-width:600px}"
    "td,th{border:1px solid #ccc;padding:10px;text-align:left}"
    "tr:nth-child(even){background:#e8e8e8}"
    "a.btn{display:inline-block;margin-top:16px;padding:8px 18px;"
    "background:#1a73e8;color:#fff;text-decoration:none;border-radius:4px}"
    "a.btn:hover{background:#1558b0}"
    "</style></head><body>"
    "<h1>&#128167; trtSmartHome – Wassermelder v3.0</h1>"
    "<h2>Aktueller Status</h2>"
    "<table>"
    "<tr><th>Parameter</th><th>Wert</th></tr>"
    "<tr><td>Alarmstufe</td><td>" + stufeText + "</td></tr>"
    "<tr><td>Buzzer</td><td>" + String(stummgeschaltet ? "&#128263; Stummgeschaltet" : "&#128266; Aktiv") + "</td></tr>"
    "<tr><td>IP-Adresse</td><td>" + WiFi.localIP().toString() + "</td></tr>"
    "<tr><td>Uhrzeit</td><td>" + getZeitstempel() + "</td></tr>"
    "</table>"
    "<a class='btn' href='/stumm'>Buzzer umschalten</a>"
    "<h2>Ereignisprotokoll</h2>"
    "<table><tr><th>#</th><th>Eintrag</th></tr>";

  for (int i = 0; i < MAX_LOG; i++) {
    int idx = (logIndex - 1 - i + MAX_LOG) % MAX_LOG;
    if (ereignisLog[idx].length() > 0) {
      html += "<tr><td>" + String(i + 1) + "</td><td>" + ereignisLog[idx] + "</td></tr>";
    }
  }
  html += "</table></body></html>";
  server.send(200, "text/html; charset=UTF-8", html);
}

// ============================================================
// Webserver – Buzzer umschalten
// ============================================================
void handleStumm() {
  stummgeschaltet = !stummgeschaltet;
  String antwort = stummgeschaltet ? "Buzzer stummgeschaltet." : "Buzzer aktiv.";
  logEreignis(antwort);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", antwort);
}

// ============================================================
// Alarmstufe bestimmen
// ============================================================
Alarmstufe bestimmeStufe(int wert) {
  if (wert >= SCHWELLE_NASS)   return NASS;
  if (wert >= SCHWELLE_FEUCHT) return FEUCHT;
  return TROCKEN;
}

// ============================================================
// Alarm anzeigen, loggen und Telegram-Benachrichtigung senden
// ============================================================
void alarmanzeige(Alarmstufe stufe, int wert) {
  unsigned long jetzt = millis();
  String nachricht;

  switch (stufe) {
    case TROCKEN:
      noTone(summerPin);
      digitalWrite(ledPin, LOW);
      nachricht = "Kein Wasser (TROCKEN), Wert: " + String(wert);
      Serial.println("✔ " + nachricht);
      break;

    case FEUCHT:
      if (!stummgeschaltet) tone(summerPin, 800, 200);
      digitalWrite(ledPin, HIGH);
      nachricht = "Sensor FEUCHT, Wert: " + String(wert);
      Serial.println("⚠ " + nachricht);
      break;

    case NASS:
      if (!stummgeschaltet) tone(summerPin, 1200, 500);
      digitalWrite(ledPin, HIGH);
      nachricht = "ALARM – Wasser erkannt (NASS), Wert: " + String(wert);
      Serial.println("🚨 " + nachricht);
      break;
  }

  logEreignis(nachricht);

  // Telegram-Benachrichtigung bei Alarm (Mindestpause einhalten)
  if (stufe != TROCKEN && WiFi.status() == WL_CONNECTED &&
      (jetzt - letzteTelegramZeit >= TELEGRAM_PAUSE_MS)) {
    letzteTelegramZeit = jetzt;
    String tgText = "🏠 *trtSmartHome Wassermelder*\n"
                    + nachricht + "\n"
                    + getZeitstempel();
    bot.sendMessage(CHAT_ID, tgText, "Markdown");
  }
}

// ============================================================
// setup()
// ============================================================
void setup() {
  pinMode(summerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(stummTastePin, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("\ntrtSmartHome – Wassermelder v3.0 startet...");

  // WLAN verbinden
  WiFi.begin(WIFI_SSID, WIFI_PASSWORT);
  Serial.print("WLAN wird verbunden");
  int versuche = 0;
  while (WiFi.status() != WL_CONNECTED && versuche < 20) {
    delay(500);
    Serial.print(".");
    versuche++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWLAN verbunden – IP: " + WiFi.localIP().toString());

    // NTP-Zeitsynchronisation
    configTime(GMT_OFFSET_SEC, SOMMER_OFFSET_SEC, NTP_SERVER);
    Serial.println("NTP-Zeitsynchronisation läuft...");

    // Telegram: SSL-Zertifikat ignorieren (vereinfachte Schulumgebung)
    securedClient.setInsecure();

    // Startnachricht via Telegram
    String startMsg = "🏠 trtSmartHome Wassermelder v3.0 gestartet.\nIP: " +
                      WiFi.localIP().toString();
    bot.sendMessage(CHAT_ID, startMsg, "");

    // Webserver konfigurieren und starten
    server.on("/", handleRoot);
    server.on("/stumm", handleStumm);
    server.begin();
    Serial.println("Webserver gestartet auf http://" + WiFi.localIP().toString());

  } else {
    Serial.println("\nWLAN nicht erreichbar – Offline-Modus (kein Web, kein Telegram).");
  }

  Serial.println("Schwellen: FEUCHT >= " + String(SCHWELLE_FEUCHT) +
                 " | NASS >= " + String(SCHWELLE_NASS));
}

// ============================================================
// loop()
// ============================================================
void loop() {
  // Webserver-Anfragen bearbeiten
  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  }

  unsigned long jetzt = millis();

  // Stummschalttaste entprellt abfragen
  bool tasteJetzt = (digitalRead(stummTastePin) == LOW);
  if (tasteJetzt && !letzteTasteGedrueckt) {
    stummgeschaltet = !stummgeschaltet;
    String msg = stummgeschaltet ? "Buzzer stummgeschaltet." : "Buzzer aktiv.";
    Serial.println(msg);
    logEreignis(msg);
  }
  letzteTasteGedrueckt = tasteJetzt;

  // Messung im definierten Intervall
  if (jetzt - letzteZeit >= INTERVAL_MS) {
    letzteZeit = jetzt;

    int analogWert  = analogRead(wasserSensorPin);
    Alarmstufe stufe = bestimmeStufe(analogWert);

    if ((int)stufe != letzteStufe) {
      letzteStufe = (int)stufe;
      alarmanzeige(stufe, analogWert);
    }
  }
}
