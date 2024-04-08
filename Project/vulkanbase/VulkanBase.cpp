#include "VulkanBase.h"
#include "../Engine/Meshes/Rect.h"
#include "../Engine/Timer.h"
#include "VulkanUtil.h"

#include <functional>
#include <chrono>
#include <thread>
#include <algorithm>


VulkanBase::VulkanBase() :
	m_Window{},
	m_Surface{},
	imageAvailableSemaphore{},
	renderFinishedSemaphore{},
	renderFinishedSemaphore2{},
	m_DebugMessenger{},
	m_InFlightFence{},
	m_InFlightFence2{},
	m_Instance{},
	m_DeviceManager{},
	m_CameraRadius{ 5 },
	m_DragStart{},
	m_Rotation{},
	m_Camera{}
{

	const float xOffset = 3.f;

	//glm::vec3 vertices[] =
	//{
	//glm::vec3((-0.5f - 0.3f + xOffset) * 10.0f, -0.5f * 10.0f, -0.5f * 10.0f), //Front bottom-left corner
	//glm::vec3((-0.5f - 0.3f + xOffset) * 10.0f, -0.5f * 10.0f,  0.5f * 10.0f), //Back bottom-left corner
	//glm::vec3((-0.5f - 0.3f + xOffset) * 10.0f,  0.5f * 10.0f, -0.5f * 10.0f), //Front top-left corner
	//glm::vec3((-0.5f - 0.3f + xOffset) * 10.0f,  0.5f * 10.0f,  0.5f * 10.0f), //Back top-left corner
	//glm::vec3((0.5f - 0.3f + xOffset) * 10.0f, -0.5f * 10.0f, -0.5f * 10.0f),  //Front bottom-right corner
	//glm::vec3((0.5f - 0.3f + xOffset) * 10.0f, -0.5f * 10.0f,  0.5f * 10.0f),  //Back bottom-right corner
	//glm::vec3((0.5f - 0.3f + xOffset) * 10.0f,  0.5f * 10.0f, -0.5f * 10.0f),  //Front top-right corner
	//glm::vec3((0.5f - 0.3f + xOffset) * 10.0f,  0.5f * 10.0f,  0.5f * 10.0f)   //Back top-right corner

	//};

	//m_CubeMesh.AddTriangle(0, 1, 2);// front face
	//m_CubeMesh.AddTriangle(1, 3, 2);

	//m_CubeMesh.AddTriangle(4, 0, 6);
	//m_CubeMesh.AddTriangle(0, 2, 6);//back

	//m_CubeMesh.AddTriangle(5, 4, 7);
	//m_CubeMesh.AddTriangle(4, 6, 7);//top

	//m_CubeMesh.AddTriangle(1, 5, 3);
	//m_CubeMesh.AddTriangle(5, 7, 3);//bottom

	//m_CubeMesh.AddTriangle(2, 3, 6);// right
	//m_CubeMesh.AddTriangle(3, 7, 6);

	//m_CubeMesh.AddTriangle(4, 5, 0);// left 
	//m_CubeMesh.AddTriangle(5, 1, 0);

	//for (int i = 0; i < 8; ++i)
	//{
	//	m_CubeMesh.AddVertex(vertices[i], glm::vec3(1.0f), vertices[i]); // Assuming color is white for all vertices
	//}
	//std::vector<Vertex3D> meshVertexes = m_CubeMesh.GetVertices();

	//// Calculate normals
	//for (size_t i = 0; i < m_CubeMesh.GetIndices().size(); i += 3)
	//{
	//	uint16_t i1 = m_CubeMesh.GetIndices()[i];
	//	uint16_t i2 = m_CubeMesh.GetIndices()[i + 1];
	//	uint16_t i3 = m_CubeMesh.GetIndices()[i + 2];

	//	glm::vec3 v1 = meshVertexes[i1].position;
	//	glm::vec3 v2 = meshVertexes[i2].position;
	//	glm::vec3 v3 = meshVertexes[i3].position;

	//	glm::vec3 faceNormal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
	//	if (i / 2 % 2 == 1) faceNormal *= 1;
	//	// Add the face normal to each vertex normal
	//	meshVertexes[i1].normal += faceNormal;
	//	meshVertexes[i2].normal += faceNormal;
	//	meshVertexes[i3].normal += faceNormal;
	//}

	//m_CubeMesh.SetVertices(meshVertexes);

	m_CubeMesh.InitializeCube(glm::vec3((-0.5f - 0.3f + xOffset) * 10.0f, -0.5f * 10.0f, -0.5f * 10.0f), 10);

	LoadModel(MODEL_PATH, m_Model.GetVertices(), m_Model.GetModelIndices(), m_Model);
}

