#pragma once
#include "Rect.h"

RectMesh::RectMesh(glm::vec2 left, double width, double height):
    m_LeftPos(left),
    m_Width(width),
    m_Height(height)
{

}

void RectMesh::InitializeRect(const VkPhysicalDevice& physicalDevice, const VkDevice& device)
{

    std::vector<Vertex> rectVertices =
    {
       { { m_LeftPos }, { 1.0f, 0.0f, 0.0f } },// {-0.25,-0.25},
         { {m_LeftPos.x + m_Width, m_LeftPos.y }, { 0.0f, 1.0f, 0.0f } },//{0.25,-0.25}
       { { m_LeftPos.x,m_LeftPos.y + m_Height  }, { 0.0f, 0.0f, 1.0f } },//{-0.25,0.25}
         { { m_LeftPos.x,m_LeftPos.y + m_Height  }, { 0.0f, 0.0f, 1.0f } },// -0.25,0.25)
       { {m_LeftPos.x + m_Width, m_LeftPos.y }, { 0.0f, 1.0f, 0.0f } },//0.25,-0.25
       { { m_LeftPos.x + m_Width,m_LeftPos.y + m_Height}, { 1.0f, 0.0f, 0.0f } }//0.25,0.25
    };
    
    m_Vertices = rectVertices;

    Initialize(physicalDevice, device, m_Vertices);
}