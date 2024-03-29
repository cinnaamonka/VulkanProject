#include "Camera.h"

void Camera::UpdateUniformBuffer(VertexUBO& buffer, VkExtent2D extent)
{
	// If the camera transform has changed, update matrix
	if (m_HasChanged)
		UpdateMatrix();

	// Set buffer view matrix
	buffer.view = m_Matrix;

	// Set the projection matrix
	buffer.proj = glm::perspective(m_FovAngle, extent.width / static_cast<float>(extent.height), 0.1f, 100.0f);
	buffer.proj[1][1] *= -1;
}

void Camera::UpdateMatrix()
{
	// Set hasChanged to false
	m_HasChanged = false;

	// Create translation matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_Position);

	// Cast rotation vector to quaternion
	glm::quat quaternion = glm::quat(m_Rotation);
	// Create rotation matrix
	glm::mat4 rotationMatrix = glm::mat4_cast(quaternion);
	// Create scaling matrix
	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), m_Scale);

	// Multiply matrices
	m_Matrix = translationMatrix * rotationMatrix * scalingMatrix;
}
