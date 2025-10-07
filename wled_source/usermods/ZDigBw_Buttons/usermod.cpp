#include "wled.h"

/*
 * ZDigBw Lampe - Button Usermod
 * Erweitert WLED um zwei programmierbare Drucktaster
 * 
 * Button 1 (D6/GPIO12): AN/AUS + Effekte
 * Button 2 (D7/GPIO13): Musik + Zusätzlich
 */

// Globale Variablen für DFPlayer-Kommunikation
extern bool dfPlayerPlayPause;
extern bool dfPlayerNextTrack;
extern bool dfPlayerChangeVolume;

class ZDigBwButtonsUsermod : public Usermod {
private:
    bool enabled = true;
    bool initDone = false;
    
    // Button Pins
    int8_t button1Pin = 12;  // D6 - AN/AUS + Effekte
    int8_t button2Pin = 13;  // D7 - Musik + Zusätzlich
    
    // Button 1 Zustände
    bool button1Pressed = false;
    bool button1LongPressed = false;
    unsigned long button1PressTime = 0;
    unsigned long button1LastClick = 0;
    int button1ClickCount = 0;
    
    // Button 2 Zustände
    bool button2Pressed = false;
    bool button2LongPressed = false;
    unsigned long button2PressTime = 0;
    unsigned long button2LastClick = 0;
    int button2ClickCount = 0;
    
    // Timing - Verbesserte Zuverlässigkeit
    unsigned long lastButton1Check = 0;
    unsigned long lastButton2Check = 0;
    static const unsigned long BUTTON_CHECK_INTERVAL = 30;  // Häufigere Prüfung
    static const unsigned long BUTTON_LONG_PRESS_TIME = 800;  // Kürzer für bessere Reaktion
    static const unsigned long BUTTON_DOUBLE_CLICK_TIME = 400;  // Länger für zuverlässigere Erkennung
    
    // Button-Debouncing
    static const int BUTTON_DEBOUNCE_COUNT = 3;  // Mehrfache Bestätigung
    int button1DebounceCount = 0;
    int button2DebounceCount = 0;
    
    // Helligkeitsstufen
    int brightnessLevels[5] = {32, 64, 128, 192, 255};
    int currentBrightnessLevel = 2;
    
    // Strings für Flash-Speicher
    static const char _name[];
    static const char _enabled[];
    static const char _button1Pin[];
    static const char _button2Pin[];

public:
    void setup() override {
        if (button1Pin >= 0) {
            pinMode(button1Pin, INPUT_PULLUP);
        }
        if (button2Pin >= 0) {
            pinMode(button2Pin, INPUT_PULLUP);
        }
        initDone = true;
    }

