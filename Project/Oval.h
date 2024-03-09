#pragma once
#include <vulkan/vulkan.h>
#include "Mesh.h"

class Oval : public Mesh
{
public:
	Oval(glm::vec2 center,float radius, int numVertexes);
	~Oval() {};

	void InitializeOval(const VkPhysicalDevice& physicalDevice, const VkDevice& device);

private:
	glm::vec2 m_Center;
	float m_Radius = 0.5f;
	int m_VertixesNum;
};