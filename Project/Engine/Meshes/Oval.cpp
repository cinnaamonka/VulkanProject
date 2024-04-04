#pragma once 
#include "Oval.h"

constexpr double g_Pi = 3.14;

Oval::Oval(glm::vec2 center, float radius, int numVertexes) :
	m_Center(center),
	m_Radius(radius),
	m_VertixesNum(numVertexes)
{

}
void Oval::InitializeOval(const VkPhysicalDevice& physicalDevice, const VkDevice& device, const VkQueue& graphicsQueue,
    const CommandPool& commandPool)
{
    std::vector<Vertex> finalVertexes;

    for (int i = 0; i < m_VertixesNum; i++)
    {
        float angle = static_cast<float>(2 * g_Pi * i / m_VertixesNum);
        glm::vec2 point(m_Center.x + m_Radius * glm::cos(angle), m_Center.y + m_Radius * glm::sin(angle));
        finalVertexes.push_back({ point, glm::vec3{0.0f, 1.0f, 0.0f} });
    }

    finalVertexes.push_back({ m_Center, glm::vec3{0.0f, 0.0f, 1.0f} });

    std::vector<uint16_t> indices;
    for (uint16_t i = 0; i < m_VertixesNum; ++i) 
    {
        indices.push_back(i);
        indices.push_back((i + 1) % m_VertixesNum);
        indices.push_back(m_VertixesNum); 
    }

    m_Vertices = finalVertexes;

    Initialize(physicalDevice, device, m_Vertices, graphicsQueue, commandPool, indices);
}
