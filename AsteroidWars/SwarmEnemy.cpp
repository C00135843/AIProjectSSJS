#include "stdafx.h"
#include "SwarmEnemy.h"

SwarmEnemy::SwarmEnemy(int id)
{
	SetID(id);

	// Load main menu background image
	m_texture.loadFromFile("Pics/SwarmBoid.png");
	m_sprite = sf::Sprite(m_texture);

	// Radar
	m_radarTexture.loadFromFile("Pics/SwarmBoidRadar2.png");
	m_radarSprite = sf::Sprite(m_radarTexture);
	m_radarSprite.setScale(4.0f, 4.0f);

	// Scale
	m_sprite.setScale(0.9f, 0.9f);
}

void SwarmEnemy::Draw(sf::RenderWindow &window)
{
	window.draw(m_sprite);
}

void SwarmEnemy::DrawOnRadar(sf::RenderWindow &window)
{
	m_radarSprite.setPosition(m_sprite.getPosition());
	window.draw(m_radarSprite);
}

void SwarmEnemy::SetPosition(int x, int y)
{
	m_position.x = x;
	m_position.y = y;

	m_sprite.setPosition(m_position);
}

void SwarmEnemy::SetRotation(float theta)
{
	m_rotation = theta;

	m_sprite.setRotation(m_rotation);
}

Vector2f SwarmEnemy::GetPosition()
{
	return m_position;
}

Sprite SwarmEnemy::GetSprite()
{
	return m_sprite;
}

int SwarmEnemy::GetID()
{
	return m_id;
}

void SwarmEnemy::SetID(int id)
{
	m_id = id;
}