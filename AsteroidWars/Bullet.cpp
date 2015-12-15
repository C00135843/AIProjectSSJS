#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet(Vector2f pos, Vector2f vel)
{
	// Load main menu background image
	m_texture.loadFromFile("Pics/Bullet.png");
	m_sprite = sf::Sprite(m_texture);

	// Scale
	m_sprite.setScale(0.25f, 0.25f);

	m_speed = 3.0f;

	m_velocity.x = vel.x * m_speed;
	m_velocity.y = vel.y * m_speed;

	m_position = pos + m_velocity;
	m_sprite.setPosition(m_position);

	// Window dimensions
	m_windowWidth = 800;
	m_windowHeight = 600;
}

void Bullet::Update()
{
	m_position += m_velocity;
	m_sprite.setPosition(m_position);
}

bool Bullet::OutOfBounds()
{
	if (m_position.x >= m_windowWidth || m_position.x <= 0
		|| m_position.y >= m_windowHeight || m_position.y <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Bullet::Draw(RenderWindow &window)
{
	window.draw(m_sprite);
}