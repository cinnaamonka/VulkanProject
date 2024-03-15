#pragma once
#include <vulkan/vulkan.h>
#include "Mesh.h"  

class CommandPool;

class RectMesh : public Mesh
{
    
public:
    RectMesh(glm::vec2 left,double width,double height);
    ~RectMesh() {};
    void InitializeRect(const VkPhysicalDevice& physicalDevice, const VkDevice& device,const VkQueue& graphicsQueue,
        const CommandPool& commandPool);
private:
    glm::vec2 m_LeftPos;
    double m_Width;
    double m_Height;
};