#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <future>
#include <thread>

#include <stdint.h>
#include "IOUtils.h"


#define NUM_SOURCES 8
#define NUM_BUFFERS 12


struct SourceProperties
{
    float pitch;
    float gain;
    bool looping;
};

struct WAVHeader
{
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
private:
    ALuint sources[NUM_SOURCES];
    ALuint buffers[NUM_BUFFERS];
    
    ALCdevice* device;
    ALCcontext* context;
    
public:
    AudioManager();
    
    void init();
    void destroy();
    
    void setSourceProperties(const uint8_t ID, const SourceProperties& properties);
    void loadSource(const uint8_t ID, const std::string& filename);
    void stopSource(const uint8_t ID);
    const std::future<bool> playSource(const uint8_t ID);
    
private:
    void enumerateDevice();
    void generateBuffers();
    void generateSources();
    
    void Thread_setSourceProperties(const uint8_t ID, const SourceProperties& properties);
    void Thread_loadSource(const uint8_t ID, const std::string& filename);
    void Thread_playSource(const uint8_t ID, bool& result);
    void Thread_stopSource(const uint8_t ID);
    
    void enqueueTask(const std::function<void()>& task);
    void run();
    
    std::thread audioThread;
    std::atomic<bool> running = false;
    
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable taskCondition;
};

#endif
