# CMakeList.txt : Top-level CMake project file

cmake_minimum_required (VERSION 3.8)

# Project Name
project ("Assignment1")

# C++ Standard Version
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR})

# Library Directory
add_subdirectory("lib")

# Library Directory
add_subdirectory("Documentation")

# Assignment1 Executable
add_executable(Assignment1
    "main.cpp"
    "OGL_Implementation/Input.cpp"
    "OGL_Implementation/Window.cpp"
    "OGL_Implementation/Obj.cpp"
    "OGL_Implementation/Shader.cpp"
    "OGL_Implementation/Camera.cpp"
    "OGL_Implementation/Mesh.cpp"
 "OGL_Implementation/GUI.cpp" "OGL_Implementation/GUI.hpp" "OGL_Implementation/Entity.cpp" "OGL_Implementation/Entity.hpp")
# Linking libraries to Assignment1
target_link_libraries(Assignment1 opengl32 glad glfw glm ImGui
)

# Including root directory & libraries directories
target_include_directories(Assignment1 PUBLIC
    ${PROJECT_SOURCE_DIR}
    $<INSTALL_INTERFACE:include>
)

# Installation Procedure
install(
    TARGETS Assignment1
    CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
)