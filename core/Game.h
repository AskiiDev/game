#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "VulkanManager.h"
#include "AudioManager.h"
#include "Player.h"
#include "World.h"


class Game {
public:
    World world;
    VulkanManager vkManager;
    Window gameWindow;
    AudioManager audioManager;
    
public:
    Game();
    void run();

private:
    void initWindow();
    void initVulkan();
    void initAudio();
    void mainLoop();
    void cleanUp();
};

#endif
