# Breadboard Test - Schritt für Schritt

## Phase 1: WLED installieren und testen

### 1.1 ESP8266 mit WLED flashen
1. **WLED Web Installer** öffnen: https://install.wled.me/
2. **ESP8266** auswählen
3. **Neueste stabile Version** wählen
4. **ESP8266** per USB verbinden
5. **Port** auswählen und **Install** klicken

### 1.2 WLED konfigurieren
1. **WLAN** suchen: "WLED-AP"
2. **Passwort**: "wled1234"
3. **Browser**: http://4.3.2.1
4. **Config → LED Preferences**:
   - LED Pin: 2
   - LED Count: 20 (für Test)
   - LED Type: WS2812B
   - Color Order: GRB
5. **Save** klicken

### 1.3 LED-Strip testen
1. **WS2812B** an GPIO2 anschließen
2. **5V** und **GND** verbinden
3. **WebUI** öffnen und **Effekte** testen
4. **Farben** ändern
5. **Helligkeit** regeln

## Phase 2: DFPlayer Mini testen

### 2.1 DFPlayer anschließen
```
DFPlayer Mini:
- VCC -> 5V
- GND -> GND
- RX  -> GPIO4 (ESP8266)
- TX  -> GPIO5 (ESP8266)
- SPK1 -> Lautsprecher +
- SPK2 -> Lautsprecher -
```

### 2.2 MP3-Dateien vorbereiten
1. **Micro-SD Karte** formatieren (FAT32)
2. **MP3-Dateien** kopieren (z.B. 001.mp3, 002.mp3, ...)
3. **SD-Karte** in DFPlayer einstecken

### 2.3 DFPlayer testen
1. **WLED-Erweiterungen** installieren
2. **WebUI** öffnen
3. **DFPlayer-Funktionen** testen

## Phase 3: Buttons testen

### 3.1 Buttons anschließen
```
Button 1 (AN/AUS + Effekte):
- Pin 1 -> GPIO12 (ESP8266)
- Pin 2 -> GND

Button 2 (Musik + Zusätzlich):
- Pin 1 -> GPIO13 (ESP8266)
- Pin 2 -> GND
```

### 3.2 Button-Funktionen testen
- **Button 1 kurz**: AN/AUS
- **Button 1 lang**: Effekt wechseln
- **Button 1 doppelt**: Helligkeit ändern
- **Button 2 kurz**: Musik Start/Stop
- **Button 2 lang**: Nächster Song
- **Button 2 doppelt**: Lautstärke erhöhen

## Phase 4: Alles zusammen testen

### 4.1 Vollständiges Setup
- **Alle Komponenten** anschließen
- **Spannungsversorgung** prüfen
- **Verbindungen** kontrollieren

### 4.2 Funktionstest
- **WLED WebUI** funktioniert
- **LED-Effekte** laufen
- **DFPlayer** spielt Musik
- **Buttons** steuern alles
- **Synchronisation** funktioniert

## Troubleshooting

### LEDs leuchten nicht
- **5V-Versorgung** prüfen
- **DIN-Verbindung** prüfen
- **LED-Count** in WLED anpassen

### DFPlayer reagiert nicht
- **RX/TX-Verbindungen** prüfen
- **MP3-Dateien** auf SD-Karte
- **Lautsprecher** angeschlossen

### Buttons funktionieren nicht
- **Pull-up-Widerstände** prüfen
- **GPIO-Pins** korrekt
- **GND-Verbindung** prüfen

### ESP8266 startet nicht
- **3.3V-Versorgung** prüfen
- **Boot-Pins** korrekt
- **USB-Kabel** funktioniert
