#ifndef VULKAN_MANAGER_H
#define VULKAN_MANAGER_H

#include "VulkanUtils.h"
#include "DeviceManager.h"
#include "Surface.h"


class VulkanManager {
private:
    VkInstance instance;
    DeviceManager deviceManager;
    Surface surface;
    
public:
    VulkanManager();
    void init(GLFWwindow* window);
    void destroy(); 
    
private:
    void createInstance();
};

#endif
