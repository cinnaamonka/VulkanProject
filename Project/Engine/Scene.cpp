#include "Scene.h"
#include "./2D/Mesh2D.h"
#include "./3D/Mesh3D.h"
#include "../Engine/Meshes/Rect.h"
#include "../Engine/Meshes/Oval.h"
#include "../Engine/Meshes/RoundedRect.h"
#include "Structs.h"
#include "IndexBuffer.h"
#include "DescriptorSetManager.h"
#include "GraphicsPipeline.h"

constexpr float g_Pi = 3.14159265359f;

void Scene::AddMesh(Mesh2D& mesh, const VkPhysicalDevice& physicalDevice,const VkDevice& device, const VkQueue& graphicsGueue, 
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

void Scene::AddMesh(const Mesh3D& mesh) 
{
	m_3DMeshes.push_back(mesh);
} 

void Scene::DrawMesh(const VkPipelineLayout& pipelineLayout,const VkCommandBuffer& cmdBuffer)
{
	for (const auto& mesh : m_Meshes)
	{
		mesh.Draw(cmdBuffer);
	}

	for (auto& mesh : m_3DMeshes)
	{
		mesh.Draw(pipelineLayout, cmdBuffer);
	}
}

void Scene::DestroyMeshes(const VkDevice& device)
{
	for (auto& mesh : m_Meshes)
	{
		mesh.DestroyMesh(device,GraphicsPipeline::GetDiscriptorSetLayout());
	}
}

//void Scene::DestroyUniformBuffer(const VkDevice& device)
//{
//	for (auto& mesh : m_Meshes)
//	{
//		mesh.DestroyUniformBuffers(device, GraphicsPipeline::GetDiscriptorSetLayout());
//	}
//}

