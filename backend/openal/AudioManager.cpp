#include "AudioManager.h"

#include <chrono>
#include <thread>


AudioManager::AudioManager()
{    
}


void AudioManager::init()
{
    enumerateDevice();
    generateBuffers();
    generateSources();
}


void AudioManager::enumerateDevice()
{
    device = alcOpenDevice(nullptr);
    if (!device)
        throw std::runtime_error("failed to open file");
    
    context = alcCreateContext(device, nullptr);
    if (!context)
    {
        alcCloseDevice(device);
        throw std::runtime_error("failed to create context");
    }
    
    alcMakeContextCurrent(context);
}


void AudioManager::playFromSource(uint8_t iD, const std::string& filename)
{
    loadPCM(iD, filename);
    
    alSourcei(sources[iD], AL_BUFFER, buffers[iD]);
    alSourcePlay(sources[iD]);
}



void AudioManager::generateBuffers()
{
    alGetError();
    alGenBuffers(NUM_BUFFERS, buffers);
    alGetError();
}


void AudioManager::generateSources()
{
    alGetError();
    alGenSources(NUM_SOURCES, sources);
    alGetError();
}


void AudioManager::loadPCM(uint8_t iD, const std::string& filename)
{
    std::vector<char> fileData = readFile(filename);
    WAVHeader* header = reinterpret_cast<WAVHeader*>(fileData.data());
    
    if (std::string(header->riff, 4) != "RIFF" || std::string(header->wave, 4) != "WAVE")
        throw std::runtime_error("invalid wav");
    
    
    ALenum format;
    switch (header->bitsPerSample)
    {
        case 8:
            format = (header->numChannels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
            break;
        case 16:
            format = (header->numChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
            break;
        default:
            throw std::runtime_error("unsupported wav type");
    }
    
    alBufferData(buffers[iD], format, fileData.data() + sizeof(WAVHeader), header->dataSize, header->sampleRate);
}


void AudioManager::destroy()
{
    ALCcontext* c = alcGetCurrentContext();
    ALCdevice* d = alcGetContextsDevice(c);
    
    alDeleteSources(8, sources);
    alDeleteBuffers(12, buffers);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(c);
    alcCloseDevice(d);
}
