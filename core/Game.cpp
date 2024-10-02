#include "Game.h"



Game::Game()
{
}


void Game::run()
{
    player.init();
    
    initWindow();
    initVulkan();
    
    initAudio();
//    audioManager.playFromSource(0, "res/sfx/ui/startup.wav");
    
    mainLoop();
    cleanUp();
}


void Game::initWindow()
{
    gameWindow.init(this, &player);
}


void Game::initVulkan()
{
    vkManager.init(&gameWindow, &player);
}


void Game::initAudio()
{
    audioManager.init();
}


void Game::mainLoop()
{
    while (gameWindow.isRunning())
    {
        gameWindow.update();
        vkManager.draw();
    }
}


void Game::cleanUp()
{
    audioManager.destroy();
    
    vkManager.idle();
    vkManager.destroy();
    gameWindow.destroy();
}
