#include "wled.h"
#undef R  // Fix macro conflict with SoftwareSerial
#include <SoftwareSerial.h>

/*
 * ZDigBw Lampe - DFPlayer Mini Usermod
 * Erweitert WLED um MP3-Wiedergabe mit DFPlayer Mini
 * 
 * RX Pin (D2/GPIO4): DFPlayer TX
 * TX Pin (D1/GPIO5): DFPlayer RX
 */

// Globale Variablen für Button-Kommunikation
bool dfPlayerPlayPause = false;
bool dfPlayerNextTrack = false;
bool dfPlayerChangeVolume = false;

class ZDigBwDFPlayerUsermod : public Usermod {
private:
    bool enabled = true;
    bool initDone = false;
    
    // DFPlayer Pins
    int8_t rxPin = 4;  // D2 - DFPlayer TX
    int8_t txPin = 5;  // D1 - DFPlayer RX
    
    // SoftwareSerial für DFPlayer
    SoftwareSerial* dfSerial;
    
    // DFPlayer Zustand
    bool isPlaying = false;
    int volume = 20;
    int currentTrack = 1;
    int totalTracks = 0;
    
    // Musik-Preset Konfiguration
    int musicPreset = 2;  // Preset das beim Musikstart geladen wird
    
    // Strings für Flash-Speicher
    static const char _name[];
    static const char _enabled[];
    static const char _rxPin[];
    static const char _txPin[];
    static const char _volume[];
    static const char _musicPreset[];

public:
    void setup() override {
        if (rxPin >= 0 && txPin >= 0) {
            dfSerial = new SoftwareSerial(rxPin, txPin);
            dfSerial->begin(9600);
            delay(1000);
            
            // DFPlayer initialisieren
            sendCommand(0x3F, 0x00, 0x00); // Query Status
            delay(100);
            
            // Lautstärke setzen
            setVolume(volume);
            delay(100);
            
            // Anzahl Tracks ermitteln
            sendCommand(0x48, 0x00, 0x00); // Query Total Files
            delay(100);
        }
        initDone = true;
    }

    void loop() override {
        if (!enabled || !initDone || !dfSerial) return;
        
        // Button-Ereignisse verarbeiten
        if (dfPlayerPlayPause) {
            dfPlayerPlayPause = false;
            if (isPlaying) {
                pause();
            } else {
                play();
                // Musik-Preset laden beim Start
                loadMusicPreset();
            }
        }
        
        if (dfPlayerNextTrack) {
            dfPlayerNextTrack = false;
            nextTrack();
        }
        
        if (dfPlayerChangeVolume) {
            dfPlayerChangeVolume = false;
            volume = (volume + 5) % 31; // 0-30
            setVolume(volume);
        }
        
        // Musik-Ende-Erkennung
        checkMusicEnd();
    }

    void connected() override {
        // Optional: MQTT-Topics für Musik-Steuerung
    }

    void addToJsonInfo(JsonObject& root) override {
        JsonObject user = root["u"];
        if (user.isNull()) user = root.createNestedObject("u");
        
        JsonArray info = user.createNestedArray(FPSTR(_name));
        info.add(F("DFPlayer Mini MP3-Player"));
        info.add("Volume: " + String(volume));
        info.add("Track: " + String(currentTrack));
        info.add("Music Preset: " + String(musicPreset));
    }

    void addToJsonState(JsonObject& root) override {
        if (!initDone) return;
        
        JsonObject usermod = root[FPSTR(_name)];
        if (usermod.isNull()) usermod = root.createNestedObject(FPSTR(_name));
        
        usermod["rxPin"] = rxPin;
        usermod["txPin"] = txPin;
        usermod["volume"] = volume;
        usermod["isPlaying"] = isPlaying;
        usermod["currentTrack"] = currentTrack;
        usermod["totalTracks"] = totalTracks;
        usermod["musicPreset"] = musicPreset;
    }

    void readFromJsonState(JsonObject& root) override {
        if (!initDone) return;
        
        JsonObject usermod = root[FPSTR(_name)];
        if (!usermod.isNull()) {
            if (usermod["volume"] != nullptr) {
                int newVolume = usermod["volume"];
                if (newVolume >= 0 && newVolume <= 30) {
                    setVolume(newVolume);
                }
            }
            
            if (usermod["play"] != nullptr) {
                if (usermod["play"]) {
                    play();
                } else {
                    pause();
                }
            }
            
            if (usermod["next"] != nullptr && usermod["next"]) {
                nextTrack();
            }
            
            if (usermod["musicPreset"] != nullptr) {
                int newPreset = usermod["musicPreset"];
                if (newPreset >= 1 && newPreset <= 10) {
                    musicPreset = newPreset;
                }
            }
        }
    }

    void addToConfig(JsonObject& root) override {
        JsonObject top = root.createNestedObject(FPSTR(_name));
        top[FPSTR(_enabled)] = enabled;
        top[FPSTR(_rxPin)] = rxPin;
        top[FPSTR(_txPin)] = txPin;
        top[FPSTR(_volume)] = volume;
        top[FPSTR(_musicPreset)] = musicPreset;
    }

    bool readFromConfig(JsonObject& root) override {
        JsonObject top = root[FPSTR(_name)];
        
        bool configComplete = !top.isNull();
        configComplete &= getJsonValue(top[FPSTR(_enabled)], enabled);
        configComplete &= getJsonValue(top[FPSTR(_rxPin)], rxPin, 4);
        configComplete &= getJsonValue(top[FPSTR(_txPin)], txPin, 5);
        configComplete &= getJsonValue(top[FPSTR(_volume)], volume, 20);
        configComplete &= getJsonValue(top[FPSTR(_musicPreset)], musicPreset, 2);
        
        return configComplete;
    }

