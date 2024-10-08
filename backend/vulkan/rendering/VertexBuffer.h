#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "VulkanUtils.h"
#include "DeviceManager.h"
#include "World.h"
#include <array>


class VertexBuffer {
public:
    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    std::vector<uint32_t> vertexOffsets;
    std::vector<uint32_t> indexOffsets;

private:
    VkDeviceMemory vertexBufferMemory;
    VkDeviceMemory indexBufferMemory;
    
    VkCommandPool commandPool;
    DeviceManager* deviceManager;
    
    World* world;
    
public:
    VertexBuffer();
    void init(DeviceManager* d, VkCommandPool cp, World* w);
    void destroy();
    
private:
    void populateBuffers();
    
    void createVertexBuffer();
    void createIndexBuffer();
    
};

#endif
