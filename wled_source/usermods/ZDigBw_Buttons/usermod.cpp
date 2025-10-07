#include "wled.h"

/*
 * ZDigBw Lampe - Button Usermod mit verbessertem Debouncing
 * Erweitert WLED um zwei programmierbare Drucktaster
 * 
 * Button 1 (D6/GPIO12): AN/AUS + Presets
 * Button 2 (D7/GPIO13): Musik + Zusätzlich
 * 
 * Verbesserte Debouncing-Techniken basierend auf professionellen Bibliotheken
 */

// Globale Variablen für DFPlayer-Kommunikation
extern bool dfPlayerPlayPause;
extern bool dfPlayerNextTrack;
extern bool dfPlayerChangeVolume;

class ZDigBwButtonsUsermod : public Usermod {
private:
    bool enabled = true;
    bool initDone = false;
    
    // Button-Pins
    int8_t button1Pin = 12;  // D6
    int8_t button2Pin = 13;  // D7
    
    // Professionelles Debouncing (basierend auf button-debounce Bibliothek)
    struct ButtonState {
        bool currentState = false;      // Aktueller Hardware-Zustand
        bool stableState = false;       // Stabiler, debounced Zustand
        bool lastStableState = false;   // Vorheriger stabiler Zustand
        unsigned long lastChangeTime = 0; // Zeit der letzten Zustandsänderung
        unsigned long lastPressTime = 0;  // Zeit des letzten Drucks
        unsigned long lastReleaseTime = 0; // Zeit des letzten Loslassens
        int clickCount = 0;             // Anzahl der Klicks
        bool longPressTriggered = false; // Lang-Druck bereits ausgelöst
    };
    
    ButtonState button1;
    ButtonState button2;
    
    // Timing-Konstanten (optimiert für Zuverlässigkeit)
    static const unsigned long DEBOUNCE_TIME = 50;        // ms - Zeit für stabilen Zustand
    static const unsigned long LONG_PRESS_TIME = 800;     // ms - Zeit für Lang-Druck
    static const unsigned long DOUBLE_CLICK_TIME = 400;   // ms - Zeit für Doppel-Klick
    static const unsigned long POLL_INTERVAL = 20;        // ms - Polling-Intervall

public:
    void setup() override {
        if (button1Pin >= 0) {
            pinMode(button1Pin, INPUT_PULLUP);
        }
        
        if (button2Pin >= 0) {
            pinMode(button2Pin, INPUT_PULLUP);
        }
        
        initDone = true;
        Serial.println(F("ZDigBw Buttons Usermod initialisiert (verbessertes Debouncing)"));
    }

    void loop() override {
        if (!enabled || !initDone) return;
        
        unsigned long currentTime = millis();
        
        // Button 1 verarbeiten
        if (button1Pin >= 0) {
            processButton(button1, digitalRead(button1Pin) == LOW, currentTime, 1);
        }
        
        // Button 2 verarbeiten
        if (button2Pin >= 0) {
            processButton(button2, digitalRead(button2Pin) == LOW, currentTime, 2);
        }
    }

    // Professionelle Button-Verarbeitung mit verbessertem Debouncing
    void processButton(ButtonState& btn, bool hardwareState, unsigned long currentTime, int buttonId) {
        // Hardware-Zustand aktualisieren
        btn.currentState = hardwareState;
        
        // Debouncing: Prüfe ob Zustand stabil ist
        if (btn.currentState != btn.stableState) {
            // Zustand hat sich geändert - starte Debounce-Timer
            if (btn.lastChangeTime == 0) {
                btn.lastChangeTime = currentTime;
            }
            
            // Prüfe ob Debounce-Zeit abgelaufen ist
            if (currentTime - btn.lastChangeTime >= DEBOUNCE_TIME) {
                // Zustand ist stabil - aktualisiere stabilen Zustand
                btn.stableState = btn.currentState;
                btn.lastChangeTime = 0;
                
                // Verarbeite Zustandsänderung
                if (btn.stableState && !btn.lastStableState) {
                    // Button wurde gedrückt
                    btn.lastPressTime = currentTime;
                    btn.longPressTriggered = false;
                    
                    // Klick-Zählung für Doppel-Klick
                    if (currentTime - btn.lastReleaseTime < DOUBLE_CLICK_TIME) {
                        btn.clickCount++;
                    } else {
                        btn.clickCount = 1;
                    }
                    
                } else if (!btn.stableState && btn.lastStableState) {
                    // Button wurde losgelassen
                    btn.lastReleaseTime = currentTime;
                    
                    // Verarbeite Klick-Aktionen
                    if (btn.clickCount == 1 && !btn.longPressTriggered) {
                        // Einfacher Klick
                        if (buttonId == 1) {
                            togglePower();
                        } else {
                            toggleMusic();
                        }
                    }
                    
                    btn.clickCount = 0;
                }
                
                btn.lastStableState = btn.stableState;
            }
        } else {
            // Zustand ist stabil - reset Debounce-Timer
            btn.lastChangeTime = 0;
        }
        
        // Lang-Druck-Erkennung
        if (btn.stableState && !btn.longPressTriggered && 
            currentTime - btn.lastPressTime >= LONG_PRESS_TIME) {
            
            btn.longPressTriggered = true;
            
            if (buttonId == 1) {
                nextPreset();
            } else {
                nextTrack();
            }
        }
        
        // Doppel-Klick-Erkennung
        if (btn.clickCount >= 2 && currentTime - btn.lastReleaseTime >= DOUBLE_CLICK_TIME) {
            if (buttonId == 1) {
                changeBrightness();
            } else {
                changeVolume();
            }
            btn.clickCount = 0;
        }
    }

