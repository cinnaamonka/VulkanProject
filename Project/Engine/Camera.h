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

#endif 
#include "Structs.h"
constexpr float PI = 3.14159265358979323846f;

constexpr auto TO_RADIANS(PI / 180.0f);

struct Camera
{
	Camera() = default;

	Camera(const glm::vec3& _origin, float _fovAngle) :
		origin{ _origin },
		fovAngle{ _fovAngle }
	{
	}

	glm::vec3 origin = { 1 * cosf(0),0,-5 * sinf(1) };
	float fovAngle = 90.f;

	glm::vec3 forward = { 0,0,1 };
	glm::vec3 up = { 0,1,0 };
	glm::vec3 right = { 1,0,0 };

	float totalPitch = 0.f;
	float totalYaw = 0.f;

	glm::mat4 m_RotationMatrixX = {};
	glm::mat4 m_RotationMatrixY = {};

	const glm::vec3 UnitX = glm::vec3{ 1, 0, 0 };
	const glm::vec3 UnitY = glm::vec3{ 0, 1, 0 };
	const glm::vec3 UnitZ = glm::vec3{ 0, 0, 1 };
	const glm::vec3 Zero = glm::vec3{ 0, 0, 0 };


	glm::mat4 CalculateCameraToWorld()
	{
		// Construct the camera-to-world transformation matrix
		return glm::mat4(
			glm::vec4(right, 0.0f),
			glm::vec4(up, 0.0f),
			glm::vec4(-forward, 0.0f), // Negate forward if it's meant to be the view direction
			glm::vec4(origin, 1.0f)
		);
	}

	void HandleZoom(int zoomValue)
	{
		const float minFOV = 0.0f;
		const float maxFOV = 180.0f;

		fovAngle += zoomValue;
		fovAngle = std::max(minFOV, std::min(maxFOV, fovAngle));
	}
	void OnKeyEvent(int key, int scancode, int action, int mods, float elapsedSec)
	{
		const float step = 0.3f;

		if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			origin += (step * elapsedSec) * glm::normalize(forward);
		}
		if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			origin -= (step * elapsedSec) * glm::normalize(forward);
		}
		if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			origin -= (step * elapsedSec) * glm::normalize(right);
		}
		if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			origin += (step * elapsedSec) * glm::normalize(right);
		}

	}
	void OnMouseMove(double xpos, double ypos, float dragStartX, float dragStartY, float elapsedSec)
	{
		float threshold = 20.0f;

		float dy = static_cast<float>(ypos) - dragStartY;
		float dx = static_cast<float>(xpos) - dragStartX;

		// Set sensitivity for mouse movement
		float sensitivity = 0.001f;

		if (std::abs(dy) > threshold)
		{
			// Update the pitch based on mouse movement
			totalPitch += dy * sensitivity;
		}
		if (std::abs(dx) > threshold)
		{
			// Update the pitch based on mouse movement
			totalYaw += dx * sensitivity;
		}

		// Adjust the camera's orientation based on the new pitch
		glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(totalPitch), UnitX);
		glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(totalYaw), -UnitY);
		forward = glm::vec3(rotationX * rotationY * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
		right = glm::vec3(rotationX * rotationY * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		up = glm::vec3(rotationX * rotationY * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	}

	void Update(float elapsedSec)
	{
		m_RotationMatrixX = glm::rotate(glm::mat4(1.0f), totalPitch, UnitY);
		m_RotationMatrixY = glm::rotate(glm::mat4(1.0f), totalYaw, UnitX);
	}

	ViewProjection GetViewProjection(float screenWidth, float screenHeight, float nearPlane, float farPlane)
	{
		ViewProjection vp{};

		// Calculate aspect ratio
		float aspectRatio = screenWidth / screenHeight;

		// Generate the view matrix using glm::lookAt()
		glm::vec3 targetPos = origin + forward;
		vp.view = glm::lookAt(origin, targetPos, up);

		// Generate the projection matrix using glm::perspective()
		vp.proj = glm::perspective(glm::radians(fovAngle), aspectRatio, nearPlane, farPlane);

		return vp;
	}
};