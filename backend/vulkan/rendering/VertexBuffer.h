#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "VulkanUtils.h"
#include "DeviceManager.h"
#include <array>


class VertexBuffer {
public:
    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

private:
    VkDeviceMemory vertexBufferMemory;
    VkDeviceMemory indexBufferMemory;
    
    VkCommandPool commandPool;
    DeviceManager* deviceManager;
    
public:
    VertexBuffer();
    void init(DeviceManager* d, VkCommandPool cp);
    void destroy();
    
private:
    void populateBuffers();
    
    void createVertexBuffer();
    void createIndexBuffer();
    
};

#endif