void VulkanBase::Run()
{
	initWindow();
	InitVulkan();

	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();

	while (doContinue)
	{
		Timer::Update();
		m_Lag += Timer::GetElapsed();

		while (m_Lag >= 1.0 / 60.0)
		{
			// fixed update here
			m_Lag -= 1.0 / 60.0;
		}
		doContinue = glfwWindowShouldClose(m_Window);
		MainLoop();
		Cleanup();

		const auto ms_per_frame = std::chrono::milliseconds(1000) / 60;

		const auto sleepTime = Timer::GetCurrent() + std::chrono::milliseconds(ms_per_frame) - std::chrono::high_resolution_clock::now();

		std::this_thread::sleep_for(sleepTime);
	}
}

void VulkanBase::InitVulkan()
{
	// week 06
	createInstance();
	setupDebugMessenger();
	CreateSurface();

	m_DeviceManager.PickPhysicalDevice(m_Instance, m_Surface);
	m_DeviceManager.CreateLogicalDevice(device, m_Surface);

	m_SwapChain.CreateSwapChain(m_Surface, m_Window, FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), m_Surface), device, m_DeviceManager.GetPhysicalDevice());
	m_SwapChain.CreateImageViews(device);

	m_CommandPool.CreateCommandPool(device, FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), m_Surface));

	m_DAEPipeline.Initialize(device, m_DeviceManager.GetPhysicalDevice(), m_SwapChain.GetSwapChainImageFormat(),
		m_SwapChain.GetSwapChainImageViews(),
		m_SwapChain.GetSwapChainExtent(), FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), m_Surface),
		m_DeviceManager.GetGraphicsQueue(), m_CommandPool);


	m_DAEPipeline3D.Initialize(device, m_DeviceManager.GetPhysicalDevice(), m_SwapChain.GetSwapChainImageFormat(),
		m_SwapChain.GetSwapChainImageViews(), m_SwapChain.GetSwapChainExtent(),
		FindQueueFamilies(m_DeviceManager.GetPhysicalDevice(), m_Surface), m_DeviceManager.GetGraphicsQueue(), m_CommandPool, m_CubeMesh, m_Model);

	// week 06
	createSyncObjects();
}

void VulkanBase::MainLoop()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		glfwPollEvents();
		// week 06
		DrawFrame();
	}
	vkDeviceWaitIdle(device);
}

void VulkanBase::Cleanup()
{
	vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
	vkDestroySemaphore(device, renderFinishedSemaphore2, nullptr);
	vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
	vkDestroyFence(device, m_InFlightFence, nullptr);
	vkDestroyFence(device, m_InFlightFence2, nullptr);

	m_DAEPipeline.DestroyPipeline(device, m_CommandPool);
	m_DAEPipeline3D.DestroyPipeline(device, m_CommandPool);

	for (auto imageView : m_SwapChain.GetSwapChainImageViews())
	{
		vkDestroyImageView(device, imageView, nullptr);
	}

	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
	}
	vkDestroySwapchainKHR(device, m_SwapChain.GetSwapChain(), nullptr);

	m_DAEPipeline.DestroyMeshes(device);
	m_DAEPipeline3D.DestroyMeshes(device);

	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
	vkDestroyInstance(m_Instance, nullptr);


	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void VulkanBase::CreateSurface()
{
	if (glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &m_Surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

void VulkanBase::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	glfwSetWindowUserPointer(m_Window, this);

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
			vBase->OnKeyEvent(key, scancode, action, mods);
		});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
			vBase->OnMouseMove(window, xpos, ypos);
		});
	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
			vBase->OnMouseEvent(window, button, action, mods);
		});
}
void VulkanBase::OnKeyEvent(int key, int scancode, int action, int mods)
{
	m_Camera.OnKeyEvent(key, scancode, action, mods, Timer::GetElapsed());
}
void VulkanBase::OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);


	if (state == GLFW_PRESS)
	{
		m_Camera.OnMouseMove(xpos, ypos, m_DragStart.x, m_DragStart.y, Timer::GetElapsed());
	}
}
void VulkanBase::OnMouseEvent(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		std::cout << "right mouse button pressed\n";

		double xpos, ypos;

		glfwGetCursorPos(window, &xpos, &ypos);

		m_DragStart.x = static_cast<float>(xpos);
		m_DragStart.y = static_cast<float>(ypos);
	}
}
