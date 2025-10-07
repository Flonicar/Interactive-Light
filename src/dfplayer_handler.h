// DFPlayer Mini Handler für MP3-Wiedergabe
#ifndef DFPLAYER_HANDLER_H
#define DFPLAYER_HANDLER_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "config.h"

class DFPlayerHandler {
private:
    SoftwareSerial* mySerial;
    int volume;
    bool isPlaying;
    int currentTrack;
    int totalTracks;
    
public:
    DFPlayerHandler(int rxPin, int txPin);
    void begin();
    void update();
    
    // Grundfunktionen
    void play();
    void pause();
    void stop();
    void nextTrack();
    void previousTrack();
    
    // Lautstärke
    void setVolume(int vol);
    void volumeUp();
    void volumeDown();
    
    // Status
    bool getIsPlaying();
    int getCurrentTrack();
    int getTotalTracks();
    
    // Wiedergabe
    void playTrack(int track);
    void playFolder(int folder, int track);
    
private:
    void sendCommand(byte command, byte param1, byte param2);
    void waitForResponse();
};

#endif
