#pragma once
#include <vulkan/vulkan.h>
#include "./Engine/2D/GP2Shader2D.h"
#include "./Engine/CommandBuffer.h"
#include "./Engine/GraphicsPipeline.h"
#include "./Engine/CommandPool.h"
#include "./Engine/RenderPass.h"
#include "./Engine/2D/Mesh2D.h"
#include "./Engine/Scene.h"
#include "../Meshes/Rect.h"
#include "../Meshes/Oval.h"
#include "../Meshes/RoundedRect.h"
#include "../DepthBuffer.h"

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
       const VkQueue& graphicsQueue, CommandPool& commandPool, ImageManager& imageManager,DepthBuffer& depthBuffer);

   void DestroyPipeline(const VkDevice& device, CommandPool& commandPool);
   void DestroyMeshes(const VkDevice device);
   void DestroyUniformBuffers(const VkDevice& device);

   void Record(const VkExtent2D& swapChainExtent, uint32_t imageIndex);

   VulkanCommandBuffer& GetCommandBuffer()
   {
       return m_CommandBuffer;
   }
   GraphicsPipeline& GetGraphicsPipeline()
   {
       return m_GraphicsPipeline;
   }
private:
    void DrawScene(const VkExtent2D& swapChainExtent, uint32_t imageIndex);
private:
    GraphicsPipeline m_GraphicsPipeline;
    GP2Shader m_Shader; 
    VulkanCommandBuffer m_CommandBuffer;
    RenderPass m_RenderPass;
 
    std::vector<Mesh2D> m_Meshes;
    Scene m_Scene;
};
