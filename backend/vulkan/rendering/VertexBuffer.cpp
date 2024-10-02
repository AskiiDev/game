#include "VertexBuffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


VertexBuffer::VertexBuffer()
{
}


void VertexBuffer::init(DeviceManager d, VkCommandPool cp)
{
    deviceManager = d;
    commandPool = cp;
    
    loadModel();
    createVertexBuffer();
    createIndexBuffer();
}


void VertexBuffer::loadModel()
{
    vertices = {
        // Front face
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 1}, // Bottom-left
        {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, 1},  // Bottom-right
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, 1},   // Top-right
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 1},  // Top-left

        // Back face (reverse winding)
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 0},  // Top-left
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, 0},   // Top-right
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, 0},  // Bottom-right
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0}, // Bottom-left

        // Left face
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 2}, // Bottom-left
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, 2},  // Bottom-right
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, 2},   // Top-right
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 2},  // Top-left

        // Right face
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0},   // Bottom-left
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, 0},  // Bottom-right
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, 0},   // Top-right
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 0},    // Top-left

        // Top face (reverse winding)
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0},   // Bottom-left
        {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, 0},    // Bottom-right
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, 0},   // Top-right
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 0},  // Top-left

        // Bottom face (reverse winding)
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0}, // Bottom-left
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, 0},  // Bottom-right
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, 0},   // Top-right
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 0}   // Top-left
    };

    indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20
    };
//    tinyobj::attrib_t attrib;
//    std::vector<tinyobj::shape_t> shapes;
//    std::vector<tinyobj::material_t> materials;
//    std::string warn, err;
//
//    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "res/models/viking_room.obj"))
//    {
//        throw std::runtime_error(warn + err);
//    }
//    
//    
//    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
//    
//    for (const auto& shape : shapes)
//    {
//        for (const auto& index : shape.mesh.indices)
//        {
//            Vertex vertex{};
//            vertex.pos = {
//                attrib.vertices[3 * index.vertex_index + 0],
//                attrib.vertices[3 * index.vertex_index + 1],
//                attrib.vertices[3 * index.vertex_index + 2]
//            };
//            vertex.texCoord = {
//                attrib.texcoords[2 * index.texcoord_index + 0],
//                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
//            };
//            vertex.color = {1.0f, 1.0f, 1.0f};
//            vertex.texIndex = 0;
//
//            if (uniqueVertices.count(vertex) == 0)
//            {
//                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
//                vertices.push_back(vertex);
//            }
//
//            indices.push_back(uniqueVertices[vertex]);
//        }
//    }
    
//    std::cout << vertices.size() << std::endl;
}


void VertexBuffer::createIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(deviceManager.device, deviceManager.physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(deviceManager.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(deviceManager.device, stagingBufferMemory);

    createBuffer(deviceManager.device, deviceManager.physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(deviceManager.device, deviceManager.graphicsQueue, commandPool, stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(deviceManager.device, stagingBuffer, nullptr);
    vkFreeMemory(deviceManager.device, stagingBufferMemory, nullptr);
}


void VertexBuffer::createVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(deviceManager.device, deviceManager.physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(deviceManager.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(deviceManager.device, stagingBufferMemory);

    createBuffer(deviceManager.device, deviceManager.physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
    
    copyBuffer(deviceManager.device, deviceManager.graphicsQueue, commandPool, stagingBuffer, vertexBuffer, bufferSize);
    
    vkDestroyBuffer(deviceManager.device, stagingBuffer, nullptr);
    vkFreeMemory(deviceManager.device, stagingBufferMemory, nullptr);
}


void VertexBuffer::destroy()
{
    vkDestroyBuffer(deviceManager.device, indexBuffer, nullptr);
    vkFreeMemory(deviceManager.device, indexBufferMemory, nullptr);
    
    vkDestroyBuffer(deviceManager.device, vertexBuffer, nullptr);
    vkFreeMemory(deviceManager.device, vertexBufferMemory, nullptr);
}
