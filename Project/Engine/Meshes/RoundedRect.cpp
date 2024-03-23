#include "RoundedRect.h"
#include "../CommandPool.h"

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
	const float radians = g_Pi / 2 / m_SegmentsNum;

	std::vector<Vertex> finalVertexes;

	// Upper Rect
	Vertex vertices[4]{ {glm::vec2{m_LeftPos.x + m_Radius, m_LeftPos.y + m_Height},glm::vec3{1.0f,0.0f,0.0f}},
						{glm::vec2{m_LeftPos.x + m_Width - m_Radius, m_LeftPos.y + m_Height},glm::vec3{0.0f,1.0f,0.0f}},
						{glm::vec2{m_LeftPos.x + m_Width - m_Radius, m_LeftPos.y + m_Height + m_Radius},glm::vec3{1.0f,0.0f,0.0f}},
						{glm::vec2{m_LeftPos.x + m_Radius, m_LeftPos.y + m_Height + m_Radius},glm::vec3{0.0f,0.0f,1.0f}} };

	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	finalVertexes.push_back({ vertices[3].pos, vertices[3].color });

	std::vector<uint16_t> indices =
	{
	   0, 1, 2, 2, 3, 0
	};

	// rounded parts
	Vertex startPoint{ {vertices[2].pos.x,vertices[2].pos.y + m_Radius },vertices[2].color };
	Vertex endPoint{};

	startPoint.color = glm::vec3{ 1.0f, 0.f, 0.f };
	endPoint.color = glm::vec3{ 0.0f, 1.f, 0.f };


	finalVertexes.push_back({ startPoint.pos, glm::vec3{0.0f, 0.0f, 1.0f} });

	for (int i = 0; i <= m_SegmentsNum; i++)
	{
		float angle = radians * i;
		glm::vec2 point(startPoint.pos.x + m_Radius * glm::cos(angle), startPoint.pos.y + m_Radius * glm::sin(angle));
		finalVertexes.push_back({ point, glm::vec3{0.0f, 1.0f, 0.0f} });

		indices.push_back(i + 4);
		indices.push_back((i + 1) % (m_SegmentsNum + 1) + 4);
		indices.push_back(m_SegmentsNum + 1 + 4);
	}

	startPoint.color = glm::vec3{ 1.0f, 0.f, 0.f };
	endPoint.color = glm::vec3{ 0.0f, 1.f, 0.f };

	startPoint.pos = { vertices[2].pos.x - m_Radius,vertices[2].pos.y + m_Radius };
	size_t size = finalVertexes.size();
	finalVertexes.push_back({ startPoint.pos, glm::vec3{0.0f, 0.0f, 1.0f} });

	for (int i = 0; i <= m_SegmentsNum; i++)
	{
		float angle = g_Pi / 2 + radians * i;
		glm::vec2 point(startPoint.pos.x + m_Radius * glm::cos(angle), startPoint.pos.y + m_Radius * glm::sin(angle));
		finalVertexes.push_back({ point, glm::vec3{0.0f, 1.0f, 0.0f} });

		indices.push_back(i + size); // index starts from the last vertex of the previous loop
		indices.push_back((i + size - 3) % (m_SegmentsNum + size + 1) + 4); // cyclically connect vertices
		indices.push_back(m_SegmentsNum + 1 + size); // index of the center vertex
	}

	startPoint.pos = { vertices[2].pos.x - m_Radius,vertices[2].pos.y };
	size = finalVertexes.size();
	finalVertexes.push_back({ startPoint.pos, glm::vec3{0.0f, 0.0f, 1.0f} });

	for (int i = 0; i <= m_SegmentsNum; i++)
	{
		float angle = g_Pi + radians * i;
		glm::vec2 point(startPoint.pos.x + m_Radius * glm::cos(angle), startPoint.pos.y + m_Radius * glm::sin(angle));
		finalVertexes.push_back({ point, glm::vec3{0.0f, 1.0f, 0.0f} });

		indices.push_back(i + size); // index starts from the last vertex of the previous loop
		indices.push_back((i + size - 3) % (m_SegmentsNum + size + 1) + 4); // cyclically connect vertices
		indices.push_back(m_SegmentsNum + 1 + size); // index of the center vertex
	}

	startPoint.pos = { vertices[2].pos.x,vertices[2].pos.y };
	size = finalVertexes.size();
	finalVertexes.push_back({ startPoint.pos, glm::vec3{0.0f, 0.0f, 1.0f} });

	for (int i = 0; i <= m_SegmentsNum; i++)
	{
		float angle = 3 * g_Pi / 2 + radians * i;
		glm::vec2 point(startPoint.pos.x + m_Radius * glm::cos(angle), startPoint.pos.y + m_Radius * glm::sin(angle));
		finalVertexes.push_back({ point, glm::vec3{0.0f, 1.0f, 0.0f} });

		indices.push_back(i + size); // index starts from the last vertex of the previous loop
		indices.push_back((i + size - 3) % (m_SegmentsNum + size + 1) + 4); // cyclically connect vertices
		indices.push_back(m_SegmentsNum + 1 + size); // index of the center vertex
	}

	vertices[0].pos = glm::vec2{ m_LeftPos.x, m_LeftPos.y + 3 * m_Radius };
	vertices[1].pos = glm::vec2{ m_LeftPos.x + m_Radius, m_LeftPos.y + 3 * m_Radius };
	vertices[2].pos = glm::vec2{ m_LeftPos.x + m_Radius, m_LeftPos.y + 4 * m_Radius };
	vertices[3].pos = glm::vec2{ m_LeftPos.x, m_LeftPos.y + 4 * m_Radius };

	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	finalVertexes.push_back({ vertices[3].pos, vertices[3].color });

	indices.push_back(finalVertexes.size() - 4);
	indices.push_back(finalVertexes.size() - 4 + 1);
	indices.push_back(finalVertexes.size() - 4 + 2);
	indices.push_back(finalVertexes.size() - 4);
	indices.push_back(finalVertexes.size() - 4 + 2);
	indices.push_back(finalVertexes.size() - 4 + 3);


	// middle rect
	vertices[0].pos = glm::vec2{ m_LeftPos.x + m_Radius, m_LeftPos.y + 3 * m_Radius };
	vertices[1].pos = glm::vec2{ m_LeftPos.x + 2 * m_Radius, m_LeftPos.y + 3 * m_Radius };
	vertices[2].pos = glm::vec2{ m_LeftPos.x + 2 * m_Radius, m_LeftPos.y + 4 * m_Radius };
	vertices[3].pos = glm::vec2{ m_LeftPos.x + m_Radius, m_LeftPos.y + 4 * m_Radius };

	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	finalVertexes.push_back({ vertices[3].pos, vertices[3].color });

	indices.push_back(finalVertexes.size() - 4);
	indices.push_back(finalVertexes.size() - 4 + 1);
	indices.push_back(finalVertexes.size() - 4 + 2);
	indices.push_back(finalVertexes.size() - 4);
	indices.push_back(finalVertexes.size() - 4 + 2);
	indices.push_back(finalVertexes.size() - 4 + 3);

	//Right Rect
	vertices[0].pos = glm::vec2{ m_LeftPos.x + 2 * m_Radius, m_LeftPos.y + 3 * m_Radius };
	vertices[1].pos = glm::vec2{ m_LeftPos.x + 3 * m_Radius, m_LeftPos.y + 3 * m_Radius };
	vertices[2].pos = glm::vec2{ m_LeftPos.x + 3 * m_Radius, m_LeftPos.y + 4 * m_Radius };
	vertices[3].pos = glm::vec2{ m_LeftPos.x + 2 * m_Radius, m_LeftPos.y + 4 * m_Radius };

	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	finalVertexes.push_back({ vertices[3].pos, vertices[3].color });

	indices.push_back(finalVertexes.size() - 4);
	indices.push_back(finalVertexes.size() - 4 + 1);
	indices.push_back(finalVertexes.size() - 4 + 2);
	indices.push_back(finalVertexes.size() - 4);
	indices.push_back(finalVertexes.size() - 4 + 2);
	indices.push_back(finalVertexes.size() - 4 + 3);


	//Bottom Rect
	vertices[0].pos = glm::vec2{ m_LeftPos.x + m_Radius, m_LeftPos.y + 4 * m_Radius };
	vertices[1].pos = glm::vec2{ m_LeftPos.x + 2 * m_Radius, m_LeftPos.y + 4 * m_Radius };
	vertices[2].pos = glm::vec2{ m_LeftPos.x + 2 * m_Radius, m_LeftPos.y + 5 * m_Radius };
	vertices[3].pos = glm::vec2{ m_LeftPos.x + m_Radius, m_LeftPos.y + 5 * m_Radius };

	finalVertexes.push_back({ vertices[0].pos, vertices[0].color });
	finalVertexes.push_back({ vertices[1].pos, vertices[1].color });
	finalVertexes.push_back({ vertices[2].pos, vertices[2].color });
	finalVertexes.push_back({ vertices[3].pos, vertices[3].color });

	indices.push_back(finalVertexes.size() - 4);
	indices.push_back(finalVertexes.size() - 4 + 1);
	indices.push_back(finalVertexes.size() - 4 + 2);
	indices.push_back(finalVertexes.size() - 4);
	indices.push_back(finalVertexes.size() - 4 + 2);
	indices.push_back(finalVertexes.size() - 4 + 3);




	m_Vertices = finalVertexes;

	Initialize(physicalDevice, device, m_Vertices, graphicsQueue, commandPool, indices);
}