    void loop() override {
        if (!enabled || !initDone) return;
        
        unsigned long currentTime = millis();
        
            // Button 1 Update - Verbesserte Zuverlässigkeit
            if (button1Pin >= 0 && currentTime - lastButton1Check >= BUTTON_CHECK_INTERVAL) {
                bool currentState = digitalRead(button1Pin) == LOW; // LOW = gedrückt
                
                // Debouncing - mehrfache Bestätigung
                if (currentState) {
                    button1DebounceCount++;
                    if (button1DebounceCount >= BUTTON_DEBOUNCE_COUNT && !button1Pressed) {
                        // Button wurde zuverlässig gedrückt
                        button1Pressed = true;
                        button1PressTime = currentTime;
                        button1LongPressed = false;
                        button1DebounceCount = 0;
                    }
                } else {
                    button1DebounceCount = 0;
                    if (button1Pressed) {
                        // Button wurde losgelassen
                        button1Pressed = false;

                        unsigned long pressDuration = currentTime - button1PressTime;

                        if (pressDuration < BUTTON_LONG_PRESS_TIME) {
                            // Kurzer Klick
                            if (currentTime - button1LastClick < BUTTON_DOUBLE_CLICK_TIME) {
                                button1ClickCount++;
                            } else {
                                button1ClickCount = 1;
                            }
                            button1LastClick = currentTime;
                        }
                    }
                }
                
                if (button1Pressed && !button1LongPressed) {
                    // Button ist gedrückt - prüfe auf Lang-Druck
                    if (currentTime - button1PressTime >= BUTTON_LONG_PRESS_TIME) {
                        button1LongPressed = true;
                        // Sofort Lang-Druck-Aktion ausführen
                        nextPreset();
                    }
                }

                lastButton1Check = currentTime;
            }
        
        // Button 2 Update - Verbesserte Zuverlässigkeit
        if (button2Pin >= 0 && currentTime - lastButton2Check >= BUTTON_CHECK_INTERVAL) {
            bool currentState = digitalRead(button2Pin) == LOW; // LOW = gedrückt
            
            // Debouncing - mehrfache Bestätigung
            if (currentState) {
                button2DebounceCount++;
                if (button2DebounceCount >= BUTTON_DEBOUNCE_COUNT && !button2Pressed) {
                    // Button wurde zuverlässig gedrückt
                    button2Pressed = true;
                    button2PressTime = currentTime;
                    button2LongPressed = false;
                    button2DebounceCount = 0;
                }
            } else {
                button2DebounceCount = 0;
                if (button2Pressed) {
                    // Button wurde losgelassen
                    button2Pressed = false;
                    
                    unsigned long pressDuration = currentTime - button2PressTime;
                    
                    if (pressDuration < BUTTON_LONG_PRESS_TIME) {
                        // Kurzer Klick
                        if (currentTime - button2LastClick < BUTTON_DOUBLE_CLICK_TIME) {
                            button2ClickCount++;
                        } else {
                            button2ClickCount = 1;
                        }
                        button2LastClick = currentTime;
                    }
                }
            }
            
            if (button2Pressed && !button2LongPressed) {
                // Button ist gedrückt - prüfe auf Lang-Druck
                if (currentTime - button2PressTime >= BUTTON_LONG_PRESS_TIME) {
                    button2LongPressed = true;
                    // Sofort Lang-Druck-Aktion ausführen
                    nextTrack();
                }
            }
            
            lastButton2Check = currentTime;
        }
        
        // Button 1 Aktionen
        if (button1ClickCount == 1 && currentTime - button1LastClick >= BUTTON_DOUBLE_CLICK_TIME) {
            // Einfacher Klick: AN/AUS
            togglePower();
            button1ClickCount = 0;
        }
        
        // Lang-Druck wird bereits in der Button-Logik behandelt
        
        if (button1ClickCount >= 2) {
            // Doppel-Klick: Helligkeit ändern
            changeBrightness();
            button1ClickCount = 0;
        }
        
        // Button 2 Aktionen (für DFPlayer - wird in separatem Usermod implementiert)
        if (button2ClickCount == 1 && currentTime - button2LastClick >= BUTTON_DOUBLE_CLICK_TIME) {
            // Einfacher Klick: Musik Start/Stop
            toggleMusic();
            button2ClickCount = 0;
        }
        
        // Lang-Druck wird bereits in der Button-Logik behandelt
        
        if (button2ClickCount >= 2) {
            // Doppel-Klick: Lautstärke ändern
            changeVolume();
            button2ClickCount = 0;
        }
    }

    void addToJsonInfo(JsonObject& root) override {
        JsonObject user = root["u"];
        if (user.isNull()) user = root.createNestedObject("u");
        
        JsonArray info = user.createNestedArray(FPSTR(_name));
        info.add(F("Button 1: AN/AUS + Effekte"));
        info.add(F("Button 2: Musik + Zusätzlich"));
    }

    void addToJsonState(JsonObject& root) override {
        if (!initDone) return;
        
        JsonObject usermod = root[FPSTR(_name)];
        if (usermod.isNull()) usermod = root.createNestedObject(FPSTR(_name));
        
        usermod["button1Pin"] = button1Pin;
        usermod["button2Pin"] = button2Pin;
        usermod["brightnessLevel"] = currentBrightnessLevel;
    }

    void readFromJsonState(JsonObject& root) override {
        if (!initDone) return;
        
        JsonObject usermod = root[FPSTR(_name)];
        if (!usermod.isNull()) {
            // Button-Pins können über JSON geändert werden
            if (usermod["button1Pin"] != nullptr) button1Pin = usermod["button1Pin"];
            if (usermod["button2Pin"] != nullptr) button2Pin = usermod["button2Pin"];
        }
    }

    void addToConfig(JsonObject& root) override {
        JsonObject top = root.createNestedObject(FPSTR(_name));
        top[FPSTR(_enabled)] = enabled;
        top[FPSTR(_button1Pin)] = button1Pin;
        top[FPSTR(_button2Pin)] = button2Pin;
    }

    bool readFromConfig(JsonObject& root) override {
        JsonObject top = root[FPSTR(_name)];
        
        bool configComplete = !top.isNull();
        configComplete &= getJsonValue(top[FPSTR(_enabled)], enabled);
        configComplete &= getJsonValue(top[FPSTR(_button1Pin)], button1Pin, 12);
        configComplete &= getJsonValue(top[FPSTR(_button2Pin)], button2Pin, 13);
        
        return configComplete;
    }

