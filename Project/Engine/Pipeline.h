#pragma once
#include <vulkan/vulkan.h>
#include "GP2Shader.h"
#include "CommandBuffer.h"
#include "GraphicsPipeline.h"
#include "CommandPool.h"
#include "RenderPass.h"
#include "Mesh.h"
#include "Scene.h"
#include "Rect.h"
#include "Oval.h"
#include "RoundedRect.h"

class Pipeline
{
public:
    Pipeline();
    ~Pipeline() = default;

   Pipeline(const Pipeline& other) = delete;
   Pipeline& operator=(const Pipeline& other) = delete;
   Pipeline(Pipeline&& other) = delete;
   Pipeline& operator=(Pipeline&& other) = delete;

   void Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkFormat& swapChainImageFormat,
       std::vector<VkImageView>& swapChainImageViews,
       const VkExtent2D& swapChainExtent,const QueueFamilyIndices& queueFamilyIndexes,
       const VkQueue& graphicsQueue);

   void DestroyPipeline(const VkDevice& device);
   void DestroyMeshes(const VkDevice device);

   void Record(const VkExtent2D& swapChainExtent, uint32_t imageIndex);

   const VulkanCommandBuffer& GetCommandBuffer() const
   {
       return m_CommandBuffer;
   }
private:
    void DrawScene(const VkExtent2D& swapChainExtent, uint32_t imageIndex);
private:
    GraphicsPipeline m_GraphicsPipeline;
    GP2Shader m_Shader; 
    VulkanCommandBuffer m_CommandBuffer;
    RenderPass m_RenderPass;
    CommandPool m_CommandPool;
    std::vector<Mesh> m_Meshes;
    Scene m_Scene;
};
