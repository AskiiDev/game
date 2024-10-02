#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <stdexcept>
#include <vector>
#include <stdint.h>
#include "IOUtils.h"


struct WAVHeader {
    char riff[4]; // "RIFF"
    uint32_t fileSize;
    char wave[4]; // "WAVE"
    char fmt[4];  // "fmt "
    uint32_t fmtSize;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char data[4]; // "data"
    uint32_t dataSize;
};


class AudioManager {
public:
    ALuint sources[8];
    ALuint buffers[12];
    
private:
    ALCdevice* device;
    ALCcontext* context;
    
public:
    AudioManager();
    void init();
    void destroy();
    
    void playFromSource(uint8_t iD, const std::string& filename);
    
private:
    void enumerateDevice();
    void generateBuffers();
    void generateSources();
    
    void loadPCM(uint8_t iD, const std::string& filename);

};

#endif
