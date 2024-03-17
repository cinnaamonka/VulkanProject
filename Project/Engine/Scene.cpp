#include "Scene.h"
#include "Mesh.h"
#include "Rect.h"
#include "Oval.h"
#include "RoundedRect.h"
#include "Structs.h"
#include "IndexBuffer.h"

constexpr float g_Pi = 3.14159265359f;

void Scene::AddMesh(Mesh& mesh, const VkPhysicalDevice& physicalDevice,const VkDevice& device, const VkQueue& graphicsGueue, 
	const CommandPool& commandPool)
{
	RectMesh* rectMesh = dynamic_cast<RectMesh*>(&mesh);

	if (rectMesh)
	{
		rectMesh->InitializeRect(physicalDevice, device,graphicsGueue,commandPool);
		m_Meshes.push_back(*rectMesh);
	}

	Oval* ovalMesh = dynamic_cast<Oval*>(&mesh);

	if (ovalMesh)
	{
		ovalMesh->InitializeOval(physicalDevice, device,graphicsGueue,commandPool);
	
		m_Meshes.push_back(*ovalMesh);
	}

	RoundedRect* roundedRectMesh = dynamic_cast<RoundedRect*>(&mesh);

	if (roundedRectMesh)
	{
		roundedRectMesh->InitializeRoundedRect(physicalDevice, device,graphicsGueue,commandPool);
		m_Meshes.push_back(*roundedRectMesh);
	}

}

void Scene::DrawMesh(const VkCommandBuffer& cmdBuffer) const
{
	for (const auto& mesh : m_Meshes)
	{
		mesh.Draw(cmdBuffer);
	}
}

void Scene::DestroyMeshes(const VkDevice& device)
{
	for (auto& mesh : m_Meshes)
	{
		mesh.DestroyMesh(device);
	}
}

