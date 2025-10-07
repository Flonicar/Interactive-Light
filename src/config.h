// ZDigBw Lampe - Hauptkonfiguration
#ifndef CONFIG_H
#define CONFIG_H

// LED-Konfiguration
#define LED_PIN 2                    // GPIO Pin für WS2812B Data
#define LED_COUNT 200                // Gesamtanzahl LEDs (anpassen!)
#define LED_SEGMENT_1_START 0        // Segment 1: LEDs 0-99
#define LED_SEGMENT_1_END 99
#define LED_SEGMENT_2_START 100      // Segment 2: LEDs 100-199
#define LED_SEGMENT_2_END 199

// Button-Konfiguration
#define BUTTON_1_PIN 12              // AN/AUS + Effekte (D6 auf ESP8266MOD)
#define BUTTON_2_PIN 13              // Musik + Zusätzlich (D7 auf ESP8266MOD)
#define BUTTON_DEBOUNCE_TIME 50      // ms
#define BUTTON_LONG_PRESS_TIME 2000  // ms für lange Drücke
#define BUTTON_DOUBLE_CLICK_TIME 500 // ms für Doppelklick

// DFPlayer Mini Konfiguration
#define DFPLAYER_RX_PIN 4            // ESP8266 TX -> DFPlayer RX
#define DFPLAYER_TX_PIN 5            // ESP8266 RX -> DFPlayer TX
#define DFPLAYER_VOLUME 20           // Standard-Lautstärke (0-30)

// WLED-Konfiguration
#define WLED_BRIGHTNESS 128          // Standard-Helligkeit (0-255)
#define WLED_EFFECT_SPEED 128        // Standard-Effektgeschwindigkeit

// Timing-Konfiguration
#define MAIN_LOOP_DELAY 10           // ms Hauptschleife
#define BUTTON_CHECK_INTERVAL 20     // ms Button-Check

#endif
