# Interaktive Lampe - Intelligente LED-Lampe mit Musik

## 🎯 Projektübersicht
Eine intelligente Lampe mit **WS2812B LED-Strips**, gesteuert von einem **ESP8266** mit **WLED-Firmware**, **DFPlayer Mini MP3-Player** und **zwei programmierbaren Drucktastern**.

## 🔧 Hardware-Komponenten
- **ESP8266 ESP-12F** - Hauptcontroller
- **WS2812B LED-Strips** - 2 separate Strips (28 + 10 LEDs)
- **DFPlayer Mini** - MP3-Wiedergabe
- **2x 12mm Edelstahl-Drucktaster** - Steuerung (IP67 wasserdicht)
- **5V/3.3V Spannungsversorgung**
- **1000µF Kondensator** - für stabile LED-Versorgung

## 🎮 Button-Funktionen

### Button 1 (GPIO12/D6): AN/AUS + Presets
- **Kurzer Druck**: Lampe AN/AUS
- **Langer Druck**: Preset wechseln (1-3)
- **Doppel-Klick**: Helligkeit ändern

### Button 2 (GPIO13/D7): Musik + Zusätzlich
- **Kurzer Druck**: Musik Start/Stop + **automatisches Musik-Preset laden**
- **Langer Druck**: Nächster Track
- **Doppel-Klick**: Lautstärke ändern

## ⚡ Verkabelung

### ESP8266MOD Entwicklungsboard
```
Obere Reihe:
- TX    -> nicht verwendet (Serial Debug)
- RX    -> nicht verwendet (Serial Debug)
- D1    -> DFPlayer Mini RX (GPIO5)
- D2    -> DFPlayer Mini TX (GPIO4)
- D3    -> WS2812B Strip 2 DIN (GPIO0) - 10 LEDs
- D4    -> WS2812B Strip 1 DIN (GPIO2) - 28 LEDs
- G     -> GND
- 5V    -> 5V Spannungsversorgung

Untere Reihe:
- RST   -> Reset-Taster
- A0    -> nicht verwendet (ADC)
- D0    -> nicht verwendet (GPIO16)
- D5    -> nicht verwendet (GPIO14)
- D6    -> Button 1 (GPIO12)
- D7    -> Button 2 (GPIO13)
- D8    -> nicht verwendet (GPIO15)
- 3V3   -> 3.3V Spannungsversorgung
```

### WS2812B LED-Strips
```
Strip 1 (28 LEDs):
- VCC  -> 5V (mit 1000µF Kondensator parallel)
- GND  -> GND
- DIN  -> GPIO 2 (D4, ESP8266)

Strip 2 (10 LEDs):
- VCC  -> 5V (mit 1000µF Kondensator parallel)
- GND  -> GND
- DIN  -> GPIO 0 (D3, ESP8266)
```

### DFPlayer Mini
```
VCC  -> 5V
GND  -> GND
RX   -> GPIO 4 (ESP8266)
TX   -> GPIO 5 (ESP8266)
SPK1 -> Lautsprecher +
SPK2 -> Lautsprecher -
```

### Drucktaster
```
Button 1 (AN/AUS + Effekte):
- Pin 1 -> GPIO 12 (D6, ESP8266) + 10kΩ Pull-up zu 3.3V
- Pin 2 -> GND

Button 2 (Musik + Zusätzlich):
- Pin 1 -> GPIO 13 (D7, ESP8266) + 10kΩ Pull-up zu 3.3V
- Pin 2 -> GND
```

## 🚀 Installation & Setup

### 1. WLED-Firmware kompilieren
```bash
cd wled_source
python -m platformio run -e Interaktive_Lampe
```

### 2. Firmware flashen
```bash
python -m platformio run -e Interaktive_Lampe --target upload --upload-port COM3
```

### 3. WLED WebUI konfigurieren
- **IP-Adresse** des ESP8266 aufrufen
- **Usermods aktivieren**: ZDigBw_Buttons, ZDigBw_DFPlayer
- **Musik-Preset konfigurieren** (Standard: Preset 2)
- **LED-Strips konfigurieren**: 2 Strips (28 + 10 LEDs)

### 4. MP3-Dateien vorbereiten
- **Micro-SD Karte** formatieren (FAT32)
- **MP3-Dateien** in Ordner `01/` kopieren
- **Dateinamen**: `001.mp3`, `002.mp3`, etc.

## 🎵 Besondere Features

### Musik-Preset-Integration
- **Automatisches Preset-Laden** beim Musikstart
- **Konfigurierbar** in WLED WebUI (1-10)
- **Standard**: Preset 2 wird beim Musikstart geladen

### Intelligente Preset-Verwaltung
- **Generisches Preset-Cycling** zwischen verfügbaren Presets
- **Keine hartcodierten Limits** - funktioniert mit beliebig vielen Presets
- **Fehlerfreie Behandlung** nicht existierender Presets

### Zwei separate LED-Strips
- **Individuelle Konfiguration** pro Strip
- **Unterschiedliche Effekte** möglich
- **Separate Helligkeitssteuerung**

## 📁 Projektstruktur
```
Interaktive Lampe/
├── wled_source/                 # WLED-Firmware mit Usermods
│   ├── usermods/
│   │   ├── ZDigBw_Buttons/      # Button-Steuerung
│   │   └── ZDigBw_DFPlayer/     # MP3-Player
│   ├── platformio_override.ini # Build-Konfiguration
│   └── wled00/my_config.h      # LED-Konfiguration
├── verkabelung.md              # Detaillierte Verkabelung
├── Breadboard_Setup.md         # Breadboard-Setup
└── README.md                   # Diese Datei
```

## 🔧 Konfiguration

### LED-Konfiguration (`wled00/my_config.h`)
```cpp
#define MAX_LEDS 38          // Strip 1: 28 LEDs + Strip 2: 10 LEDs
#define DATA_PINS 2          // Zwei separate Strips
#define LEDPIN 2            // Strip 1: GPIO2 (D4)
#define LEDPIN2 0           // Strip 2: GPIO0 (D3)
```

### Button-Konfiguration
- **Button 1**: GPIO12 (D6) - AN/AUS + Presets
- **Button 2**: GPIO13 (D7) - Musik + Zusätzlich
- **Pull-up-Widerstände**: 10kΩ zu 3.3V

### DFPlayer-Konfiguration
- **RX Pin**: GPIO4 (D2)
- **TX Pin**: GPIO5 (D1)
- **Baudrate**: 9600
- **Musik-Preset**: Konfigurierbar (Standard: 2)

## 🛠️ Troubleshooting

### LEDs leuchten nicht
- **5V-Versorgung** prüfen
- **Kondensator** parallel zu 5V
- **DIN-Verbindungen** prüfen

### DFPlayer reagiert nicht
- **RX/TX-Verbindungen** prüfen
- **Micro-SD Karte** formatieren
- **MP3-Dateien** in korrektem Ordner

### Buttons funktionieren nicht
- **Pull-up-Widerstände** prüfen (10kΩ)
- **GPIO-Pins** korrekt verbunden
- **GND-Verbindungen** prüfen

## 📝 Lizenz
Dieses Projekt ist für private Nutzung bestimmt.

## 👨‍💻 Autor
Entwickelt für das Interaktive Lampe Projekt.
