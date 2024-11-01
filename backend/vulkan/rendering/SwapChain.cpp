#include "SwapChain.h"


SwapChain::SwapChain()
{
}


void SwapChain::init(VkSurfaceKHR s, Window* w, DeviceManager* d)
{
    window = w;
    surface = s;
    deviceManager = d;
    
    createSwapChain();
    createImageViews();
}


void SwapChain::reconstructChain()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(window->window, &width, &height);
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(window->window, &width, &height);
        glfwWaitEvents();
    }
    
    destroy();
}


void SwapChain::createSwapChain()
{
    SwapChainSupportDetails swapChainSupport = deviceManager->querySwapChainSupport(deviceManager->physicalDevice);
    
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
//    
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }
    
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    QueueFamilyIndices indices = deviceManager->findQueueFamilies(deviceManager->physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }
    
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    
    if (vkCreateSwapchainKHR(deviceManager->device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }
    
    vkGetSwapchainImagesKHR(deviceManager->device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(deviceManager->device, swapChain, &imageCount, swapChainImages.data());
    
    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    // VK_PRESENT_MODE_FIFO_KHR for V-Sync
    // VK_PRESENT_MODE_IMMEDIATE_KHR for uncapped
    return VK_PRESENT_MODE_FIFO_KHR;
}


VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}


VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    int width = window->desiredResolution.width;
    int height = window->desiredResolution.height;
    
    VkExtent2D actualExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };
    
    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    
//    std::cout << actualExtent.width << std::endl;
    return actualExtent;
}


void SwapChain::createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());
    VkDevice device = deviceManager->device;
    
    for (uint32_t i = 0; i < swapChainImages.size(); i++)
        swapChainImageViews[i] = createImageView(device, swapChainImages[i], swapChainImageFormat, 1);
}


void SwapChain::destroy()
{
    
    for (auto framebuffer : swapChainFramebuffers)
        vkDestroyFramebuffer(deviceManager->device, framebuffer, nullptr);
    
    for (auto imageView : swapChainImageViews)
        vkDestroyImageView(deviceManager->device, imageView, nullptr);
    
    swapChainImageViews.clear();
    swapChainFramebuffers.clear();
    
    vkDestroySwapchainKHR(deviceManager->device, swapChain, nullptr);
}
