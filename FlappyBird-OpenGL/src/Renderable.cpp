#include "Renderable.h"

Renderable::Renderable(glm::vec2 position, glm::vec2 size)
{
	//Build the vertice array from the given arguments
	m_Vertices[0] = position.x;
	m_Vertices[1] = position.y;
	m_Vertices[2] = 0.0f;
	m_Vertices[3] = 0.0f;

	m_Vertices[4] = position.x + size.x;
	m_Vertices[5] = position.y;
	m_Vertices[6] = 1.0f;
	m_Vertices[7] = 0.0f;

	m_Vertices[8] = position.x + size.x;
	m_Vertices[9] = position.y + size.y;
	m_Vertices[10] = 1.0f;
	m_Vertices[11] = 1.0f;

	m_Vertices[12] = position.x;
	m_Vertices[13] = position.y + size.y;
	m_Vertices[14] = 0.0f;
	m_Vertices[15] = 1.0f;
}
