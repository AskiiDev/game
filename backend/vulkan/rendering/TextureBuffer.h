#ifndef TEXTUREBUFFER_H
#define TEXTUREBUFFER_H


#include "VulkanUtils.h"
#include "DeviceManager.h"
#include "stb_image.h"



class TextureBuffer {
public:
    VkImageView textureImageView;
    VkSampler textureSampler;
    
private:
    DeviceManager deviceManager;
    VkCommandPool commandPool;
    
    uint8_t mipLevels;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    
public:
    TextureBuffer();
    
    void init(DeviceManager d, VkCommandPool cp);
    void destroy();

private:
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    
    void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
    void createTextureImage(const char* filename);
    void createTextureImageView();
    void createTextureSampler();
};

#endif
