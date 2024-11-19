#include "AudioManager.h"
#include <iostream>


AudioManager::AudioManager()
{    
}

void AudioManager::init()
{
    enumerateDevice();
    generateBuffers();
    generateSources();
    
    running = true;
    audioThread = std::thread(&AudioManager::run, this);
}

void AudioManager::enumerateDevice()
{
    device = alcOpenDevice(nullptr);
    if (!device)
    {
        throw std::runtime_error("failed to open file");
    }
    
    context = alcCreateContext(device, nullptr);
    if (!context)
    {
        alcCloseDevice(device);
        throw std::runtime_error("failed to create context");
    }
    
    alcMakeContextCurrent(context);
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

void AudioManager::enqueueTask(const std::function<void()>& task)
{
    std::lock_guard<std::mutex> lock(queueMutex);
    taskQueue.push(task);
    taskCondition.notify_one();
}


/*-------------------------------------------------*/

void AudioManager::Thread_setSourceProperties(const uint8_t ID, const SourceProperties& properties)
{
    alSourcef(sources[ID], AL_PITCH, properties.pitch);
    alSourcef(sources[ID], AL_GAIN, properties.gain);
    alSourcei(sources[ID], AL_LOOPING, properties.looping);
}

void AudioManager::Thread_playSource(const uint8_t ID, bool& result)
{
    ALint sourceState;
    alGetSourcei(sources[ID], AL_SOURCE_STATE, &sourceState);
    
    if (sourceState == AL_PLAYING)
    {
        result = false;
        return;
    }
    
    alSourcei(sources[ID], AL_BUFFER, buffers[ID]);
    alSourcePlay(sources[ID]);
    
    result = true;
}

void AudioManager::Thread_stopSource(const uint8_t ID)
{
    ALint sourceState;
    alGetSourcei(sources[ID], AL_SOURCE_STATE, &sourceState);
    
    if (sourceState != AL_PLAYING)
    {
        return;
    }
    
    alSourceStop(sources[ID]);
}

void AudioManager::Thread_loadSource(const uint8_t ID, const std::string& filename)
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
    
    alBufferData(buffers[ID], format, fileData.data() + sizeof(WAVHeader), header->dataSize, header->sampleRate);
}

/*-------------------------------------------------*/


void AudioManager::setSourceProperties(const uint8_t ID, const SourceProperties& properties)
{
    enqueueTask([this, ID, properties]()
    {
        Thread_setSourceProperties(ID, properties);
    });
}

void AudioManager::loadSource(const uint8_t ID, const std::string& filename)
{
    enqueueTask([this, ID, filename]()
    {
        Thread_loadSource(ID, filename);
    });
}

const std::future<bool> AudioManager::playSource(const uint8_t ID)
{
    auto promise = std::make_shared<std::promise<bool>>();
    
    enqueueTask([this, ID, promise]()
    {
        bool taskResult = false;
        Thread_playSource(ID, taskResult);
        promise->set_value(taskResult);
    });
    
    return promise->get_future();
}

void AudioManager::stopSource(const uint8_t ID)
{
    enqueueTask([this, ID]()
    {
        Thread_stopSource(ID);
    });
}

void AudioManager::destroy()
{
    if (running)
    {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            running = false;
        }
        taskCondition.notify_all();
        if (audioThread.joinable())
            audioThread.join();
    }

    ALCcontext* c = alcGetCurrentContext();
    ALCdevice* d = alcGetContextsDevice(c);
    
    alDeleteSources(8, sources);
    alDeleteBuffers(12, buffers);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(c);
    alcCloseDevice(d);
}

void AudioManager::run()
{
    while (running)
   {
       std::function<void()> task;

       {
           std::unique_lock<std::mutex> lock(queueMutex);
           
           taskCondition.wait(lock, [this]() { return !taskQueue.empty() || !running; });

           if (!running && taskQueue.empty())
               return;

           task = std::move(taskQueue.front());
           taskQueue.pop();
       }
       
       task();
   }
}
