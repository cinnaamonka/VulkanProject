#pragma once 
#include "Oval.h"

constexpr double g_Pi = 3.14;

Oval::Oval(glm::vec2 center, float radius, int numVertexes):
    m_Center(center),
    m_Radius(radius),
    m_VertixesNum(numVertexes)
{

}
void Oval::InitializeOval(const VkPhysicalDevice& physicalDevice, const VkDevice& device, const VkQueue& graphicsQueue,
    const CommandPool& commandPool)
{
    float radians = g_Pi * 2 / m_VertixesNum;

    std::vector<Vertex> finalVertexes;

    Vertex center = { glm::vec2{m_Center}, glm::vec3{0.0f,0.0f,1.0f} };
    Vertex startPoint = { glm::vec2{}, glm::vec3{0.0f,1.0f,0.0f} };
    Vertex endPoint = { glm::vec2{}, glm::vec3{1.0f,0.0f,0.0f} };

    for (int i = 0; i < m_VertixesNum; i++)
    {
        startPoint.pos.x = m_Center.x + m_Radius * glm::cos(radians * i);
        startPoint.pos.y = m_Center.y + m_Radius * glm::sin(radians * i);
        endPoint.pos.x = m_Center.x + m_Radius * glm::cos(radians * (i + 1));
        endPoint.pos.y = m_Center.y + m_Radius * glm::sin(radians * (i + 1));

        finalVertexes.push_back({ startPoint.pos, startPoint.color });
        finalVertexes.push_back({ endPoint.pos, endPoint.color });
        finalVertexes.push_back({ center.pos, center.color });

    }

    m_Vertices = finalVertexes;
 
    Initialize(physicalDevice, device, m_Vertices, graphicsQueue,commandPool);
}
