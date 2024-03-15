#include "RoundedRect.h"
#include "CommandPool.h"

constexpr float g_Pi = 3.14f;

RoundedRect::RoundedRect(glm::vec2 leftPos, double width, double height, double radius, int numOfSegments) :
	m_LeftPos(leftPos),
	m_Width(width),
	m_Height(height),
	m_Radius(radius),
	m_SegmentsNum(numOfSegments)
{

}

void RoundedRect::InitializeRoundedRect(const VkPhysicalDevice& physicalDevice, const VkDevice& device, const VkQueue& graphicsQueue, const CommandPool& commandPool)
{
	float radians = g_Pi / 2 / m_SegmentsNum;

	std::vector<Vertex> finalVertexes;

	//Upper Rect
	Vertex vertices[4]{ {glm::vec2{m_LeftPos.x + m_Radius, m_LeftPos.y + m_Height},glm::vec3{1.0f,0.0f,0.0f}},
						{glm::vec2{m_LeftPos.x + m_Width - m_Radius, m_LeftPos.y + m_Height},glm::vec3{0.0f,1.0f,0.0f}},
						{glm::vec2{m_LeftPos.x + m_Width - m_Radius, m_LeftPos.y + m_Height + m_Radius},glm::vec3{1.0f,0.0f,0.0f}},
						{glm::vec2{m_LeftPos.x + m_Radius, m_LeftPos.y + m_Height + m_Radius},glm::vec3{0.0f,0.0f,1.0f}} };


	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	finalVertexes.push_back({ vertices[3].pos, vertices[3].color });

	Vertex startPoint;
	Vertex endPoint;

	startPoint.color = glm::vec3{ 1.0f, 0.f, 0.f };
	endPoint.color = glm::vec3{ 0.0f, 1.f, 0.f };

	for (int i = 0; i < m_SegmentsNum; i++)
	{
		startPoint.pos.x = vertices[3].pos.x + m_Radius * glm::cos(radians * i + g_Pi);
		startPoint.pos.y = vertices[3].pos.y + m_Radius * glm::sin(radians * i + g_Pi);

		endPoint.pos.x = vertices[3].pos.x + m_Radius * glm::cos(radians * (i + 1) + g_Pi);
		endPoint.pos.y = vertices[3].pos.y + m_Radius * glm::sin(radians * (i + 1) + g_Pi);

		finalVertexes.push_back({ startPoint.pos, startPoint.color });
		finalVertexes.push_back({ endPoint.pos, endPoint.color });
		finalVertexes.push_back({ vertices[3].pos, vertices[3].color });
	}

	//Left Rect
	vertices[0].pos = glm::vec2{ m_LeftPos.x,  m_LeftPos.y + 3 * m_Radius + m_Radius };
	vertices[1].pos = glm::vec2{ m_LeftPos.x + m_Radius,  m_LeftPos.y + 3 * m_Radius + m_Radius };
	vertices[2].pos = glm::vec2{ m_LeftPos.x + m_Radius,  m_LeftPos.y + 3 * m_Radius };
	vertices[3].pos = glm::vec2{ m_LeftPos.x,  m_LeftPos.y + 3 * m_Radius };

	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[3].pos, vertices[3].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });

	for (int i = 0; i < m_SegmentsNum; i++)
	{
		startPoint.pos.x = vertices[1].pos.x + m_Radius * glm::cos(radians * i + g_Pi / 2);
		startPoint.pos.y = vertices[1].pos.y + m_Radius * glm::sin(radians * i + g_Pi / 2);

		endPoint.pos.x = vertices[1].pos.x + m_Radius * glm::cos(radians * (i + 1) + g_Pi / 2);
		endPoint.pos.y = vertices[1].pos.y + m_Radius * glm::sin(radians * (i + 1) + g_Pi / 2);

		finalVertexes.push_back({ startPoint.pos, startPoint.color });
		finalVertexes.push_back({ endPoint.pos, endPoint.color });
		finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	}

	// middle rect
	vertices[0].pos = glm::vec2{ m_LeftPos.x + m_Radius,  m_LeftPos.y + 2 * m_Height };
	vertices[1].pos = glm::vec2{ m_LeftPos.x + m_Width - m_Radius,  m_LeftPos.y + 2 * m_Height };
	vertices[2].pos = glm::vec2{ m_LeftPos.x + m_Width - m_Radius, m_LeftPos.y - m_Radius + 2 * m_Height };
	vertices[3].pos = glm::vec2{ m_LeftPos.x + m_Radius, m_LeftPos.y - m_Radius + 2 * m_Height };

	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[3].pos, vertices[3].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });

	//Right Rect
	vertices[0].pos = glm::vec2{ m_LeftPos.x + m_Width - m_Radius,  m_LeftPos.y + 2 * m_Height };
	vertices[1].pos = glm::vec2{ m_LeftPos.x + m_Width,  m_LeftPos.y + 2 * m_Height };
	vertices[2].pos = glm::vec2{ m_LeftPos.x + m_Width, m_LeftPos.y - m_Radius + 2 * m_Height };
	vertices[3].pos = glm::vec2{ m_LeftPos.x + m_Width - m_Radius, m_LeftPos.y - m_Radius + 2 * m_Height };

	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[3].pos, vertices[3].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });

	for (int i = 0; i < m_SegmentsNum; i++)
	{
		startPoint.pos.x = vertices[3].pos.x + m_Radius * glm::cos(radians * i - g_Pi / 2);
		startPoint.pos.y = vertices[3].pos.y + m_Radius * glm::sin(radians * i - g_Pi / 2);

		endPoint.pos.x = vertices[3].pos.x + m_Radius * glm::cos(radians * (i + 1) - g_Pi / 2);
		endPoint.pos.y = vertices[3].pos.y + m_Radius * glm::sin(radians * (i + 1) - g_Pi / 2);

		finalVertexes.push_back({ startPoint.pos, startPoint.color });
		finalVertexes.push_back({ endPoint.pos, endPoint.color });
		finalVertexes.push_back({ vertices[3].pos, vertices[3].color });
	}

	//Bottom Rect
	vertices[0].pos = glm::vec2{ m_LeftPos.x + m_Radius, m_LeftPos.y - 2 * m_Radius + 3 * m_Height };
	vertices[1].pos = glm::vec2{ m_LeftPos.x + m_Width - m_Radius, m_LeftPos.y - 2 * m_Radius + 3 * m_Height };
	vertices[2].pos = glm::vec2{ m_LeftPos.x + m_Width - m_Radius, m_LeftPos.y + 3 * m_Height - m_Radius };
	vertices[3].pos = glm::vec2{ m_LeftPos.x + m_Radius, m_LeftPos.y + 3 * m_Height - m_Radius };

	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	finalVertexes.push_back({ vertices[3].pos, vertices[3].color });

	for (int i = 0; i < m_SegmentsNum; i++)
	{
		startPoint.pos.x = vertices[1].pos.x + m_Radius * glm::cos(radians * i);
		startPoint.pos.y = vertices[1].pos.y + m_Radius * glm::sin(radians * i);

		endPoint.pos.x = vertices[1].pos.x + m_Radius * glm::cos(radians * (i + 1));
		endPoint.pos.y = vertices[1].pos.y + m_Radius * glm::sin(radians * (i + 1));

		finalVertexes.push_back({ startPoint.pos, startPoint.color });
		finalVertexes.push_back({ endPoint.pos, endPoint.color });
		finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	}
	m_Vertices = finalVertexes;

	Initialize(physicalDevice, device, m_Vertices, graphicsQueue, commandPool);
}



