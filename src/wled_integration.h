// WLED Integration für zwei LED-Segmente
#ifndef WLED_INTEGRATION_H
#define WLED_INTEGRATION_H

#include <Arduino.h>
#include <FastLED.h>
#include "config.h"

class WLEDIntegration {
private:
    CRGB* leds;
    int segment1Start;
    int segment1End;
    int segment2Start;
    int segment2End;
    
    // Segment 1 Zustand
    bool segment1On;
    int segment1Brightness;
    int segment1Effect;
    CRGB segment1Color;
    
    // Segment 2 Zustand
    bool segment2On;
    int segment2Brightness;
    int segment2Effect;
    CRGB segment2Color;
    
    // Effekt-Timing
    unsigned long lastEffectUpdate;
    int effectSpeed;
    
public:
    WLEDIntegration();
    void begin();
    void update();
    
    // Segment 1 Funktionen
    void setSegment1On(bool on);
    void setSegment1Brightness(int brightness);
    void setSegment1Effect(int effect);
    void setSegment1Color(CRGB color);
    void nextSegment1Effect();
    
    // Segment 2 Funktionen
    void setSegment2On(bool on);
    void setSegment2Brightness(int brightness);
    void setSegment2Effect(int effect);
    void setSegment2Color(CRGB color);
    void nextSegment2Effect();
    
    // Globale Funktionen
    void setAllOn(bool on);
    void setAllBrightness(int brightness);
    void setAllEffect(int effect);
    void setAllColor(CRGB color);
    
    // Status
    bool getSegment1On();
    bool getSegment2On();
    int getSegment1Brightness();
    int getSegment2Brightness();
    
private:
    void updateEffect();
    void applyEffect(int start, int end, int effect, CRGB color, int brightness);
    void solidColor(int start, int end, CRGB color, int brightness);
    void rainbow(int start, int end, int brightness);
    void breathing(int start, int end, CRGB color, int brightness);
    void twinkle(int start, int end, CRGB color, int brightness);
};

#endif
