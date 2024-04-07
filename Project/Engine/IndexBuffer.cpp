#include "IndexBuffer.h"
#include "BaseBuffer.h"


void IndexBuffer::CreateIndexBuffer(const std::vector<uint16_t>& indices,const VkDevice& device,
    const CommandPool& commandPool,const VkQueue& graphicsQueue, const VkPhysicalDevice& physicalDevice,
    VkBuffer& indexBuffer,VkDeviceMemory& deviceMemory)
{
    
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    BaseBuffer::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        stagingBuffer, stagingBufferMemory,device,physicalDevice);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    BaseBuffer::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        indexBuffer, deviceMemory,device,physicalDevice);

    BaseBuffer::CopyBuffer(stagingBuffer, indexBuffer, bufferSize,device,commandPool,graphicsQueue);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void IndexBuffer::CreateIndexBuffer(const std::vector<uint32_t>& indices, const VkDevice& device,
    const CommandPool& commandPool, const VkQueue& graphicsQueue, const VkPhysicalDevice& physicalDevice,
    VkBuffer& indexBuffer, VkDeviceMemory& deviceMemory)
{

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    BaseBuffer::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory, device, physicalDevice);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    BaseBuffer::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        indexBuffer, deviceMemory, device, physicalDevice);

    BaseBuffer::CopyBuffer(stagingBuffer, indexBuffer, bufferSize, device, commandPool, graphicsQueue);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}


void IndexBuffer::DestroyIndexBuffer(const VkDevice& device,const VkBuffer& indexBuffer, VkDeviceMemory& deviceMemory)
{
    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkFreeMemory(device, deviceMemory, nullptr);
}
