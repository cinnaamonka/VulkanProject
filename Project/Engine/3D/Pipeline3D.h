//#pragma once
//#include <vulkan/vulkan.h>
//#include "./Engine/3D/GP2Shader3D.h"
//#include "./Engine/CommandBuffer.h"
//#include "./Engine/GraphicsPipeline3D.h"
//#include "./Engine/CommandPool.h"
//#include "./Engine/RenderPass.h"
//#include "./Engine/3D/Mesh3D.h"
//#include "./Engine/Scene.h"
//
//class Pipeline3D
//{
//public:
//    Pipeline3D();
//    ~Pipeline3D() = default;
//
//   Pipeline3D(const Pipeline3D& other) = delete;
//   Pipeline3D& operator=(const Pipeline3D& other) = delete;
//   Pipeline3D(Pipeline3D&& other) = delete;
//   Pipeline3D& operator=(Pipeline3D&& other) = delete;
//
//    void Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkFormat& swapChainImageFormat,
//        std::vector<VkImageView>& swapChainImageViews,
//        const VkExtent2D& swapChainExtent, const QueueFamilyIndices& queueFamilyIndexes,
//        const VkQueue& graphicsQueue);
//
//    void DestroyPipeline(const VkDevice& device);
//    void DestroyMeshes(const VkDevice device);
//    void DestroyUniformBuffers(const VkDevice device);
//
//    void Record(const VkExtent2D& swapChainExtent, uint32_t imageIndex);
//
//    const VulkanCommandBuffer& GetCommandBuffer() const
//    {
//        return m_CommandBuffer;
//    }
//private:
//    void DrawScene(const VkExtent2D& swapChainExtent, uint32_t imageIndex);
//private:
//    GraphicsPipeline3D m_GraphicsPipeline;
//    GP2Shader3D m_Shader;
//    VulkanCommandBuffer m_CommandBuffer;
//    RenderPass m_RenderPass;
//    CommandPool m_CommandPool;
//    std::vector<Mesh2D> m_Meshes;
//    Scene m_Scene;
//};
