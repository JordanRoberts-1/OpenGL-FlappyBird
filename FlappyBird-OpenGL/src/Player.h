#pragma once
#include "TexturedQuad.h"

class Player :
	public TexturedQuad
{
public:
	Player(const std::string& texture, const std::string& shader, glm::vec2 pos);
	void Update();

private:
	glm::vec2 m_Velocity;
	glm::vec2 m_Acceleration;
};

