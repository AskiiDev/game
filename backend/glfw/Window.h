#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Player.h"
#include "World.h"
#include <chrono>


struct Resolution
{
    uint16_t width;
    uint16_t height;
    uint16_t refreshRate = 60;
};


const int TARGET_FPS = 60;
const std::chrono::duration<double> FRAME_DURATION(1.0 / TARGET_FPS);
extern std::chrono::high_resolution_clock::time_point previousTime;


class Window {
private:
    std::chrono::duration<double> deltaTime;
    
    double deltaX = 0.0, deltaY = 0.0;
    double prevX = 0.0, prevY = 0.0;
    double lastDeltaX = 0, lastDeltaY = 0;

    World* world;
    Player* player;
    
public:
    GLFWwindow* window;
    GLFWmonitor* monitor;
    
    float mouseSensitivity = 0.12f;
    
    bool isFocused = false;
    bool wasFocused = false;
    bool updatingCursor = false;
    
    bool isFullscreen = false;
    Resolution desiredResolution;
    
public:
    Window();
    
    void init(void* pointer, World* w);
    void update();
    void destroy();
    
    bool isRunning() { return !glfwWindowShouldClose(window); };
    std::vector<Resolution> queryResolutions();
    
private:
    static void errorCallback(int error, const char* description);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    
    void updateCursorDelta();
    void handleKeyboardInput(int key, int scancode, int action, int mods);
    
    void toggleFullscreen();
};

#endif
