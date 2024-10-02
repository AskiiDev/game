#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "VulkanUtils.h"
#include "DeviceManager.h"
#include <GLFW/glfw3.h>
#include "Window.h"


class SwapChain {
public:
    VkSwapchainKHR swapChain;
    VkExtent2D swapChainExtent;
    VkFormat swapChainImageFormat;
    
    std::vector<VkFramebuffer> swapChainFramebuffers;
    std::vector<VkImageView> swapChainImageViews;
    
private:
    DeviceManager* deviceManager;
    Window* window;
    VkSurfaceKHR surface;
    
    std::vector<VkImage> swapChainImages;
    
public:
    SwapChain();
    void init(VkSurfaceKHR s, Window* w, DeviceManager* d);
    void destroy();
    
    void createSwapChain();
    void createImageViews();
    void reconstructChain();

private:
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    
};

#endif
