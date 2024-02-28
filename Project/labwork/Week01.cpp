#include "vulkanbase/VulkanBase.h"

void VulkanBase::initWindow() 
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}



void VulkanBase::DrawScene() 
{
	// vertex count,instance count,first vertex,first instance
	vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}