    void togglePower() {
        if (bri) {
            briLast = bri;
            bri = 0;
        } else {
            bri = briLast;
        }
        stateUpdated(CALL_MODE_BUTTON);
        Serial.println(F("Power umgeschaltet"));
    }

    void nextPreset() {
        // Intelligente Lösung: Wechsle nur zwischen tatsächlich vorhandenen Presets
        static int currentPreset = 1; // Beginnt bei 1
        
        // Konfigurierbare Anzahl von Presets (ändern Sie die 3 auf Ihre Anzahl)
        const int maxPresets = 3; // HIER IHRE ANZAHL EINTRAGEN!
        currentPreset = (currentPreset % maxPresets) + 1; // 1-3 Presets
        
        // Verwende WLED's eingebaute Preset-Funktion
        applyPreset(currentPreset);
        
        Serial.print(F("Preset ")); Serial.println(currentPreset);
        stateUpdated(CALL_MODE_BUTTON);
    }

    void changeBrightness() {
        if (bri > 128) {
            bri = 63;   // Mittel
        } else if (bri > 63) {
            bri = 15;   // Niedrig
        } else {
            bri = 255;  // Hoch
        }
        stateUpdated(CALL_MODE_BUTTON);
        Serial.print(F("Helligkeit: ")); Serial.println(bri);
    }

    void toggleMusic() {
        dfPlayerPlayPause = true;
        Serial.println(F("Musik Start/Stop"));
    }

    void nextTrack() {
        dfPlayerNextTrack = true;
        Serial.println(F("Nächster Track"));
    }

    void changeVolume() {
        dfPlayerChangeVolume = true;
        Serial.println(F("Lautstärke ändern"));
    }

    void addToJsonInfo(JsonObject& root) override {
        JsonObject user = root["u"];
        if (user.isNull()) user = root.createNestedObject("u");
        
        JsonArray info = user.createNestedArray(F("ZDigBw Buttons"));
        info.add(F("Button 1: Power + Presets"));
        info.add(F("Button 2: Musik + Zusätzlich"));
        info.add(F("Verbessertes Debouncing"));
    }

    void addToJsonState(JsonObject& root) override {
        JsonObject user = root["u"];
        if (user.isNull()) user = root.createNestedObject("u");
        
        user["button1Pin"] = button1Pin;
        user["button2Pin"] = button2Pin;
        user["debounceTime"] = DEBOUNCE_TIME;
        user["longPressTime"] = LONG_PRESS_TIME;
    }

    void readFromJsonState(JsonObject& root) override {
        JsonObject user = root["u"];
        if (user.isNull()) return;
        
        if (user["button1Pin"].is<int>()) button1Pin = user["button1Pin"];
        if (user["button2Pin"].is<int>()) button2Pin = user["button2Pin"];
    }

    void addToConfig(JsonObject& root) override {
        JsonObject top = root["top"];
        if (top.isNull()) top = root.createNestedObject("top");
        
        JsonArray pins = top.createNestedArray(F("Button Pins"));
        pins.add(button1Pin);
        pins.add(button2Pin);
    }

    void readFromConfig(JsonObject& root) override {
        JsonObject top = root["top"];
        if (top.isNull()) return;
        
        if (top["Button Pins"].is<JsonArray>()) {
            JsonArray pins = top["Button Pins"];
            if (pins.size() >= 2) {
                button1Pin = pins[0];
                button2Pin = pins[1];
            }
        }
    }

    void appendConfigData() override {
        oappend(SET_F("addInfo('ZDigBw Buttons - Verbessertes Debouncing');"));
    }

    uint16_t getId() override {
        return USERMOD_ID_BUTTONS;
    }
};

// Usermod-Instanz erstellen
ZDigBwButtonsUsermod* ZDigBwButtonsUsermodInstance = nullptr;

void registerZDigBwButtonsUsermod() {
    if (ZDigBwButtonsUsermodInstance == nullptr) {
        ZDigBwButtonsUsermodInstance = new ZDigBwButtonsUsermod();
        usermods.add(ZDigBwButtonsUsermodInstance);
    }
}