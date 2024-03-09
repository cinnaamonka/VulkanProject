#pragma once
#include <vulkan/vulkan.h>
#include "Mesh.h"  

class RectMesh : public Mesh
{
    
public:
    RectMesh() {};
    void InitializeRect(const VkPhysicalDevice& physicalDevice, const VkDevice& device, glm::vec2 left, double width, double height);

};