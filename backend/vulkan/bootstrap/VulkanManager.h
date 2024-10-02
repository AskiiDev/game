#ifndef VULKAN_MANAGER_H
#define VULKAN_MANAGER_H

#include "VulkanUtils.h"
#include "DeviceManager.h"
#include "RenderPipeline.h"
#include "SwapChain.h"
#include "Window.h"
#include "Surface.h"
#include "Player.h"


class VulkanManager {
public:
    RenderPipeline renderPipeline;
    Surface surface;
    
private:
    VkInstance instance;
    DeviceManager deviceManager;
    
    SwapChain swapChain;
    
public:
    VulkanManager();
    void init(Window* window, Player* p);
    void destroy();
    void draw();
    void idle();
    
private:
    void createInstance();
};

#endif