    void appendConfigData() override {
        oappend(F("addInfo('")); oappend(String(FPSTR(_name)).c_str()); oappend(F(":rxPin")); oappend(F("',1,'DFPlayer RX Pin (D2/GPIO4)');"));
        oappend(F("addInfo('")); oappend(String(FPSTR(_name)).c_str()); oappend(F(":txPin")); oappend(F("',1,'DFPlayer TX Pin (D1/GPIO5)');"));
        oappend(F("addInfo('")); oappend(String(FPSTR(_name)).c_str()); oappend(F(":volume")); oappend(F("',1,'Volume (0-30)');"));
        oappend(F("addInfo('")); oappend(String(FPSTR(_name)).c_str()); oappend(F(":musicPreset")); oappend(F("',1,'Music Preset (1-10)');"));
    }

    // DFPlayer-Funktionen
    void play() {
        if (!dfSerial) return;
        sendCommand(0x0D, 0x00, 0x00); // Play
        isPlaying = true;
        delay(100);
    }

    void pause() {
        if (!dfSerial) return;
        sendCommand(0x0E, 0x00, 0x00); // Pause
        isPlaying = false;
        delay(100);
    }

    void stop() {
        if (!dfSerial) return;
        sendCommand(0x16, 0x00, 0x00); // Stop
        isPlaying = false;
        delay(100);
    }

    void nextTrack() {
        if (!dfSerial) return;
        sendCommand(0x01, 0x00, 0x00); // Next Track
        currentTrack++;
        if (currentTrack > totalTracks) {
            currentTrack = 1;
        }
        delay(100);
    }

    void previousTrack() {
        if (!dfSerial) return;
        sendCommand(0x02, 0x00, 0x00); // Previous Track
        currentTrack--;
        if (currentTrack < 1) {
            currentTrack = totalTracks;
        }
        delay(100);
    }

    void setVolume(int vol) {
        if (!dfSerial) return;
        if (vol < 0) vol = 0;
        if (vol > 30) vol = 30;
        
        volume = vol;
        sendCommand(0x06, 0x00, vol);
        delay(100);
    }

    void volumeUp() {
        if (volume < 30) {
            volume++;
            setVolume(volume);
        }
    }

    void volumeDown() {
        if (volume > 0) {
            volume--;
            setVolume(volume);
        }
    }

    void playTrack(int track) {
        if (!dfSerial) return;
        if (track >= 1 && track <= totalTracks) {
            sendCommand(0x03, 0x00, track);
            currentTrack = track;
            isPlaying = true;
            delay(100);
        }
    }

    // Musik-Preset Funktion
    void loadMusicPreset() {
        // WLED Preset laden beim Musikstart
        applyPreset(musicPreset);
        Serial.print(F("Musik-Preset ")); Serial.print(musicPreset); Serial.println(F(" geladen"));
    }
    
    // Musik-Ende-Erkennung
    void checkMusicEnd() {
        static unsigned long lastCheck = 0;
        static bool wasPlaying = false;
        
        // Nur alle 2 Sekunden prüfen
        if (millis() - lastCheck < 2000) return;
        lastCheck = millis();
        
        // Prüfe ob Musik gestoppt wurde
        if (wasPlaying && !isPlaying) {
            // Musik ist zu Ende - lade Preset 1
            applyPreset(1);
            Serial.println(F("Musik beendet - Preset 1 geladen"));
        }
        
        wasPlaying = isPlaying;
    }
    
    // Public Getter für Button-Usermod
    bool getIsPlaying() { return isPlaying; }
    int getCurrentTrack() { return currentTrack; }
    int getVolume() { return volume; }
    int getMusicPreset() { return musicPreset; }

    uint16_t getId() override {
        return USERMOD_ID_DFPLAYER;
    }

private:
    void sendCommand(byte command, byte param1, byte param2) {
        if (!dfSerial) return;
        
        byte commandBuffer[10];
        
        // DFPlayer Command Format
        commandBuffer[0] = 0x7E;  // Start Byte
        commandBuffer[1] = 0xFF;  // Version
        commandBuffer[2] = 0x06;  // Length
        commandBuffer[3] = command; // Command
        commandBuffer[4] = 0x00;   // Feedback
        commandBuffer[5] = param1; // Parameter 1
        commandBuffer[6] = param2; // Parameter 2
        commandBuffer[7] = 0x00;   // Checksum High
        commandBuffer[8] = 0x00;   // Checksum Low
        commandBuffer[9] = 0xEF;   // End Byte
        
        // Checksum berechnen
        uint16_t checksum = 0xFFFF - (0xFF + 0x06 + command + 0x00 + param1 + param2) + 1;
        commandBuffer[7] = checksum >> 8;
        commandBuffer[8] = checksum & 0xFF;
        
        // Command senden
        for (int i = 0; i < 10; i++) {
            dfSerial->write(commandBuffer[i]);
        }
    }
};

// Strings für Flash-Speicher
const char ZDigBwDFPlayerUsermod::_name[] PROGMEM = "ZDigBwDFPlayer";
const char ZDigBwDFPlayerUsermod::_enabled[] PROGMEM = "enabled";
const char ZDigBwDFPlayerUsermod::_rxPin[] PROGMEM = "rxPin";
const char ZDigBwDFPlayerUsermod::_txPin[] PROGMEM = "txPin";
const char ZDigBwDFPlayerUsermod::_volume[] PROGMEM = "volume";
const char ZDigBwDFPlayerUsermod::_musicPreset[] PROGMEM = "musicPreset";

static ZDigBwDFPlayerUsermod dfplayer_usermod;
REGISTER_USERMOD(dfplayer_usermod);
