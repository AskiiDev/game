#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "VulkanUtils.h"
#include "Surface.h"


class DeviceManager {
public:
    VkDevice device;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    
private:
    VkInstance instance;
    Surface surface;
    GLFWwindow* window;
    
public:
    DeviceManager();
    void init(VkInstance i, Surface s, GLFWwindow* w);
    void destroy();
    
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

private:    
    void pickPhysicalDevice();
    void createLogicalDevice();
    
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);

};

#endif
