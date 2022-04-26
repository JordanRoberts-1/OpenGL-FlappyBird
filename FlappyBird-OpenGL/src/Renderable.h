#pragma once
#include <array>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"
#include "Renderer.h"

class Renderable
{

public:
	Renderable(glm::vec2 position, glm::vec2 size);
	inline std::array<float, 16> getVertices() { return m_Vertices; }
private:
	std::array<float, 16> m_Vertices;
};

