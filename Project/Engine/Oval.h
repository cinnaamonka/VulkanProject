#pragma once
#include <vulkan/vulkan.h>
#include "Mesh.h"

class CommandPool;

class Oval : public Mesh
{
public:
	Oval(glm::vec2 center,float radius, int numVertexes);
	~Oval() {};

	void InitializeOval(const VkPhysicalDevice& physicalDevice, const VkDevice& device,const VkQueue& graphicsQueue,
		const CommandPool& commandPool);

private:
	glm::vec2 m_Center;
	float m_Radius = 0.5f;
	int m_VertixesNum;
};