#include "Player.h"

Player::Player(const std::string& texture, const std::string& shader, glm::vec2 pos)
	: Entity(texture, shader, pos), m_Velocity(glm::vec2(0.1f, 0.1f)), m_Acceleration(glm::vec2(0.1f, 0.1f))
{

}

void Player::Update()
{
	m_Velocity += m_Acceleration;
	m_Position += m_Velocity;
}