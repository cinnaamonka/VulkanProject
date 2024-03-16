#include "Mesh.h"
#include "BaseBuffer.h"
#include "CommandPool.h"
#include "VertexBuffer.h"

void Mesh::DestroyMesh(const VkDevice& device)
{

    vkDestroyBuffer(device, m_VkBuffer, nullptr);
    vkFreeMemory(device, m_VkDeviceMemory, nullptr);
}

void Mesh::Draw(const VkCommandBuffer& buffer) const
{
    VkBuffer vertexBuffers[] = { m_VkBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);
    vkCmdDraw(buffer, static_cast<uint32_t>(m_Vertices.size()), 1, 0, 0);
}
void Mesh::AddVertex(glm::vec2 pos, glm::vec3 color)
{
    m_Vertices.push_back(Vertex{ pos, color });
}

void Mesh::Initialize(const VkPhysicalDevice& physicalDevice, const VkDevice& device, const std::vector<Vertex> vertexes, const VkQueue& graphicsQueue, const CommandPool& commandPool)
{
    VkDeviceSize bufferSize = sizeof(vertexes[0]) * vertexes.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

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
}



