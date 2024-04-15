#include "Mesh2D.h"
#include "../BaseBuffer.h"
#include "../CommandPool.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <cstring>

Mesh2D::Mesh2D():
    m_Vertices{},
    m_Indexes{},
    m_VertexConstant{},
    m_MaxFramesInFlight(3),
    m_UniformBuffersMapped{},
    m_UniformBuffersMemory{}
{

}
void Mesh2D::DestroyMesh(const VkDevice& device)
{
    m_IndexBuffer->Destroy();
    m_VertexBuffer->Destroy();
}

void Mesh2D::AddVertex(glm::vec2 pos, glm::vec3 color)
{
    m_Vertices.push_back(Vertex{ pos, color });
}

void Mesh2D::Initialize(const VkPhysicalDevice& physicalDevice, const VkDevice& device, const std::vector<Vertex> vertexes,
    const VkQueue& graphicsQueue, const CommandPool& commandPool, std::vector<uint16_t> indices)
{
    m_Indexes = indices;

    VkDeviceSize bufferSize = sizeof(Vertex) * vertexes.size();

    m_VertexBuffer = std::make_unique<DAEDataBuffer>(physicalDevice, device,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize);

    m_IndexBuffer = std::make_unique<DAEDataBuffer>(physicalDevice, device,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize);

    VkBuffer stagingBuffer{};
    VkDeviceMemory stagingBufferMemory{};

    VertexBuffer::CreateVertexBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory, device, physicalDevice);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertexes.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    m_VertexBuffer->Destroy();
    m_IndexBuffer->Destroy();
    VertexBuffer::CreateVertexBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer->GetVkBuffer(), m_VertexBuffer->GetDeviceMemory(), device, physicalDevice);

    BaseBuffer::CopyBuffer(stagingBuffer, m_VertexBuffer->GetVkBuffer(), bufferSize, device, commandPool, graphicsQueue);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

    IndexBuffer::CreateIndexBuffer(indices, device, commandPool, graphicsQueue, physicalDevice,
        m_IndexBuffer->GetVkBuffer(), m_IndexBuffer->GetDeviceMemory());

    CreateUniformBuffers(device, physicalDevice);
}

void Mesh2D::Draw(const VkPipelineLayout& pipelineLayout, const VkCommandBuffer& commandBuffer)
{
    m_VertexBuffer->BindAsVertexBuffer(commandBuffer);
    m_IndexBuffer->BindAsIndexBuffer(commandBuffer);
    vkCmdPushConstants(
        commandBuffer,
        pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(MeshData),
        &m_VertexConstant);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indexes.size()), 1, 0, 0, 0);
}

void Mesh2D::CreateUniformBuffers(const VkDevice& device, const VkPhysicalDevice& physcialDevice)
{
    VkDeviceSize bufferSize = sizeof(VertexUBO);

    m_UniformBuffers.resize(m_MaxFramesInFlight);
    m_UniformBuffersMemory.resize(m_MaxFramesInFlight);
    m_UniformBuffersMapped.resize(m_MaxFramesInFlight);

    for (size_t i = 0; i < m_MaxFramesInFlight; i++)
    {
        BaseBuffer::CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            m_UniformBuffers[i], m_UniformBuffersMemory[i], device, physcialDevice);

        vkMapMemory(device, m_UniformBuffersMemory[i], 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
    }
}

void Mesh2D::DestroyUniformBuffers(const VkDevice& device)
{
    for (size_t i = 0; i < m_MaxFramesInFlight; i++)
    {
        vkDestroyBuffer(device, m_UniformBuffers[i], nullptr);
        vkFreeMemory(device, m_UniformBuffersMemory[i], nullptr);
    }
}




