/*****************************************************************//**
 * \file   Constants.hpp
 * \brief  Contains all constants/constexpr variables associated
 *         with settings
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 04 2022
 *********************************************************************/

namespace Constants
{
namespace Window
{
constexpr const char * windowName = "Assignment 2: Pruvost Kevin 2021400603";
}; // !Constants::Window

namespace Paths
{
// Window
constexpr const char * windowIcon            = "resources/tsinghua_icon.png";
// Shaders
constexpr const char * pointShaderVertex     = "resources/point.vert.glsl";
constexpr const char * pointShaderFrag       = "resources/point.frag.glsl";
constexpr const char * faceShaderVertex      = "resources/face.vert.glsl";
constexpr const char * faceShaderFrag        = "resources/face.frag.glsl";
constexpr const char * wireframeShaderVertex = "resources/wireframe.vert.glsl";
constexpr const char * wireframeShaderFrag   = "resources/wireframe.frag.glsl";
// Planets
constexpr const char * earth   = "resources/earth.jpg";
constexpr const char * mercury = "resources/mercury.jpg";
constexpr const char * sun     = "resources/sun.jpg";
constexpr const char * moon    = "resources/moon.jpg";
constexpr const char * neptune = "resources/neptune.jpg";
constexpr const char * uranus  = "resources/uranus.jpg";
constexpr const char * venus   = "resources/venus.jpg";
constexpr const char * saturn  = "resources/saturn.jpg";
constexpr const char * jupiter = "resources/jupiter.jpg";
constexpr const char * mars    = "resources/mars.jpg";
namespace Models
{
namespace Rat
{
constexpr const char * objFile  = "resources/real_rat.obj";
constexpr const char * material = "resources/real_rat.mtl";
constexpr const char * texture  = "resources/rat.png";
}; // !Constants::Paths::Models::Rat
}; // !Constants::Paths::Models
}; // !Constants::Paths
}; // !Constants