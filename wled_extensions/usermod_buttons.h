// WLED UserMod für Button-Integration
// Diese Datei in den WLED-Ordner kopieren: wled00/usermods/

#pragma once

#include "wled.h"

// Button-Konfiguration
#define BUTTON_1_PIN 12  // AN/AUS + Effekte
#define BUTTON_2_PIN 13  // Musik + Zusätzlich
#define BUTTON_DEBOUNCE_TIME 50
#define BUTTON_LONG_PRESS_TIME 2000
#define BUTTON_DOUBLE_CLICK_TIME 500

class UsermodButtons : public Usermod {
private:
    bool enabled = true;
    
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
    
    // Debouncing
    unsigned long lastButton1Check = 0;
    unsigned long lastButton2Check = 0;
    
    // Helligkeitsstufen
    int brightnessLevels[5] = {32, 64, 128, 192, 255};
    int currentBrightnessLevel = 2;
    
    // Effekt-Indizes
    int currentEffect1 = 0;
    int currentEffect2 = 0;
    
public:
    void setup() override {
        if (!enabled) return;
        
        pinMode(BUTTON_1_PIN, INPUT_PULLUP);
        pinMode(BUTTON_2_PIN, INPUT_PULLUP);
        
        DEBUG_PRINTLN("Buttons initialisiert");
        DEBUG_PRINTF("Button 1 Pin: %d\n", BUTTON_1_PIN);
        DEBUG_PRINTF("Button 2 Pin: %d\n", BUTTON_2_PIN);
    }
    
    void loop() override {
        if (!enabled) return;
        
        updateButtons();
        handleButtonActions();
    }
    
    void updateButtons() {
        unsigned long currentTime = millis();
        
        // Button 1 Update
        if (currentTime - lastButton1Check >= 20) {
            bool currentState1 = !digitalRead(BUTTON_1_PIN);
            
            if (currentState1 && !button1Pressed) {
                button1Pressed = true;
                button1PressTime = currentTime;
                button1ClickCount++;
            } else if (!currentState1 && button1Pressed) {
                button1Pressed = false;
                button1LongPressed = false;
            } else if (currentState1 && button1Pressed && 
                       (currentTime - button1PressTime >= BUTTON_LONG_PRESS_TIME) && 
                       !button1LongPressed) {
                button1LongPressed = true;
            }
            
            lastButton1Check = currentTime;
        }
        
        // Button 2 Update
        if (currentTime - lastButton2Check >= 20) {
            bool currentState2 = !digitalRead(BUTTON_2_PIN);
            
            if (currentState2 && !button2Pressed) {
                button2Pressed = true;
                button2PressTime = currentTime;
                button2ClickCount++;
            } else if (!currentState2 && button2Pressed) {
                button2Pressed = false;
                button2LongPressed = false;
            } else if (currentState2 && button2Pressed && 
                       (currentTime - button2PressTime >= BUTTON_LONG_PRESS_TIME) && 
                       !button2LongPressed) {
                button2LongPressed = true;
            }
            
            lastButton2Check = currentTime;
        }
    }
    
    void handleButtonActions() {
        // Button 1 - AN/AUS + Effekte
        if (isButton1ShortPress()) {
            togglePower();
            DEBUG_PRINTLN("Button 1: Power Toggle");
        }
        
        if (isButton1LongPress()) {
            nextEffect();
            DEBUG_PRINTLN("Button 1: Next Effect");
        }
        
        if (isButton1DoubleClick()) {
            nextBrightness();
            DEBUG_PRINTLN("Button 1: Next Brightness");
        }
        
        // Button 2 - Musik + Zusätzlich
        if (isButton2ShortPress()) {
            toggleMusic();
            DEBUG_PRINTLN("Button 2: Music Toggle");
        }
        
        if (isButton2LongPress()) {
            nextMusicTrack();
            DEBUG_PRINTLN("Button 2: Next Music Track");
        }
        
        if (isButton2DoubleClick()) {
            musicVolumeUp();
            DEBUG_PRINTLN("Button 2: Music Volume Up");
        }
    }
    
    // Button 1 Funktionen
    bool isButton1ShortPress() {
        if (button1Pressed || button1LongPressed) return false;
        
        if (button1ClickCount > 0 && (millis() - button1PressTime >= BUTTON_DEBOUNCE_TIME)) {
            button1ClickCount = 0;
            return true;
        }
        return false;
    }
    
    bool isButton1LongPress() {
        if (button1LongPressed) {
            button1LongPressed = false;
            return true;
        }
        return false;
    }
    
    bool isButton1DoubleClick() {
        if (button1ClickCount >= 2 && (millis() - button1PressTime >= BUTTON_DOUBLE_CLICK_TIME)) {
            button1ClickCount = 0;
            return true;
        }
        return false;
    }
    
    // Button 2 Funktionen
    bool isButton2ShortPress() {
        if (button2Pressed || button2LongPressed) return false;
        
        if (button2ClickCount > 0 && (millis() - button2PressTime >= BUTTON_DEBOUNCE_TIME)) {
            button2ClickCount = 0;
            return true;
        }
        return false;
    }
    
    bool isButton2LongPress() {
        if (button2LongPressed) {
            button2LongPressed = false;
            return true;
        }
        return false;
    }
    
    bool isButton2DoubleClick() {
        if (button2ClickCount >= 2 && (millis() - button2PressTime >= BUTTON_DOUBLE_CLICK_TIME)) {
            button2ClickCount = 0;
            return true;
        }
        return false;
    }
    
    // Aktionen
    void togglePower() {
        if (bri > 0) {
            bri = 0;
            stateChanged = true;
        } else {
            bri = brightnessLevels[currentBrightnessLevel];
            stateChanged = true;
        }
    }
    
    void nextEffect() {
        if (bri == 0) return;
        
        effectCurrent = (effectCurrent + 1) % strip.getModeCount();
        stateChanged = true;
        DEBUG_PRINTF("Effect changed to: %d\n", effectCurrent);
    }
    
    void nextBrightness() {
        if (bri == 0) return;
        
        currentBrightnessLevel = (currentBrightnessLevel + 1) % 5;
        bri = brightnessLevels[currentBrightnessLevel];
        stateChanged = true;
        DEBUG_PRINTF("Brightness changed to: %d\n", bri);
    }
    
    void toggleMusic() {
        if (dfplayerMod) {
            dfplayerMod->togglePlayPause();
        }
    }
    
    void nextMusicTrack() {
        if (dfplayerMod) {
            dfplayerMod->nextTrack();
        }
    }
    
    void musicVolumeUp() {
        if (dfplayerMod) {
            dfplayerMod->volumeUp();
        }
    }
    
    void enable() {
        enabled = true;
        setup();
    }
    
    void disable() {
        enabled = false;
    }
    
    bool isEnabled() {
        return enabled;
    }
};

// Globale Instanz
UsermodButtons* buttonMod = nullptr;
