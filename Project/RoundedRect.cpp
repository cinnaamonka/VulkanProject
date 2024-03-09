#include "RoundedRect.h"
constexpr float g_Pi = 3.14f;

void RoundedRect::InitializeRoundedRect(const VkPhysicalDevice& physicalDevice, const VkDevice& device, glm::vec2 left, double width, double height, float radius, int numVertices)
{
    //m_Vertices.push_back({ { left }, { 1.0f, 0.0f, 0.0f } });
    //m_Vertices.push_back({ {left.x + width, left.y }, { 0.0f, 1.0f, 0.0f } });
    //m_Vertices.push_back({ { left.x,left.y + height  }, { 0.0f, 0.0f, 1.0f } });
    //m_Vertices.push_back({ { left.x,left.y + height  }, { 0.0f, 0.0f, 1.0f } });
    //m_Vertices.push_back({ {left.x + width, left.y }, { 0.0f, 1.0f, 0.0f } });
    //m_Vertices.push_back({ { left.x + width,left.y + height}, { 1.0f, 0.0f, 0.0f } });


    //std::vector<glm::vec2> rectVertices;

    //// Calculate the center points of the circles at each corner
    //glm::vec2 topLeftCenter = left + glm::vec2(radius, radius);
    //glm::vec2 topRightCenter = left + glm::vec2(width - radius, radius);
    //glm::vec2 bottomLeftCenter = left + glm::vec2(radius, height - radius);
    //glm::vec2 bottomRightCenter = left + glm::vec2(width - radius, height - radius);

    //rectVertices.push_back(topLeftCenter);
    //rectVertices.push_back(topRightCenter);
    //rectVertices.push_back(bottomLeftCenter);
    //rectVertices.push_back(bottomRightCenter);
    //
    //std::vector<glm::vec2> positions;

    //m_SegmentsNum = numVertices * 3;

    //// 4 times creating a circle
    //for (const auto& pos : rectVertices)
    //{
    //    positions.push_back(pos);

    //    for (int i = 0; i <= numVertices; ++i)
    //    {
    //        float theta = static_cast<float>(i) / static_cast<float>(numVertices) * 2.0f * 3.14159f;
    //        float x = pos.x + radius * std::cos(theta);
    //        float y = pos.y + radius * std::sin(theta);
    //        positions.push_back({ x, y });
    //    }

    //    std::vector<glm::vec2> triangles;
    //    for (int i = 1; i <= numVertices; ++i)
    //    {
    //        triangles.push_back(positions[0]); // Center point
    //        triangles.push_back(positions[i]);
    //        triangles.push_back(positions[(i % numVertices) + 1]);
    //    }

    //    for (int i = 0; i < m_SegmentsNum; ++i)
    //    {
    //        m_Vertices.push_back({ triangles[i],{1,0,0} });
    //    }
    //}

    //Initialize(physicalDevice, device, m_Vertices);
}
