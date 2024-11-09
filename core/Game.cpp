#include "Game.h"

#include <thread>
#include <chrono>


Game::Game()
{
}


void Game::run()
{
    world.load();
    
    initWindow();
    initVulkan();
    
    initAudio();
//    audioManager.playFromSource(0, "res/sfx/ui/startup.wav");
    
    mainLoop();
    cleanUp();
}


void Game::initWindow()
{
    gameWindow.init(this, &world);
}


void Game::initVulkan()
{
    vkManager.init(&gameWindow, &world);
}


void Game::initAudio()
{
    audioManager.init();
}


void Game::mainLoop()
{
    while (glfwGetWindowAttrib(gameWindow.window, GLFW_VISIBLE) == GLFW_FALSE ||
           glfwGetWindowAttrib(gameWindow.window, GLFW_FOCUSED) == GLFW_FALSE)
    {
        glfwPollEvents();
    }
    
    
    gameWindow.resetUpdateTimer();
    
    while (!glfwWindowShouldClose(gameWindow.window))
    {
        gameWindow.update();
        vkManager.draw();
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


void Game::cleanUp()
{
    audioManager.destroy();
    
    vkManager.idle();
    vkManager.destroy();
    gameWindow.destroy();
}
