// ZDigBw Lampe - Hauptprogramm
// ESP8266 mit WLED, DFPlayer Mini und Button-Steuerung

#include "config.h"
#include "button_handler.h"
#include "dfplayer_handler.h"
#include "wled_integration.h"

// Globale Objekte
ButtonHandler buttons(BUTTON_1_PIN, BUTTON_2_PIN);
DFPlayerHandler dfplayer(DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);
WLEDIntegration leds;

// Systemzustand
bool systemOn = false;
int currentBrightness = WLED_BRIGHTNESS;
int brightnessLevels[] = {32, 64, 128, 192, 255};
int currentBrightnessLevel = 2; // Index für brightnessLevels

void setup() {
    Serial.begin(115200);
    Serial.println("ZDigBw Lampe gestartet");
    
    // Initialisierung
    buttons.begin();
    dfplayer.begin();
    leds.begin();
    
    // Startzustand
    leds.setAllOn(false);
    leds.setAllBrightness(currentBrightness);
    
    Serial.println("System bereit");
    Serial.println("Button 1: AN/AUS + Effekte");
    Serial.println("Button 2: Musik + Zusätzlich");
}

void loop() {
    // Button-Status aktualisieren
    buttons.update();
    
    // DFPlayer aktualisieren
    dfplayer.update();
    
    // LED-Effekte aktualisieren
    leds.update();
    
    // Button 1 - AN/AUS + Effekte
    if (buttons.isButton1ShortPress()) {
        systemOn = !systemOn;
        leds.setAllOn(systemOn);
        Serial.println(systemOn ? "System AN" : "System AUS");
    }
    
    if (buttons.isButton1LongPress()) {
        if (systemOn) {
            leds.nextSegment1Effect();
            leds.nextSegment2Effect();
            Serial.println("Effekte gewechselt");
        }
    }
    
    if (buttons.isButton1DoubleClick()) {
        if (systemOn) {
            currentBrightnessLevel = (currentBrightnessLevel + 1) % 5;
            currentBrightness = brightnessLevels[currentBrightnessLevel];
            leds.setAllBrightness(currentBrightness);
            Serial.print("Helligkeit: ");
            Serial.println(currentBrightness);
        }
    }
    
    // Button 2 - Musik + Zusätzlich
    if (buttons.isButton2ShortPress()) {
        if (dfplayer.getIsPlaying()) {
            dfplayer.pause();
            Serial.println("Musik pausiert");
        } else {
            dfplayer.play();
            Serial.println("Musik gestartet");
        }
    }
    
    if (buttons.isButton2LongPress()) {
        dfplayer.nextTrack();
        Serial.println("Nächster Track");
    }
    
    if (buttons.isButton2DoubleClick()) {
        dfplayer.volumeUp();
        Serial.print("Lautstärke erhöht: ");
        Serial.println(dfplayer.getCurrentTrack());
    }
    
    // Kurze Pause für Stabilität
    delay(MAIN_LOOP_DELAY);
}

// Zusätzliche Funktionen
void handleSystemStartup() {
    // Startup-Sequenz
    leds.setAllColor(CRGB::Green);
    leds.setAllOn(true);
    delay(500);
    leds.setAllOn(false);
    delay(200);
    leds.setAllColor(CRGB::Blue);
    leds.setAllOn(true);
    delay(500);
    leds.setAllOn(false);
    delay(200);
    leds.setAllColor(CRGB::Red);
    leds.setAllOn(true);
    delay(500);
    leds.setAllOn(false);
}

void handleSystemShutdown() {
    // Shutdown-Sequenz
    for (int i = 255; i >= 0; i -= 5) {
        leds.setAllBrightness(i);
        leds.update();
        delay(20);
    }
    leds.setAllOn(false);
}
