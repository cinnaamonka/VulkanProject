#pragma once
#include <glm/glm.hpp>
struct Vertex3D
{
	glm::vec2 position;
	glm::vec3 color;
};
struct VertexUBO {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

struct VertexConstant 
{
	glm::mat4 modelMatrix;
};