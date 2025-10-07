# Breadboard Setup - ZDigBw Lampe

## Komponenten für das Breadboard
- **ESP8266 ESP-12F** (auf Breadboard-Adapter)
- **WS2812B LED-Strips** (2 separate Strips: 28 + 10 LEDs)
- **DFPlayer Mini** (auf Breadboard)
- **2x 12mm Drucktaster** (mit Kabeln)
- **Breadboard** (830 Punkte)
- **Jumper-Kabel**
- **Widerstände**: 2x 10kΩ (Pull-up für Buttons)
- **Kondensator**: 1000µF (für LED-Strip)
- **Lautsprecher** (klein, 8Ω)

## Verkabelung auf dem Breadboard

### ESP8266 Position
```
ESP8266 auf Breadboard-Adapter:
- 3.3V -> Breadboard Plus-Leiste
- GND  -> Breadboard Minus-Leiste
- GPIO0 -> LED-Strip 2 DIN (10 LEDs)
- GPIO2 -> LED-Strip 1 DIN (28 LEDs)
- GPIO4 -> DFPlayer TX
- GPIO5 -> DFPlayer RX
- GPIO12 -> Button 1 (D6, mit 10kΩ Pull-up)
- GPIO13 -> Button 2 (D7, mit 10kΩ Pull-up)
```

### DFPlayer Mini Position
```
DFPlayer Mini:
- VCC -> 5V (externe Versorgung)
- GND -> GND
- RX  -> GPIO4 (ESP8266)
- TX  -> GPIO5 (ESP8266)
- SPK1 -> Lautsprecher +
- SPK2 -> Lautsprecher -
```

### LED-Strips Position
```
WS2812B Strip 1 (28 LEDs):
- VCC -> 5V (mit 1000µF Kondensator)
- GND -> GND
- DIN -> GPIO2 (ESP8266)

WS2812B Strip 2 (10 LEDs):
- VCC -> 5V (mit 1000µF Kondensator)
- GND -> GND
- DIN -> GPIO0 (ESP8266)
```

### Button-Position
```
Button 1 (AN/AUS + Effekte):
- Pin 1 -> GPIO12 (ESP8266)
- Pin 2 -> GND

Button 2 (Musik + Zusätzlich):
- Pin 1 -> GPIO13 (ESP8266)
- Pin 2 -> GND
```

## Spannungsversorgung
- **5V/2A Netzteil** für LEDs und DFPlayer
- **3.3V Spannungsregler** (AMS1117-3.3V) für ESP8266
- **Breadboard Plus-Leiste**: 3.3V
- **Breadboard Minus-Leiste**: GND

## Test-Reihenfolge
1. **ESP8266** mit WLED flashen
2. **LED-Strip** anschließen und testen
3. **DFPlayer Mini** anschließen und testen
4. **Buttons** anschließen und testen
5. **Alles zusammen** testen

## Sicherheitshinweise
- **Polarität beachten** - ESP8266 läuft mit 3.3V!
- **Stromverbrauch** - kleine LED-Sektion für Test
- **Kabelverbindungen** - alle Verbindungen prüfen
- **Spannungspegel** - 5V und 3.3V nicht verwechseln
