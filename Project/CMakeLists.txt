# Source files
set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADER_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${SHADER_SOURCE_DIR}/*.frag"
    "${SHADER_SOURCE_DIR}/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
)


set(SOURCES
    "Engine/main.cpp"
    "vulkanbase/VulkanBase.h" 
    "vulkanbase/VulkanUtil.h"
    # Add other source files here
     
    
    "labwork/Week06.cpp"
 "Engine/2D/GP2Shader2D.h" "Engine/2D/GP2Shader2D.cpp")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${GLSL_SOURCE_FILES}  "vulkanbase/VulkanUtil.cpp"  "Engine/CommandPool.h" "Engine/CommandPool.cpp" "Engine/Structs.h" "Engine/CommandBuffer.h" "Engine/CommandBuffer.cpp" "vulkanbase/VulkanBase.cpp"   "Engine/2D/Mesh2D.cpp" "Engine/Meshes/Rect.h" "Engine/Meshes/Rect.cpp" "Engine/Meshes/Oval.h" "Engine/Meshes/Oval.cpp" "Engine/Meshes/RoundedRect.h" "Engine/Meshes/RoundedRect.cpp" "Engine/Scene.h" "Engine/Scene.cpp" "Engine/GraphicsPipeline.h" "Engine/GraphicsPipeline.cpp" "Engine/RenderPass.h" "Engine/RenderPass.cpp" "Engine/BaseBuffer.cpp" "Engine/VertexBuffer.h" "Engine/VertexBuffer.cpp" "Engine/IndexBuffer.h" "Engine/IndexBuffer.cpp" "Engine/2D/Pipeline2D.h" "Engine/2D/Pipeline2D.cpp" "Engine/SwapChain.h" "Engine/SwapChain.cpp" "Engine/DescriptorSetManager.h" "Engine/DescriptorSetManager.cpp" "shaders/3DShader.vert" "Engine/GPUObject.h"   "Engine/3D/GP2Shader3D.h" "Engine/3D/GP2Shader3D.cpp" "Engine/3D/Pipeline3D.h" "Engine/3D/Pipleine3D.cpp" "Engine/3D/Mesh3D.h" "Engine/3D/Mesh3D.cpp" "Engine/GraphicsPipeline3D.h" "Engine/GraphicsPipeline3D.cpp" "Engine/DeviceManager.h" "Engine/DeviceManager.cpp" "Engine/Camera.cpp" "Engine/DaeDataBuffer.cpp"    "Engine/SyncObjectManager.h" "Engine/SyncObjectManager.cpp")
add_dependencies(${PROJECT_NAME} Shaders)
# Link libraries
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} glfw)