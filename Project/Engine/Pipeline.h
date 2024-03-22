#pragma once
#include <vulkan/vulkan.h>
#include "GP2Shader.h"
#include "CommandBuffer.h"
#include "Mesh.h"

class Pipeline
{
public:
    Pipeline() = default;
    ~Pipeline() = default;

   Pipeline(const Pipeline& other) = delete;
   Pipeline& operator=(const Pipeline& other) = delete;
   Pipeline(Pipeline&& other) = delete;
   Pipeline& operator=(Pipeline&& other) = delete;

   void Initialize();
   void RecordPipeline();
private:
    void DrawScene();
private:
    VkPipeline m_Pipeline;
    GP2Shader m_Shader; 
    VulkanCommandBuffer m_CommandBuffer;
    std::vector<Mesh> m_Meshes;
};
