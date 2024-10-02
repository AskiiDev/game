#include "Window.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "Game.h"


std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();


Window::Window()
{
}


void Window::init(void* pointer, Player* p)
{
    player = p;
    glfwInit();
    
    monitor = glfwGetPrimaryMonitor();
    
    std::vector<std::array<int, 3>> resolutions = queryResolutions();
//    for (auto a : resolutions)
//    {
//        printf("%dx%d\n", a[0], a[1]);
//    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
//    desiredResolution = resolutions.back();
    desiredResolution = {800, 600, 60};
    
    window = glfwCreateWindow(desiredResolution[0], desiredResolution[1], "game", nullptr, nullptr);
    
    glfwSetErrorCallback(errorCallback);
    
    glfwSetWindowUserPointer(window, pointer);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    
    game = pointer;
    
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}


void Window::update()
{
    glfwPollEvents();
    
    if (!isFocused)
    {
        player->mvDirection = 0;
    }
    
    updateCursorDelta();
//    printf("%f, %f\n", (float) deltaX, (float) deltaY);
    player->updatePlayerMovement(std::chrono::duration<float>(deltaTime).count());
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = currentTime - previousTime;
    
//    if (deltaTime < FRAME_DURATION)
//    {
//        auto sleepTime = FRAME_DURATION - deltaTime;
//        std::this_thread::sleep_for(sleepTime);
//    }

    previousTime = std::chrono::high_resolution_clock::now();
}


void Window::destroy()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}


void Window::updateCursorDelta()
{
    if (!isFocused)
    {
        deltaX = 0;
        deltaY = 0;
        return;
    }
    
    updatingCursor = true;
    
    if (wasFocused)
    {
        deltaX = 0;
        deltaY = 0;
        glfwSetCursorPos(window, prevX, prevY);
        wasFocused = false;
//        return;
    }
    
    double currX, currY;

    glfwGetCursorPos(window, &currX, &currY);
    deltaX = (currX - prevX) * mouseSensitivity;
    deltaY = (currY - prevY) * mouseSensitivity;
    
    const double deadZone = 0.1;
    if (std::abs(deltaX) < deadZone) deltaX = 0;
    if (std::abs(deltaY) < deadZone) deltaY = 0;
    
    // Apply smoothing
    const float smoothFactor = 0.5f;
    deltaX = deltaX * smoothFactor + lastDeltaX * (1 - smoothFactor);
    deltaY = deltaY * smoothFactor + lastDeltaY * (1 - smoothFactor);
    
    lastDeltaX = deltaX;
    lastDeltaY = deltaY;
        
    prevX = currX;
    prevY = currY;
    
    player->camera.yaw += deltaX;
    player->camera.pitch = std::clamp(player->camera.pitch - deltaY, -87., 80.);
    player->updateCameraVectors();
    
    updatingCursor = false;
}


void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1)
    {
        auto g = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
        
        while (g->gameWindow.updatingCursor)
            continue;
        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        g->gameWindow.isFocused = true;
        g->gameWindow.wasFocused = true;
    }
}


void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
    game->gameWindow.handleKeyboardInput(key, scancode, action, mods);
}


void Window::toggleFullscreen()
{
    if (!isFullscreen)
    {
        glfwSetWindowMonitor(window, monitor, 0, 0, desiredResolution[0], desiredResolution[1], desiredResolution[2]);
        //                glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    }
    else
    {
        glfwSetWindowMonitor(window, NULL, 100, 100, desiredResolution[0], desiredResolution[1], 0);
        //                glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    }
    
    isFullscreen = !isFullscreen;
}


void Window::handleKeyboardInput(int key, int scancode, int action, int mods)
{
    if (!isFocused)
        return;
    
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            while (updatingCursor)
                continue;
            
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            isFocused = false;
        }
        
        if (key == GLFW_KEY_SPACE)
        {
//            toggleFullscreen();
        }
                
        if (key == GLFW_KEY_W)
            player->mvDirection |= MV_FORWARD;
            
        if (key == GLFW_KEY_S)
            player->mvDirection |= MV_BACKWARD;

        if (key == GLFW_KEY_A)
            player->mvDirection |= MV_LEFT;
                
        if (key == GLFW_KEY_D)
            player->mvDirection |= MV_RIGHT;
    }
    else if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_W)
            player->mvDirection &= ~MV_FORWARD;
            
        if (key == GLFW_KEY_S)
            player->mvDirection &= ~MV_BACKWARD;

        if (key == GLFW_KEY_A)
            player->mvDirection &= ~MV_LEFT;
                
        if (key == GLFW_KEY_D)
            player->mvDirection &= ~MV_RIGHT;
    }
}


void Window::errorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error: " << description << std::endl;
}


void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
    game->vkManager.renderPipeline.framebufferResized = true;
}


std::vector<std::array<int, 3>> Window::queryResolutions()
{
    std::vector<std::array<int, 3>> resolutions;
    
    int count;
    const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);
    
    float xScale, yScale;
    glfwGetMonitorContentScale(monitor, &xScale, &yScale);
    
//    printf("%fx%f\n", xScale, yScale);

    for (int i = 0; i < count; i++)
        resolutions.push_back( {(int) (modes[i].width / xScale), (int) (modes[i].height / yScale), modes[i].refreshRate} );
    
    return resolutions;
}

