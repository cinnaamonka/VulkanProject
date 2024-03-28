#pragma once
#include <vulkan/vulkan.h>
#include "../2D/Mesh2D.h"  

class CommandPool;

class RectMesh : public Mesh2D
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