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

struct Camera
{
	Camera() = default;

	Camera(const glm::vec3& _origin, float _fovAngle) :
		origin{ _origin },
		fovAngle{ _fovAngle }
	{
	}


	glm::vec3 origin = { 0,0,0 };
	float fovAngle = 90.f;

	glm::vec3 forward = { 0,0,1 };
	glm::vec3 up = { 0,1,0 };
	glm::vec3 right = { 1,0,0 };

	float totalPitch = 0.f;
	float totalYaw = 0.f;

	glm::mat4 cameraToWorld = {};


	glm::mat4 CalculateCameraToWorld()
	{
		right = glm::cross({ 0,1,0 }, glm::normalize(forward));
		up = glm::cross(forward, right);

		return
		{
			right,
			up,
			forward,
			origin
		};
	}

	void HandleZoom(int zoomValue)
	{
		const float minFOV = 0.0f;
		const float maxFOV = 180.0f;

		fovAngle += zoomValue;
		fovAngle = std::max(minFOV, std::min(maxFOV, fovAngle));
	}
	void OnKeyEvent(int key, int scancode, int action, int mods,float deltaTime,float step)
	{
		if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			origin += (step * deltaTime) * forward.Normalized();
		}
		if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			origin -= (step * deltaTime) * forward.Normalized();
		}
		if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			origin += (step * deltaTime) * right.Normalized();
		}
		if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
		{
			origin -= (step * deltaTime) * right.Normalized();
		}

	}
	void OnMouseMove(double xpos, double ypos, float dragStartX,float deltaTime)
	{
		const float rotationSpeed = 1.5f;

		totalYaw += TO_RADIANS * rotationSpeed * deltaTime * dragStartX;
	}
	void Update(Timer* pTimer)
	{
		const float deltaTime = pTimer->GetElapsed();
		const float step = 1.0f;

		const glm::vec3 movementDirection{};

		const glm::mat4 cameraToWorld = this->CalculateCameraToWorld();

		glm::vec3 transformedVector = glm::vec3(cameraToWorld[0]) * movementDirection.x +
			                          glm::vec3(cameraToWorld[1]) * movementDirection.y + 
			                          glm::vec3(cameraToWorld[2]) * movementDirection.z; 

		origin += transformedVector * deltaTime;


		const glm::mat4 rotMat
		{
		CalculateRotationMatrix(glm::vec3{totalPitch,totalYaw, 0.f})
		};

		glm::vec3 transformedVector2 = glm::vec3(rotMat[0]) * 0 +
									   glm::vec3(rotMat[1]) * 0 +
									   glm::vec3(rotMat[2]) * 1;

		forward = transformedVector2;
	}
	glm::mat3 CalculateRotationMatrix(const glm::vec3& r) 
	{
		// Convert Euler angles to radians
		float pitch = glm::radians(r.x);
		float yaw = glm::radians(r.y);
		float roll = glm::radians(r.z);

		// Calculate the rotation matrix
		glm::mat3 rotationMatrix;
		rotationMatrix[0][0] = glm::cos(yaw) * glm::cos(roll);
		rotationMatrix[0][1] = glm::cos(yaw) * glm::sin(roll);
		rotationMatrix[0][2] = -glm::sin(yaw);
		rotationMatrix[1][0] = glm::sin(pitch) * glm::sin(yaw) * glm::cos(roll) - glm::cos(pitch) * glm::sin(roll);
		rotationMatrix[1][1] = glm::sin(pitch) * glm::sin(yaw) * glm::sin(roll) + glm::cos(pitch) * glm::cos(roll);
		rotationMatrix[1][2] = glm::sin(pitch) * glm::cos(yaw);
		rotationMatrix[2][0] = glm::cos(pitch) * glm::sin(yaw) * glm::cos(roll) + glm::sin(pitch) * glm::sin(roll);
		rotationMatrix[2][1] = glm::cos(pitch) * glm::sin(yaw) * glm::sin(roll) - glm::sin(pitch) * glm::cos(roll);
		rotationMatrix[2][2] = glm::cos(pitch) * glm::cos(yaw);

		return rotationMatrix;
	}

	ViewProjection GetViewProjection(float screenWidth, float screenHeight, float nearPlane, float farPlane)
	{
		ViewProjection vp;

		// Calculate aspect ratio
		float aspectRatio = screenWidth / screenHeight;

		// Generate the view matrix using glm::lookAt()
		glm::vec3 targetPos = origin + forward; // Assuming forward is normalized
		vp.view = glm::lookAt(origin, targetPos, up);

		// Generate the projection matrix using glm::perspective()
		vp.proj = glm::perspective(glm::radians(fovAngle), aspectRatio, nearPlane, farPlane);

		return vp;
	}
};