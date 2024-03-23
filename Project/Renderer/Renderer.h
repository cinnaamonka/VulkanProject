//#pragma once
//#include <vector>
//#include <memory>
//#include <string>
//#include <string>
//
//#include <vulkan/vulkan.h>
//
//class Mesh;
//
//class Renderer final
//{
//public:
//    Renderer();
//    ~Renderer();
//
//    Renderer(Renderer& other) = delete;
//    Renderer(Renderer&& other) = delete;
//    Renderer& operator=(Renderer& other) = delete;
//    Renderer& operator=(Renderer&& other) = delete;
//
//    void Render(std::vector<std::unique_ptr<Mesh>>& pModels);
//
//    void AddGraphicsPipeline(const std::string& pipelineName, const std::string& vertShaderName, const std::string& fragShaderName,
//        int vertexUbos, int fragmentUbos);
//
//    size_t GetMaxFrames() const
//    { 
//        return m_MaxFramesInFlight;
//    }
//    uint32_t GetCurrentFrame() const 
//    {
//        return  m_CurrentFrame; 
//    }
//
//    VkDevice GetDevice();
//
//    VkImageView& GetDefaultImageView();
//
//    VkSampler& GetSampler();
//
//    PipelinePair& GetPipeline(const std::string& name = "Default");
//
//    // Get the commandbuffer currently in use
//    VkCommandBuffer& GetCurrentCommandBuffer();
//
//    // Get the descriptorpool manager
//    DescriptorPoolManager* GetDescriptorPoolManager() const;
//
//    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
//
//    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
//
//    void CreateVertexBuffer(std::vector<D3D::Vertex>& vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory);
//
//    void CreateIndexBuffer(std::vector<uint32_t>& indices, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory);
//
//    void UpdateUniformBuffer(UniformBufferObject& buffer);
//
//    std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayout(int vertexUbos, int fragmentUbos, int textureAmount);
//
//    // Pointer for the camera
//    Camera* GetCamera() { return m_pCamera.get(); }
//
//
//private:
//
//    std::unique_ptr<GPUObject> m_pGpuObject{};
//
//    // The maximum amount of frames in flight
//    const uint32_t m_MaxFramesInFlight{ 3 };
//
//    // The current frame
//    uint32_t m_CurrentFrame = 0;
//
//    // Handle of the VkSurfaceKHR
//    VkSurfaceKHR m_Surface{};
//
//    std::unique_ptr<BufferManager> m_pBufferManager{};
//
//    // Pointer to the descriptor pool manager
//    std::unique_ptr<DescriptorPoolManager> m_pDescriptorPoolManager{};
//
//    // Pointer to thep pipeline manager
//    std::unique_ptr<PipelineManager> m_pPipelineManager{};
//
//    // Pointer to the instance wrapper
//    std::unique_ptr<InstanceWrapper> m_pInstanceWrapper{};
//
//    // Pointer to the image manager
//    std::unique_ptr<ImageManager> m_pImageManager{};
//
//    // Pointer to the commandpool manager
//    std::unique_ptr<CommandpoolManager> m_pCommandPoolManager{};
//
//    // Pointer to the renderpass wrapper
//    std::unique_ptr<RenderpassWrapper> m_pRenderpassWrapper{};
//
//    // Pointer to the swapchain wrapper
//    std::unique_ptr<SwapchainWrapper> m_pSwapchainWrapper{};
//
//    // Pointer to the sync object manager
//    std::unique_ptr<SyncObjectManager> m_pSyncObjectManager{};
//
//    // Pointer to the camera
//    std::unique_ptr<Camera> m_pCamera{};
//
//    // Clean up all vulkan objects
//    void CleanupVulkan();
//
//    // Initialize vulkan objects
//    void InitVulkan();
//
//    // Initialize the VkSurfaceKHR
//    void CreateSurface();
//
//    // Recreate the swapchain
//    void RecreateSwapChain();
//
//    void RecordCommandBuffer(VkCommandBuffer& commandBuffer, uint32_t imageIndex, std::vector<std::unique_ptr<Model>>& pModels);
//
//    VkCommandBuffer BeginSingleTimeCommands();
//
//    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
//
//    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
//};