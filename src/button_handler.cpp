// Button-Handler Implementierung für Mehrfachbelegung
#include "button_handler.h"

ButtonHandler::ButtonHandler(int pin1, int pin2) {
    button1Pin = pin1;
    button2Pin = pin2;
}

void ButtonHandler::begin() {
    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    
    // Initialzustand
    button1Pressed = false;
    button1LongPressed = false;
    button1PressTime = 0;
    button1LastClick = 0;
    button1ClickCount = 0;
    
    button2Pressed = false;
    button2LongPressed = false;
    button2PressTime = 0;
    button2LastClick = 0;
    button2ClickCount = 0;
    
    lastButton1Check = 0;
    lastButton2Check = 0;
}

void ButtonHandler::update() {
    unsigned long currentTime = millis();
    
    // Button 1 Update
    if (currentTime - lastButton1Check >= BUTTON_CHECK_INTERVAL) {
        bool currentState = !digitalRead(button1Pin); // Invertiert wegen Pull-up
        
        if (currentState && !button1Pressed) {
            // Button gerade gedrückt
            button1Pressed = true;
            button1PressTime = currentTime;
            button1LongPressed = false;
        } else if (!currentState && button1Pressed) {
            // Button gerade losgelassen
            button1Pressed = false;
            button1LongPressed = false;
            
            // Doppelklick prüfen
            if (currentTime - button1LastClick < BUTTON_DOUBLE_CLICK_TIME) {
                button1ClickCount++;
            } else {
                button1ClickCount = 1;
            }
            button1LastClick = currentTime;
        } else if (currentState && button1Pressed && !button1LongPressed) {
            // Langen Druck prüfen
            if (currentTime - button1PressTime >= BUTTON_LONG_PRESS_TIME) {
                button1LongPressed = true;
            }
        }
        
        lastButton1Check = currentTime;
    }
    
    // Button 2 Update
    if (currentTime - lastButton2Check >= BUTTON_CHECK_INTERVAL) {
        bool currentState = !digitalRead(button2Pin); // Invertiert wegen Pull-up
        
        if (currentState && !button2Pressed) {
            // Button gerade gedrückt
            button2Pressed = true;
            button2PressTime = currentTime;
            button2LongPressed = false;
        } else if (!currentState && button2Pressed) {
            // Button gerade losgelassen
            button2Pressed = false;
            button2LongPressed = false;
            
            // Doppelklick prüfen
            if (currentTime - button2LastClick < BUTTON_DOUBLE_CLICK_TIME) {
                button2ClickCount++;
            } else {
                button2ClickCount = 1;
            }
            button2LastClick = currentTime;
        } else if (currentState && button2Pressed && !button2LongPressed) {
            // Langen Druck prüfen
            if (currentTime - button2PressTime >= BUTTON_LONG_PRESS_TIME) {
                button2LongPressed = true;
            }
        }
        
        lastButton2Check = currentTime;
    }
}

// Button 1 Funktionen
bool ButtonHandler::isButton1ShortPress() {
    if (button1ClickCount == 1 && !button1LongPressed && 
        millis() - button1LastClick < BUTTON_DOUBLE_CLICK_TIME) {
        button1ClickCount = 0;
        return true;
    }
    return false;
}

bool ButtonHandler::isButton1LongPress() {
    if (button1LongPressed) {
        button1LongPressed = false;
        return true;
    }
    return false;
}

bool ButtonHandler::isButton1DoubleClick() {
    if (button1ClickCount >= 2) {
        button1ClickCount = 0;
        return true;
    }
    return false;
}

// Button 2 Funktionen
bool ButtonHandler::isButton2ShortPress() {
    if (button2ClickCount == 1 && !button2LongPressed && 
        millis() - button2LastClick < BUTTON_DOUBLE_CLICK_TIME) {
        button2ClickCount = 0;
        return true;
    }
    return false;
}

bool ButtonHandler::isButton2LongPress() {
    if (button2LongPressed) {
        button2LongPressed = false;
        return true;
    }
    return false;
}

bool ButtonHandler::isButton2DoubleClick() {
    if (button2ClickCount >= 2) {
        button2ClickCount = 0;
        return true;
    }
    return false;
}

// Reset-Funktionen
void ButtonHandler::resetButton1() {
    button1Pressed = false;
    button1LongPressed = false;
    button1ClickCount = 0;
}

void ButtonHandler::resetButton2() {
    button2Pressed = false;
    button2LongPressed = false;
    button2ClickCount = 0;
}