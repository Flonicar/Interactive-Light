# WLED Installation - Schritt für Schritt

## 1. WLED herunterladen und installieren

### Option A: Arduino IDE
1. **Arduino IDE** öffnen
2. **Datei → Voreinstellungen**
3. **Zusätzliche Boardverwalter-URLs** hinzufügen:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
4. **Werkzeuge → Board → Boardverwalter**
5. **"esp8266"** suchen und **"ESP8266 by ESP8266 Community"** installieren
6. **Werkzeuge → Board → ESP8266 Boards → Generic ESP8266 Module** auswählen

### Option B: PlatformIO (empfohlen)
1. **VS Code** mit **PlatformIO Extension** installieren
2. **PlatformIO Home** öffnen
3. **New Project** erstellen
4. **Board**: esp12e auswählen
5. **Framework**: arduino

## 2. WLED flashen

### Methode 1: WLED Web Installer (einfachste)
1. Browser öffnen: https://install.wled.me/
2. **ESP8266** auswählen
3. **WLED Version** wählen (empfohlen: neueste stabile Version)
4. **Flash** klicken
5. **ESP8266** per USB verbinden
6. **Port** auswählen und **Install** klicken

### Methode 2: Manuell flashen
1. **WLED GitHub** öffnen: https://github.com/Aircoookie/WLED
2. **Code → Download ZIP**
3. **Arduino IDE** öffnen
4. **Datei → Öffnen** → WLED.ino
5. **Werkzeuge → Board → ESP8266 Boards → Generic ESP8266 Module**
6. **Werkzeuge → Port** → deinen COM-Port auswählen
7. **Hochladen** klicken

## 3. WLED konfigurieren

### Erste Inbetriebnahme
1. **ESP8266** mit Strom versorgen
2. **WLAN** suchen: "WLED-AP" oder "WLED-XXXXXX"
3. **Passwort**: "wled1234"
4. **Browser** öffnen: http://4.3.2.1
5. **WLED WebUI** sollte erscheinen

### WLAN konfigurieren
1. **Config → WiFi Settings**
2. **SSID** eingeben (dein WLAN)
3. **Passwort** eingeben
4. **Save & Connect** klicken
5. **ESP8266** neu starten

### LED konfigurieren
1. **Config → LED Preferences**
2. **LED Pin**: 2 (für dein Setup)
3. **LED Count**: 200 (oder deine Anzahl)
4. **LED Type**: WS2812B
5. **Color Order**: GRB
6. **Save** klicken

## 4. Testen
- **WebUI** öffnen: http://[ESP8266-IP]
- **Effekte** testen
- **Farben** ändern
- **Helligkeit** regeln

## Nächster Schritt
Nach erfolgreicher WLED-Installation können wir deine Hardware hinzufügen!
