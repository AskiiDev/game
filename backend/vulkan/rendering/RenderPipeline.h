#ifndef RENDERPIPELINE_H
#define RENDERPIPELINE_H

#include "IOUtils.h"
#include "VulkanUtils.h"
#include "DeviceManager.h"
#include "VertexBuffer.h"
#include "TextureBuffer.h"
#include "SwapChain.h"
#include "Player.h"


class RenderPipeline {
private:
    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t currentFrame = 0;
    
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;
    
    DeviceManager* deviceManager;
    SwapChain* swapChain;
    VkDevice device;
    Player* player;
    
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    
    TextureBuffer textureBuffer;
    VertexBuffer vertexBuffer;
    
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    
public:
    RenderPipeline();
    void init(DeviceManager* d, SwapChain* s, Player* p);
    void drawFrame();
    void destroy();
    void destroyDepthBuffer();
    void destroyColorResources();
    
    bool framebufferResized = false;
    
    
    // TEMP
    float lx = 0.f;
    float ly = 0.f;
    
    
private:
    void createRenderPipeline();
    void createRenderPass();
    void createCommandPool();
    
    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();
    
    void createCommandBuffers();
    
    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage);
    
    void createColorResources();
    void createDepthResources();
    
    void createFramebuffers();
    
    void createSyncObjects();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    
    VkShaderModule createShaderModule(const std::vector<char>& code);
    VkSampleCountFlagBits getMaxUsableSampleCount();
    
};

#endif