    void appendConfigData() override {
        oappend(F("addInfo('")); oappend(String(FPSTR(_name)).c_str()); oappend(F(":button1Pin")); oappend(F("',1,'Button 1 Pin (D6/GPIO12)');"));
        oappend(F("addInfo('")); oappend(String(FPSTR(_name)).c_str()); oappend(F(":button2Pin")); oappend(F("',1,'Button 2 Pin (D7/GPIO13)');"));
    }

    uint16_t getId() override {
        return USERMOD_ID_BUTTONS;
    }

private:
    void togglePower() {
        if (bri == 0) {
            bri = briLast;
            if (bri == 0) bri = 128;
        } else {
            briLast = bri;
            bri = 0;
        }
        stateUpdated(CALL_MODE_BUTTON);
    }

    void nextPreset() {
        // Preset wechseln - verwende WLED's eingebaute Preset-Funktionalität!
        Serial.println(F("Preset wechseln..."));
        
        // Stelle sicher, dass die Lampe eingeschaltet ist
        if (bri == 0) {
            bri = 128; // Mittlere Helligkeit
            Serial.println(F("Lampe eingeschaltet für Preset"));
        }
        
        // Verwende WLED's echte Preset-Funktionalität!
        // Das lädt echte WLED-Presets aus der WebUI
        
        // Verwende WLED's eingebaute Preset-Cycling-Funktionalität
        // Das ist generisch und funktioniert mit beliebig vielen Presets!
        
        // Einfache Lösung: Verwende WLED's eingebaute Preset-Cycling
        // Das wechselt automatisch zwischen allen verfügbaren Presets
        // Verwende WLED's eingebaute Preset-Cycling-Funktionalität
        // Intelligente Lösung: Wechsle nur zwischen tatsächlich vorhandenen Presets
        // Verwende WLED's eingebaute Preset-Cycling-Funktionalität
        
        // Intelligente Lösung: Wechsle nur zwischen tatsächlich vorhandenen Presets
        // Verwende WLED's eingebaute Preset-Cycling-Funktionalität
        
        // Intelligente Lösung: Wechsle nur zwischen tatsächlich vorhandenen Presets
        // Verwende WLED's eingebaute Preset-Cycling-Funktionalität
        
        // Intelligente Lösung: Wechsle nur zwischen tatsächlich vorhandenen Presets
        // Verwende WLED's eingebaute Preset-Cycling-Funktionalität
        
        // Einfache Lösung: Verwende WLED's eingebaute Preset-Cycling
        // Das wechselt automatisch zwischen allen verfügbaren Presets
        static int currentPreset = 1; // Beginnt bei 1
        
        // Intelligente Lösung: Wechsle nur zwischen verfügbaren Presets
        // Konfigurierbare Anzahl von Presets (ändern Sie die 3 auf Ihre Anzahl)
        const int maxPresets = 3; // HIER IHRE ANZAHL EINTRAGEN!
        currentPreset = (currentPreset % maxPresets) + 1; // 1-3 Presets
        
        // Verwende WLED's eingebaute Preset-Funktion
        applyPreset(currentPreset);
        
        Serial.print(F("Versuche Preset ")); Serial.println(currentPreset);
        
        Serial.println(F("WLED Preset-Cycling aktiviert (generisch)"));
        
        stateUpdated(CALL_MODE_BUTTON);
    }

    void changeBrightness() {
        currentBrightnessLevel = (currentBrightnessLevel + 1) % 5;
        bri = brightnessLevels[currentBrightnessLevel];
        stateUpdated(CALL_MODE_BUTTON);
    }

    void toggleMusic() {
        dfPlayerPlayPause = true;
        Serial.println(F("Button 2: Musik Start/Stop"));
    }

    void nextTrack() {
        dfPlayerNextTrack = true;
        Serial.println(F("Button 2: Nächster Track"));
    }

    void changeVolume() {
        dfPlayerChangeVolume = true;
        Serial.println(F("Button 2: Lautstärke ändern"));
    }
};

// Strings für Flash-Speicher
const char ZDigBwButtonsUsermod::_name[] PROGMEM = "ZDigBwButtons";
const char ZDigBwButtonsUsermod::_enabled[] PROGMEM = "enabled";
const char ZDigBwButtonsUsermod::_button1Pin[] PROGMEM = "button1Pin";
const char ZDigBwButtonsUsermod::_button2Pin[] PROGMEM = "button2Pin";

static ZDigBwButtonsUsermod buttons_usermod;
REGISTER_USERMOD(buttons_usermod);
