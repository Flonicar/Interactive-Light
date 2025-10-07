// Button-Handler für Mehrfachbelegung
#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>
#include "config.h"

class ButtonHandler {
private:
    int button1Pin;
    int button2Pin;
    
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

public:
    ButtonHandler(int pin1, int pin2);
    void begin();
    void update();
    
    // Button 1 Funktionen
    bool isButton1ShortPress();
    bool isButton1LongPress();
    bool isButton1DoubleClick();
    
    // Button 2 Funktionen
    bool isButton2ShortPress();
    bool isButton2LongPress();
    bool isButton2DoubleClick();
    
    // Reset-Funktionen
    void resetButton1();
    void resetButton2();
};

#endif
