// WLED Integration Implementierung für zwei LED-Segmente
#include "wled_integration.h"

WLEDIntegration::WLEDIntegration() {
    leds = new CRGB[LED_COUNT];
    segment1Start = LED_SEGMENT_1_START;
    segment1End = LED_SEGMENT_1_END;
    segment2Start = LED_SEGMENT_2_START;
    segment2End = LED_SEGMENT_2_END;
    
    // Segment 1 Initialzustand
    segment1On = false;
    segment1Brightness = WLED_BRIGHTNESS;
    segment1Effect = 0;
    segment1Color = CRGB::White;
    
    // Segment 2 Initialzustand
    segment2On = false;
    segment2Brightness = WLED_BRIGHTNESS;
    segment2Effect = 0;
    segment2Color = CRGB::White;
    
    // Effekt-Timing
    lastEffectUpdate = 0;
    effectSpeed = WLED_EFFECT_SPEED;
}

void WLEDIntegration::begin() {
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_COUNT);
    FastLED.setBrightness(WLED_BRIGHTNESS);
    FastLED.clear();
    FastLED.show();
    
    Serial.println("WLED Integration initialisiert");
    Serial.print("LED Count: ");
    Serial.println(LED_COUNT);
    Serial.print("Segment 1: LEDs ");
    Serial.print(segment1Start);
    Serial.print("-");
    Serial.println(segment1End);
    Serial.print("Segment 2: LEDs ");
    Serial.print(segment2Start);
    Serial.print("-");
    Serial.println(segment2End);
}

void WLEDIntegration::update() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastEffectUpdate >= effectSpeed) {
        updateEffect();
        lastEffectUpdate = currentTime;
    }
    
    FastLED.show();
}

// Segment 1 Funktionen
void WLEDIntegration::setSegment1On(bool on) {
    segment1On = on;
    if (!on) {
        // Segment ausschalten
        for (int i = segment1Start; i <= segment1End; i++) {
            leds[i] = CRGB::Black;
        }
    }
}

void WLEDIntegration::setSegment1Brightness(int brightness) {
    segment1Brightness = constrain(brightness, 0, 255);
}

void WLEDIntegration::setSegment1Effect(int effect) {
    segment1Effect = effect;
}

void WLEDIntegration::setSegment1Color(CRGB color) {
    segment1Color = color;
}

void WLEDIntegration::nextSegment1Effect() {
    segment1Effect = (segment1Effect + 1) % 4; // 4 verschiedene Effekte
}

// Segment 2 Funktionen
void WLEDIntegration::setSegment2On(bool on) {
    segment2On = on;
    if (!on) {
        // Segment ausschalten
        for (int i = segment2Start; i <= segment2End; i++) {
            leds[i] = CRGB::Black;
        }
    }
}

void WLEDIntegration::setSegment2Brightness(int brightness) {
    segment2Brightness = constrain(brightness, 0, 255);
}

void WLEDIntegration::setSegment2Effect(int effect) {
    segment2Effect = effect;
}

void WLEDIntegration::setSegment2Color(CRGB color) {
    segment2Color = color;
}

void WLEDIntegration::nextSegment2Effect() {
    segment2Effect = (segment2Effect + 1) % 4; // 4 verschiedene Effekte
}

// Globale Funktionen
void WLEDIntegration::setAllOn(bool on) {
    setSegment1On(on);
    setSegment2On(on);
}

void WLEDIntegration::setAllBrightness(int brightness) {
    setSegment1Brightness(brightness);
    setSegment2Brightness(brightness);
}

void WLEDIntegration::setAllEffect(int effect) {
    setSegment1Effect(effect);
    setSegment2Effect(effect);
}

void WLEDIntegration::setAllColor(CRGB color) {
    setSegment1Color(color);
    setSegment2Color(color);
}

// Status
bool WLEDIntegration::getSegment1On() {
    return segment1On;
}

bool WLEDIntegration::getSegment2On() {
    return segment2On;
}

int WLEDIntegration::getSegment1Brightness() {
    return segment1Brightness;
}

int WLEDIntegration::getSegment2Brightness() {
    return segment2Brightness;
}

// Private Funktionen
void WLEDIntegration::updateEffect() {
    // Segment 1 Effekte
    if (segment1On) {
        applyEffect(segment1Start, segment1End, segment1Effect, segment1Color, segment1Brightness);
    }
    
    // Segment 2 Effekte
    if (segment2On) {
        applyEffect(segment2Start, segment2End, segment2Effect, segment2Color, segment2Brightness);
    }
}

void WLEDIntegration::applyEffect(int start, int end, int effect, CRGB color, int brightness) {
    switch (effect) {
        case 0: // Solid Color
            solidColor(start, end, color, brightness);
            break;
        case 1: // Rainbow
            rainbow(start, end, brightness);
            break;
        case 2: // Breathing
            breathing(start, end, color, brightness);
            break;
        case 3: // Twinkle
            twinkle(start, end, color, brightness);
            break;
        default:
            solidColor(start, end, color, brightness);
            break;
    }
}

void WLEDIntegration::solidColor(int start, int end, CRGB color, int brightness) {
    CRGB adjustedColor = color;
    adjustedColor.nscale8(brightness);
    
    for (int i = start; i <= end; i++) {
        leds[i] = adjustedColor;
    }
}

void WLEDIntegration::rainbow(int start, int end, int brightness) {
    static uint8_t hue = 0;
    hue += 2;
    
    for (int i = start; i <= end; i++) {
        leds[i] = CHSV(hue + (i * 2), 255, brightness);
    }
}

void WLEDIntegration::breathing(int start, int end, CRGB color, int brightness) {
    static uint8_t breath = 0;
    breath += 3;
    
    uint8_t breathValue = sin8(breath);
    uint8_t adjustedBrightness = scale8(brightness, breathValue);
    
    CRGB adjustedColor = color;
    adjustedColor.nscale8(adjustedBrightness);
    
    for (int i = start; i <= end; i++) {
        leds[i] = adjustedColor;
    }
}

void WLEDIntegration::twinkle(int start, int end, CRGB color, int brightness) {
    static uint8_t twinkleCounter = 0;
    twinkleCounter += 5;
    
    for (int i = start; i <= end; i++) {
        uint8_t twinkleValue = sin8(twinkleCounter + (i * 10));
        uint8_t adjustedBrightness = scale8(brightness, twinkleValue);
        
        CRGB adjustedColor = color;
        adjustedColor.nscale8(adjustedBrightness);
        leds[i] = adjustedColor;
    }
}