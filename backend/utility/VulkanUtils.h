#ifndef VULKANUTILS_H
#define VULKANUTILS_H

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>


struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    
    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};


struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


const std::vector<const char*> validationLayers =
{
    "VK_LAYER_KHRONOS_validation"
};


const std::vector<const char*> deviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


bool checkValidationLayerSupport();

void createBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);

void copyBuffer(VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);
void endSingleTimeCommands(VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool, VkCommandBuffer commandBuffer);

VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, uint8_t mipLevels,  VkImageAspectFlags aspectFlags=VK_IMAGE_ASPECT_COLOR_BIT);

void createImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, uint8_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

void transitionImageLayout(VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint8_t mipLevels);

bool hasStencilComponent(VkFormat format);

VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

#endif
