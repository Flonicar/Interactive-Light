// WLED UserMod für DFPlayer Mini Integration
// Diese Datei in den WLED-Ordner kopieren: wled00/usermods/

#pragma once

#include "wled.h"

// DFPlayer Mini Konfiguration
#define DFPLAYER_RX_PIN 4
#define DFPLAYER_TX_PIN 5
#define DFPLAYER_VOLUME 20

class UsermodDFPlayer : public Usermod {
private:
    SoftwareSerial* dfplayerSerial;
    bool enabled = true;
    bool isPlaying = false;
    int volume = DFPLAYER_VOLUME;
    int currentTrack = 1;
    unsigned long lastUpdate = 0;
    
public:
    void setup() override {
        if (!enabled) return;
        
        dfplayerSerial = new SoftwareSerial(DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);
        dfplayerSerial->begin(9600);
        delay(1000);
        
        // DFPlayer initialisieren
        sendCommand(0x3F, 0x00, 0x00); // Query total files
        delay(500);
        setVolume(volume);
        delay(500);
        
        DEBUG_PRINTLN("DFPlayer Mini initialisiert");
    }
    
    void loop() override {
        if (!enabled) return;
        
        unsigned long now = millis();
        if (now - lastUpdate >= 100) {
            // Hier können wir Status-Updates verarbeiten
            lastUpdate = now;
        }
    }
    
    void play() {
        if (!enabled) return;
        sendCommand(0x0D, 0x00, 0x00); // Play
        isPlaying = true;
        DEBUG_PRINTLN("DFPlayer: Play");
    }
    
    void pause() {
        if (!enabled) return;
        sendCommand(0x0E, 0x00, 0x00); // Pause
        isPlaying = false;
        DEBUG_PRINTLN("DFPlayer: Pause");
    }
    
    void stop() {
        if (!enabled) return;
        sendCommand(0x16, 0x00, 0x00); // Stop
        isPlaying = false;
        DEBUG_PRINTLN("DFPlayer: Stop");
    }
    
    void nextTrack() {
        if (!enabled) return;
        sendCommand(0x01, 0x00, 0x00); // Next
        currentTrack++;
        DEBUG_PRINTLN("DFPlayer: Next Track");
    }
    
    void previousTrack() {
        if (!enabled) return;
        sendCommand(0x02, 0x00, 0x00); // Previous
        currentTrack--;
        if (currentTrack < 1) currentTrack = 1;
        DEBUG_PRINTLN("DFPlayer: Previous Track");
    }
    
    void setVolume(int vol) {
        if (!enabled) return;
        if (vol < 0) vol = 0;
        if (vol > 30) vol = 30;
        
        volume = vol;
        sendCommand(0x06, 0x00, vol);
        DEBUG_PRINTF("DFPlayer: Volume set to %d\n", vol);
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
    
    bool getIsPlaying() {
        return isPlaying;
    }
    
    int getVolume() {
        return volume;
    }
    
    int getCurrentTrack() {
        return currentTrack;
    }
    
    void playTrack(int track) {
        if (!enabled) return;
        if (track < 1) track = 1;
        
        currentTrack = track;
        sendCommand(0x03, 0x00, track);
        isPlaying = true;
        DEBUG_PRINTF("DFPlayer: Play Track %d\n", track);
    }
    
    void playFolder(int folder, int track) {
        if (!enabled) return;
        sendCommand(0x0F, folder, track);
        isPlaying = true;
        DEBUG_PRINTF("DFPlayer: Play Folder %d, Track %d\n", folder, track);
    }
    
    void togglePlayPause() {
        if (isPlaying) {
            pause();
        } else {
            play();
        }
    }
    
    void enable() {
        enabled = true;
        setup();
    }
    
    void disable() {
        enabled = false;
        if (dfplayerSerial) {
            delete dfplayerSerial;
            dfplayerSerial = nullptr;
        }
    }
    
    bool isEnabled() {
        return enabled;
    }
    
private:
    void sendCommand(byte command, byte param1, byte param2) {
        if (!dfplayerSerial) return;
        
        byte Start_Byte = 0x7E;
        byte Version_Byte = 0xFF;
        byte Command_Length = 0x06;
        byte End_Byte = 0xEF;
        byte Ack = 0x00;
        
        byte checksum = -(Version_Byte + Command_Length + command + Ack + param1 + param2);
        
        byte Command_line[10] = {Start_Byte, Version_Byte, Command_Length, command, Ack, param1, param2, checksum, End_Byte};
        
        for (byte k = 0; k < 10; k++) {
            dfplayerSerial->write(Command_line[k]);
        }
        
        delay(100);
    }
};

// Globale Instanz
UsermodDFPlayer* dfplayerMod = nullptr;
