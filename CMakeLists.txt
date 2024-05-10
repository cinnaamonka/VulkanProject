cmake_minimum_required(VERSION 3.10)

# Project Name
project(VulkanLab01)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8 # Specify the version you need
)

FetchContent_MakeAvailable(glfw)

include(FetchContent)

FetchContent_Declare(
  tinyobjloader
  GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
  GIT_TAG        release # You can specify a particular version tag here
)

FetchContent_MakeAvailable(tinyobjloader)

include(FetchContent)

FetchContent_Declare(
  image_stb
  GIT_REPOSITORY https://github.com/natanaeljr/stb-cmake-package.git
  GIT_TAG        master # You can specify a particular version~ tag here
)

FetchContent_MakeAvailable(image_stb)

# Find the required packages
find_package(Vulkan REQUIRED)

# Include Directories
include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory(Project)

# If using validation layers, copy the required JSON files (optional)
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_SOURCE_DIR}/path_to_json/VK_LAYER~_KHRONOS_validation.json"
#        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# Set the source directory
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/Project/models)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/Project/textures)

# Set the destination directory
set(DESTINATION_DIR ${CMAKE_BINARY_DIR}/Project)

# Copy the folder to the destination directory
file(COPY ${SOURCE_DIR} DESTINATION ${DESTINATION_DIR})