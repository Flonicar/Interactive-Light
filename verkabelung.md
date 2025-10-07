# Verkabelungsplan - ZDigBw Lampe

## Übersicht
Dieser Plan zeigt die Verkabelung für deine intelligente Lampe mit ESP8266, WS2812B LEDs, DFPlayer Mini und zwei Drucktastern.

## Komponenten
- **ESP8266 ESP-12F** - Hauptcontroller
- **WS2812B LED-Strips** - 2 separate Strips (28 + 10 LEDs)
- **DFPlayer Mini** - MP3-Player
- **2x 12mm Edelstahl-Drucktaster** - Steuerung
- **5V/3.3V Spannungsversorgung**
- **Micro-SD Karte** - für MP3-Dateien

## Verkabelung

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
VCC  -> 5V (mit 1000µF Kondensator parallel)
GND  -> GND
DIN  -> GPIO 2 (D4, ESP8266)

Strip 2 (10 LEDs):
VCC  -> 5V (mit 1000µF Kondensator parallel)
GND  -> GND
DIN  -> GPIO 0 (D3, ESP8266)
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
- Pin 1 -> GPIO 12 (D6 auf ESP8266MOD)
- Pin 2 -> GND

Button 2 (Musik + Zusätzlich):
- Pin 1 -> GPIO 13 (D7 auf ESP8266MOD)
- Pin 2 -> GND
```

## Spannungsversorgung
- **5V/3A Netzteil** für LEDs und DFPlayer
- **3.3V Spannungsregler** (AMS1117-3.3V) für ESP8266
- **1000µF Kondensator** parallel zu 5V für LED-Strip

## Sicherheitshinweise
- **Polarität beachten** - falsche Verkabelung kann Komponenten zerstören
- **Spannungspegel** - ESP8266 läuft mit 3.3V, LEDs mit 5V
- **Stromverbrauch** - 200 LEDs können bis zu 12A bei 5V ziehen
- **Kabelquerschnitt** - mindestens 0.75mm² für 5V-Versorgung

## Troubleshooting
- **LEDs leuchten nicht**: Prüfe 5V-Versorgung und DIN-Verbindung
- **DFPlayer reagiert nicht**: Prüfe RX/TX-Verbindungen
- **Buttons funktionieren nicht**: Prüfe Pull-up-Widerstände
- **ESP8266 startet nicht**: Prüfe 3.3V-Versorgung und Boot-Pins
