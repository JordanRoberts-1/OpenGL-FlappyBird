#include "Player.h"

Player::Player(const std::string& texture, const std::string& shader, glm::vec2 pos)
	: TexturedQuad(texture, shader, pos), m_Velocity(glm::vec2(1.0f, 1.0f)), m_Acceleration(glm::vec2(1.0f, 1.0f))
{

}

void Player::Update()
{
	m_Velocity += m_Acceleration;
	m_Position += m_Velocity;
}