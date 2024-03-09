#pragma once 
#include "Oval.h"

void Oval::InitializeOval(const VkPhysicalDevice& physicalDevice, const VkDevice& device, glm::vec2 center, float radius,int numVertices)
{
    //std::vector<glm::vec2> positions;

    //m_VertixesNum = numVertices * 3;

    //positions.push_back(center);
  
    //for (int i = 0; i <= numVertices; ++i)
    //{
    //    float theta = static_cast<float>(i) / static_cast<float>(numVertices) * 2.0f * 3.14159f;
    //    float x = center.x + radius * std::cos(theta);
    //    float y = center.y + radius * std::sin(theta);
    //    positions.push_back({ x, y });
    //}

    //std::vector<glm::vec2> triangles;
    //for (int i = 1; i <= numVertices; ++i)
    //{
    //    triangles.push_back(positions[0]); // Center point
    //    triangles.push_back(positions[i]);
    //    triangles.push_back(positions[(i % numVertices) + 1]);
    //}

    //for (int i = 0; i < m_VertixesNum; ++i)
    //{
    //    m_Vertices.push_back({ triangles[i],{1,0,0} });
    //}

    //Initialize(physicalDevice, device, m_Vertices);
}
