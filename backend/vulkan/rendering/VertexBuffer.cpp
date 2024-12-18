#include "VertexBuffer.h"


VertexBuffer::VertexBuffer()
{
}


void VertexBuffer::init(DeviceManager* d, VkCommandPool cp, World* w)
{
    deviceManager = d;
    commandPool = cp;
    world = w;
    
    populateBuffers();
    
    createVertexBuffer();
    createIndexBuffer();
}


void VertexBuffer::populateBuffers()
{
    uint32_t vertexOffset = 0;
    uint32_t indexOffset = 0;

    for (const Actor* a : world->getWorldActors())
    {
        Object obj = a->getObject();

        vertexOffsets.push_back(vertexOffset);
        indexOffsets.push_back(indexOffset);

        vertices.insert(vertices.end(), obj.vertices.begin(), obj.vertices.end());
        indices.insert(indices.end(), obj.indices.begin(), obj.indices.end());

        vertexOffset += static_cast<uint32_t>(obj.vertices.size());
        indexOffset += static_cast<uint32_t>(obj.indices.size());
    }
}


void VertexBuffer::createIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(deviceManager->device, deviceManager->physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(deviceManager->device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(deviceManager->device, stagingBufferMemory);

    createBuffer(deviceManager->device, deviceManager->physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(deviceManager->device, deviceManager->graphicsQueue, commandPool, stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(deviceManager->device, stagingBuffer, nullptr);
    vkFreeMemory(deviceManager->device, stagingBufferMemory, nullptr);
}


void VertexBuffer::createVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(deviceManager->device, deviceManager->physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(deviceManager->device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(deviceManager->device, stagingBufferMemory);

    createBuffer(deviceManager->device, deviceManager->physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
    
    copyBuffer(deviceManager->device, deviceManager->graphicsQueue, commandPool, stagingBuffer, vertexBuffer, bufferSize);
    
    vkDestroyBuffer(deviceManager->device, stagingBuffer, nullptr);
    vkFreeMemory(deviceManager->device, stagingBufferMemory, nullptr);
}


void VertexBuffer::destroy()
{
    vkDestroyBuffer(deviceManager->device, indexBuffer, nullptr);
    vkFreeMemory(deviceManager->device, indexBufferMemory, nullptr);
    
    vkDestroyBuffer(deviceManager->device, vertexBuffer, nullptr);
    vkFreeMemory(deviceManager->device, vertexBufferMemory, nullptr);
}
