/*****************************************************************//**
 * \file   Obj.cpp
 * \brief  Source code of Obj class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#include "Obj.hpp"

// C++ includes
#include <fstream>
#include <algorithm>
#include <chrono>

constexpr const std::array<std::array<float, 3>, 4> colorsRand = { {
	{0.03f, 0.67f, 0.207f}, // Dark Green
	{0.498f, 0.105f, 0.8901f}, // Clear Purple
	{0.176f, 0.564f, 0.929f}, // Cyanish
	{0.909f, 0.745f, 0.2901f} // Weird Yellow
} };

Obj::Obj()
{
}

bool Obj::TryLoad(const char * fileName)
{
	std::ifstream in;
	// ensures ifstream objects can throw exceptions:
	in.exceptions(std::ifstream::badbit);
	in.open(fileName, std::ios::in);
	
	std::string buf;
	std::string cmd;
	in.exceptions(std::istream::badbit);
	
	while (!in.eof())
	{
		try
		{
			std::getline(in, buf);
			std::istringstream iss(buf);
			iss >> cmd;
	
			if (cmd.size() == 0 || cmd[0] == '#' || cmd[0] == '\n')
			{
				continue;
			}
			// Geometric Vertex
			else if (cmd.compare("v") == 0)
			{
				GLfloat x, y, z;
				iss >> x >> y >> z;
				vertices.emplace_back(x, y, z);
			}
			// TODO: Incomplete Face Parser .obj
			// Face
			else if (cmd.compare("f") == 0)
			{
				int f[3];
				iss >> f[0] >> f[1] >> f[2];
				// Assigning a color
				std::array<float, 3> c = colorsRand[rand() % 4];
				faces.emplace_back(f, c.data());
			}
			// Texture Coordinate
			else if (cmd.compare("vt") == 0)
			{
			}
			// Vertex Normals
			else if (cmd.compare("vp") == 0)
			{
			}
			// Space Vertices
			else if (cmd.compare("vn") == 0)
			{
			}
			else
			{
				std::cerr << "Warning: unsupported line type starting with '" << buf[0] << "'" << std::endl;
				return false;
			}
		} catch (const std::istream::failure & e)
		{
			std::cerr << "Reading obj file error: " << e.what() << std::endl;
			return false;
		}
	}

	return true;
}