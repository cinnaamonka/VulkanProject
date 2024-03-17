#pragma once
#include "Rect.h"
#include "CommandPool.h"

RectMesh::RectMesh(glm::vec2 left, double width, double height) :
	m_LeftPos(left),
	m_Width(width),
	m_Height(height)
{

}

void RectMesh::InitializeRect(const VkPhysicalDevice& physicalDevice, const VkDevice& device, const VkQueue& graphicsQueue,
	const CommandPool& commandPool)
{

	std::vector<Vertex> rectVertices =
	{
	   { { m_LeftPos }, { 1.0f, 0.0f, 0.0f } },// {-0.25,-0.25},
	   { {m_LeftPos.x + m_Width, m_LeftPos.y }, { 0.0f, 1.0f, 0.0f } },//{0.25,-0.25}
	   { { m_LeftPos.x + m_Width,m_LeftPos.y + m_Height}, { 0.0f, 0.0f, 1.0f } },//0.25,0.25
	   { { m_LeftPos.x,m_LeftPos.y + m_Height  }, { 1.0f, 1.0f, 1.0f } },//{-0.25,0.25}

	};

	m_Vertices = rectVertices;

	const std::vector<uint16_t> indices =
	{
	   0, 1, 2, 2, 3, 0
	};

	Initialize(physicalDevice, device, m_Vertices, graphicsQueue, commandPool, indices);
}