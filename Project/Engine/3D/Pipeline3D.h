#pragma once
#include <vulkan/vulkan.h>
#include "./Engine/3D/GP2Shader3D.h"
#include "./Engine/CommandBuffer.h"
#include "./Engine/GraphicsPipeline3D.h"
#include "./Engine/CommandPool.h"
#include "./Engine/RenderPass.h"
#include "./Engine/3D/Mesh3D.h"
#include "./Engine/Scene.h"
#include "./Engine/DepthBuffer.h"

class Pipeline3D
{
public:
	Pipeline3D();
	~Pipeline3D() = default;

	Pipeline3D(const Pipeline3D& other) = delete;
	Pipeline3D& operator=(const Pipeline3D& other) = delete;
	Pipeline3D(Pipeline3D&& other) = delete;
	Pipeline3D& operator=(Pipeline3D&& other) = delete;

	void Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkFormat& swapChainImageFormat,
		std::vector<VkImageView>& swapChainImageViews,
		const VkExtent2D& swapChainExtent, const QueueFamilyIndices& queueFamilyIndexes,
		const VkQueue& graphicsQueue,CommandPool& commandPool, Mesh3D& mesh, Mesh3D& model, 
		ImageManager& imageManager, DepthBuffer& depthBuffer);
	
	    void DestroyPipeline(const VkDevice& device, CommandPool& commandPool);
	    void DestroyMeshes(const VkDevice& device);
	
	    void Record(const VkExtent2D& swapChainExtent, uint32_t imageIndex);
	
	    VulkanCommandBuffer& GetCommandBuffer()
	    {
	        return m_CommandBuffer;
	    }

		GraphicsPipeline3D& GetGraphicsPipeline()
		{
			return m_GraphicsPipeline;
		}

		void DestroyUniformBuffers(const VkDevice& device);

	private:
	    void DrawScene(const VkExtent2D& swapChainExtent, uint32_t imageIndex);
	private:
	    GraphicsPipeline3D m_GraphicsPipeline;
	    GP2Shader3D m_Shader;
	    VulkanCommandBuffer m_CommandBuffer;
	    RenderPass m_RenderPass;
	    Scene m_Scene;
};
