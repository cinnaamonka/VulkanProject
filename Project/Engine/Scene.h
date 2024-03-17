#pragma once
#include <vulkan/vulkan.h>
#include "Mesh.h"


class Mesh;
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

	void AddMesh(Mesh& mesh, const VkPhysicalDevice& physicalDevice,const VkDevice& device,
		const VkQueue& graphicsGueue,const CommandPool& commandPool);

	void DrawMesh(const VkCommandBuffer& cmdBuffer) const;
	void DestroyMeshes(const VkDevice& device);
private:
	std::vector<Mesh> m_Meshes;
};
