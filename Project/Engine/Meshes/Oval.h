#pragma once
#include <vulkan/vulkan.h>
#include "../2D/Mesh2D.h"

class CommandPool;

class Oval : public Mesh2D
{
public:
	Oval(glm::vec2 center,float radius, int numVertexes);
	~Oval() {};

	void InitializeOval(const VkPhysicalDevice& physicalDevice, const VkDevice& device,const VkQueue& graphicsQueue,
		const CommandPool& commandPool, ImageManager& imageManager);

private:
	glm::vec2 m_Center;
	float m_Radius = 0.5f;
	int m_VertixesNum;
};