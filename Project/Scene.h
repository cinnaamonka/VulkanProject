#pragma once
#include <vulkan/vulkan.h>
#include "Mesh.h"

class Mesh;
class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) noexcept = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) noexcept = delete;

	void AddRectangleMesh(float top, float left, float bottom, float right, const VkPhysicalDevice& physicalDevice,
					  const VkDevice& device);
	void AddOvalMesh(float centerX, float centerY, float radiusX, float radiusY, int numberOfSegments, 
				 const VkPhysicalDevice& physicalDevice, const VkDevice& device);
	void AddRoundedRectangleMesh(float top, float left, float bottom, float right, float radiusX, float radiusY, 
							 int numberOfSegmentsPerCorner, const VkPhysicalDevice& physicalDevice, 
							 const VkDevice& device);
	void DrawMesh(const VkCommandBuffer& cmdBuffer) const;
	void DestroyMeshes(const VkDevice& device);
private:
	std::vector<Mesh> m_Meshes;
};
