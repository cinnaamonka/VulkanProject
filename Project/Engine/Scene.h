#pragma once
#include <vulkan/vulkan.h>
#include "./2D/Mesh2D.h"


class Mesh2D;
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

	void DrawMesh(const VkCommandBuffer& cmdBuffer) const;
	void DestroyMeshes(const VkDevice& device);
	//void DestroyUniformBuffer(const VkDevice& device);
private:
	std::vector<Mesh2D> m_Meshes;
};
