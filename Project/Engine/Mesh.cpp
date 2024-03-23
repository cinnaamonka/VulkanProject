#include "Mesh.h"
#include "BaseBuffer.h"
#include "CommandPool.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

Mesh::Mesh():
    m_Indexes{},
    m_VkBuffer{},
    m_MaxFramesInFlight(3)
{

}
void Mesh::DestroyMesh(const VkDevice& device, const VkDescriptorSetLayout& layout)
{
    IndexBuffer::DestroyIndexBuffer(device,indexBuffer, indexBufferMemory);

    vkDestroyBuffer(device, m_VkBuffer, nullptr);
    vkFreeMemory(device, m_VkDeviceMemory, nullptr);

    //DestroyUniformBuffers(device, layout);
}

void Mesh::Draw(const VkCommandBuffer& buffer) const
{
    VkBuffer vertexBuffers[] = { m_VkBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(buffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    vkCmdDrawIndexed(buffer, static_cast<uint32_t>(m_Indexes.size()), 1, 0, 0, 0);
}
void Mesh::AddVertex(glm::vec2 pos, glm::vec3 color)
{
    m_Vertices.push_back(Vertex{ pos, color });
}

void Mesh::Initialize(const VkPhysicalDevice& physicalDevice, const VkDevice& device, const std::vector<Vertex> vertexes,
    const VkQueue& graphicsQueue, const CommandPool& commandPool, std::vector<uint16_t> indices)
{
    m_Indexes = indices;

    VkDeviceSize bufferSize = sizeof(vertexes[0]) * vertexes.size();

    VkBuffer stagingBuffer{};
    VkDeviceMemory stagingBufferMemory{};

    VertexBuffer::CreateVertexBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory, device, physicalDevice);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertexes.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    VertexBuffer::CreateVertexBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        m_VkBuffer, m_VkDeviceMemory, device, physicalDevice);

    BaseBuffer::CopyBuffer(stagingBuffer, m_VkBuffer, bufferSize, device, commandPool, graphicsQueue);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

    IndexBuffer::CreateIndexBuffer(indices, device, commandPool, graphicsQueue, physicalDevice,indexBuffer,indexBufferMemory);
    CreateUniformBuffers(device, physicalDevice);
}

void Mesh::CreateUniformBuffers(const VkDevice& device, const VkPhysicalDevice& physcialDevice)
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(m_MaxFramesInFlight);
    uniformBuffersMemory.resize(m_MaxFramesInFlight);
    uniformBuffersMapped.resize(m_MaxFramesInFlight);

    for (size_t i = 0; i < m_MaxFramesInFlight; i++)
    {
        BaseBuffer::CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            uniformBuffers[i], uniformBuffersMemory[i],device,physcialDevice);

        vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}

void Mesh::DestroyUniformBuffers(const VkDevice& device, const VkDescriptorSetLayout& descriptiveSetLayout)
{
    for (size_t i = 0; i < m_MaxFramesInFlight; i++)
    {
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }
   
}

void Mesh::UpdateUniformBuffer(uint32_t currentImage, UniformBufferObject& buffer, const VkExtent2D& swapChainExtent)
{
    static auto startTime = std::chrono::high_resolution_clock::now(); 

    auto currentTime = std::chrono::high_resolution_clock::now(); 
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count(); 

    buffer.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    buffer.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    buffer.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);

    buffer.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentImage], &buffer, sizeof(buffer));
}



