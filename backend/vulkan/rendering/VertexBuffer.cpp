#include "VertexBuffer.h"


VertexBuffer::VertexBuffer()
{
}


void VertexBuffer::init(DeviceManager* d, VkCommandPool cp)
{
    deviceManager = d;
    commandPool = cp;
    
    populateBuffers();
    
    createVertexBuffer();
    createIndexBuffer();
}


void VertexBuffer::populateBuffers()
{
    Object obj = loadObject("res/models/viking_room.obj");
    
    
    for (const Vertex& v : obj.vertices)
        vertices.push_back(v);
    
    for (const uint32_t i : obj.indices)
        indices.push_back(i);
    
    std::cout << vertices.size() << std::endl;
    std::cout << indices.size() << std::endl;
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
