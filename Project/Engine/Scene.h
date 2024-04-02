#pragma once
#include <vulkan/vulkan.h>
#include "./2D/Mesh2D.h"
#include "./3D/Mesh3D.h"

class Mesh2D;
class Mesh3D;
class CommandPool;
class IndexBuffer;

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) noexcept = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) noexcept = delete;

	void AddMesh(Mesh2D& mesh, const VkPhysicalDevice& physicalDevice,const VkDevice& device,
		const VkQueue& graphicsGueue,const CommandPool& commandPool);

	void AddMesh(const Mesh3D& mesh);

	void DrawMesh(const VkPipelineLayout& pipelineLayout, const VkCommandBuffer& cmdBuffer);
	void DestroyMeshes(const VkDevice& device);
private:
	std::vector<Mesh2D> m_Meshes;
	std::vector<Mesh3D> m_3DMeshes;
};
