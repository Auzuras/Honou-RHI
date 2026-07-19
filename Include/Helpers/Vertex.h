#ifndef HONOU_RHI_VERTEX_H
#define HONOU_RHI_VERTEX_H

#include <array>

// Temp definition for tests
struct Vertex
{
	std::array<float, 3> position = std::array<float, 3>();
	std::array<float, 3> normal = std::array<float, 3>();
	std::array<float, 2> textCoords = std::array<float, 2>();
};

#endif //HONOU_RHI_VERTEX_H