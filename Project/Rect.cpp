#pragma once
#include "Rect.h"

void RectMesh::InitializeRect(const VkPhysicalDevice& physicalDevice, const VkDevice& device, glm::vec2 left, double width, double height)
{

    //std::vector<Vertex> rectVertices =
    //{
    //   { { left }, { 1.0f, 0.0f, 0.0f } },// {-0.25,-0.25},
    //     { {left.x + width, left.y }, { 0.0f, 1.0f, 0.0f } },//{0.25,-0.25}
    //   { { left.x,left.y + height  }, { 0.0f, 0.0f, 1.0f } },//{-0.25,0.25}
    //     { { left.x,left.y + height  }, { 0.0f, 0.0f, 1.0f } },// -0.25,0.25)
    //   { {left.x + width, left.y }, { 0.0f, 1.0f, 0.0f } },//0.25,-0.25
    // 
    //   { { left.x + width,left.y + height}, { 1.0f, 0.0f, 0.0f } }//0.25,0.25
    //};
    //
    //m_Vertices = rectVertices;

    //Initialize(physicalDevice, device, m_Vertices);
}