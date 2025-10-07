// DFPlayer Mini Handler Implementierung für MP3-Wiedergabe
#include "dfplayer_handler.h"

DFPlayerHandler::DFPlayerHandler(int rxPin, int txPin) {
    mySerial = new SoftwareSerial(rxPin, txPin);
    volume = DFPLAYER_VOLUME;
    isPlaying = false;
    currentTrack = 1;
    totalTracks = 0;
}

void DFPlayerHandler::begin() {
    mySerial->begin(9600);
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
    
    Serial.println("DFPlayer Mini initialisiert");
}

void DFPlayerHandler::update() {
    // DFPlayer Status prüfen (optional)
    // Hier könnten wir auf Antworten vom DFPlayer reagieren
}

// Grundfunktionen
void DFPlayerHandler::play() {
    sendCommand(0x0D, 0x00, 0x00); // Play
    isPlaying = true;
    delay(100);
}

void DFPlayerHandler::pause() {
    sendCommand(0x0E, 0x00, 0x00); // Pause
    isPlaying = false;
    delay(100);
}

void DFPlayerHandler::stop() {
    sendCommand(0x16, 0x00, 0x00); // Stop
    isPlaying = false;
    delay(100);
}

void DFPlayerHandler::nextTrack() {
    sendCommand(0x01, 0x00, 0x00); // Next Track
    currentTrack++;
    if (currentTrack > totalTracks) {
        currentTrack = 1;
    }
    delay(100);
}

void DFPlayerHandler::previousTrack() {
    sendCommand(0x02, 0x00, 0x00); // Previous Track
    currentTrack--;
    if (currentTrack < 1) {
        currentTrack = totalTracks;
    }
    delay(100);
}

// Lautstärke
void DFPlayerHandler::setVolume(int vol) {
    if (vol < 0) vol = 0;
    if (vol > 30) vol = 30;
    
    volume = vol;
    sendCommand(0x06, 0x00, vol);
    delay(100);
}

void DFPlayerHandler::volumeUp() {
    if (volume < 30) {
        volume++;
        setVolume(volume);
    }
}

void DFPlayerHandler::volumeDown() {
    if (volume > 0) {
        volume--;
        setVolume(volume);
    }
}

// Status
bool DFPlayerHandler::getIsPlaying() {
    return isPlaying;
}

int DFPlayerHandler::getCurrentTrack() {
    return currentTrack;
}

int DFPlayerHandler::getTotalTracks() {
    return totalTracks;
}

// Wiedergabe
void DFPlayerHandler::playTrack(int track) {
    if (track >= 1 && track <= totalTracks) {
        sendCommand(0x03, 0x00, track);
        currentTrack = track;
        isPlaying = true;
        delay(100);
    }
}

void DFPlayerHandler::playFolder(int folder, int track) {
    if (folder >= 1 && folder <= 99 && track >= 1 && track <= 255) {
        sendCommand(0x0F, folder, track);
        isPlaying = true;
        delay(100);
    }
}

// Private Funktionen
void DFPlayerHandler::sendCommand(byte command, byte param1, byte param2) {
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
        mySerial->write(commandBuffer[i]);
    }
}

void DFPlayerHandler::waitForResponse() {
    unsigned long startTime = millis();
    while (mySerial->available() == 0 && millis() - startTime < 1000) {
        delay(10);
    }
    
    // Antwort lesen (optional)
    while (mySerial->available() > 0) {
        mySerial->read();
    }
}