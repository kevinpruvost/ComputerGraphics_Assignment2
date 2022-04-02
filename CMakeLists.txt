# CMakeList.txt : Top-level CMake project file

cmake_minimum_required (VERSION 3.8)

# Project Name
project ("Assignment2")

# C++ Standard Version
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR})

# Library Directory
add_subdirectory("lib")

# Library Directory
add_subdirectory("Documentation")

# Assignment2 Executable
file (GLOB ASSIGNMENT_SOURCES OGL_Implementation/*.cpp)
file (GLOB ASSIGNMENT_HEADERS OGL_Implementation/*.hpp)

add_executable(Assignment2
    "main.cpp"
    ${ASSIGNMENT_SOURCES}
    ${ASSIGNMENT_HEADERS}
 "OGL_Implementation/OpenGL_Timer.hpp" "OGL_Implementation/OpenGL_Timer.cpp")

# Linking libraries to Assignment2
target_link_libraries(Assignment2 opengl32 glad glfw glm ImGui SOIL
)

# Including root directory & libraries directories
target_include_directories(Assignment2 PUBLIC
    ${PROJECT_SOURCE_DIR}
    $<INSTALL_INTERFACE:include>
)

# Installation Procedure
install(
    TARGETS Assignment2
    CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
)