#pragma once
#include "Entity.h"

class Player :
	public Entity
{
public:
	Player(const std::string& texture, const std::string& shader, glm::vec2 pos);
	void Update();

private:
	glm::vec2 m_Velocity;
	glm::vec2 m_Acceleration;
};

