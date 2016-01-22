#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet(Vector2f pos, Vector2f vel, int windowWidth, int windowHeight, int fullWidth, int fullHeight,bool predator)
{
	// Load main menu background image
	m_texture.loadFromFile("Pics/Bullet.png");
	m_sprite = sf::Sprite(m_texture);

	// Scale
	if (predator)
		m_sprite.setScale(0.125f, 0.125f);
	else
		m_sprite.setScale(0.25f, 0.25f);

	if (predator)
		m_speed = 3.0f;
	else
		m_speed = 11.0f;

	m_velocity.x = vel.x * m_speed;
	m_velocity.y = vel.y * m_speed;

	m_position = pos + m_velocity;
	m_sprite.setPosition(m_position);

	// Window dimensions
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_fullWidth = fullWidth;
	m_fullHeight = fullHeight;
}

void Bullet::Update()
{
	m_position += m_velocity;
	m_sprite.setPosition(m_position);
}

bool Bullet::OutOfBounds(Vector2f playerPos)
{
	if (DistanceFromPlayer(playerPos) > m_windowWidth)
	{
	return true;
	}
	else
	{
	return false;
	}
}


float Bullet::DistanceFromPlayer(Vector2f playerPos)
{
	float distance;

	distance = sqrt(((m_position.x - playerPos.x)*(m_position.x - playerPos.x)) + ((m_position.y - playerPos.y)*(m_position.y - playerPos.y)));

	return distance;
}

void Bullet::Draw(RenderWindow &window)
{
	window.draw(m_sprite);
}

Sprite Bullet::GetSprite()
{
	return m_sprite;
}