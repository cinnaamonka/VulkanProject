// Camera.h
// This class will serve as a camera object and store the values of location, rotation and scale of the camera

// File includes
#ifndef GLMIncluded
#define GLMIncluded

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define GLM_ENABLE_EXPERIMENTAL
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/gtx/hash.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

#endif // !GLMIncluded
#include "Structs.h"

class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	void SetPosition(glm::vec3& position) { m_Position = position; SetDirtyFlag(); }
	void SetPosition(float x, float y, float z) { SetPosition(glm::vec3{ x,y,z }); SetDirtyFlag(); }
	void SetRotation(glm::vec3& rotation) { m_Rotation = rotation; SetDirtyFlag(); }
	void SetRotation(float x, float y, float z) { SetRotation(glm::vec3{ x,y,z }); SetDirtyFlag(); }
	void SetScale(glm::vec3& scale) { m_Scale = scale; SetDirtyFlag(); }
	void SetScale(float x, float y, float z) { SetScale(glm::vec3{ x,y,z }); SetDirtyFlag(); }
	void SetFovAngle(float angle) { m_FovAngle = angle; SetDirtyFlag(); }
	void SetFovAngleDegrees(float angle) { m_FovAngle = glm::radians(angle); SetDirtyFlag(); }

	const glm::vec3& GetPosition() const { return m_Position; }
	const glm::vec3& GetRotation() const { return m_Rotation; }
	const glm::vec3& GetScale() const { return m_Scale; }

	void UpdateUniformBuffer(UniformBufferObjectStruct& buffer, VkExtent2D extent);

private:
	bool m_HasChanged{ true };
	float m_FovAngle{ static_cast<float>(glm::radians(90.0)) };

	glm::vec3 m_Position{ 0, 0, 0 };
	glm::vec3 m_Rotation{ 0, 0, 0 };
	glm::vec3 m_Scale{ 1, 1, 1 };

	glm::mat4 m_Matrix{};

	void SetDirtyFlag() { m_HasChanged = true; }

	void UpdateMatrix();
};
