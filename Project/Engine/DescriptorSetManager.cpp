#include "DescriptorSetManager.h"
#include <stdexcept>

VkDescriptorSetLayoutBinding DescriptorSetManager::m_UboLayoutBinding;

void DescriptorSetManager::CreateDescriptorSetLayout(const VkDevice& device,
    VkDescriptorSetLayout& descriptorSetLayout)
{
    m_UboLayoutBinding.binding = 0;
    m_UboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    m_UboLayoutBinding.descriptorCount = 1;
    m_UboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    m_UboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &m_UboLayoutBinding;

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}
