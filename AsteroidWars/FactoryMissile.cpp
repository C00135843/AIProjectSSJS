#include "stdafx.h"
#include "FactoryMissile.h"

FactoryMissile::FactoryMissile(Vector2f position, Vector2f velocity)
{
	// Load missile image
	m_texture.loadFromFile("Pics/FactoryMissile.png");
	m_sprite = sf::Sprite(m_texture);

	m_position = position;
	m_velocity = velocity;

	m_sprite.setScale(0.75f, 0.75f);

	m_speed = 5.0f;

	m_velocity.x = m_velocity.x * m_speed;
	m_velocity.y = m_velocity.y * m_speed;

	m_position = m_position + m_velocity;
	m_sprite.setPosition(m_position);

	alive = true;
}

void FactoryMissile::Draw(RenderWindow &window)
{
	window.draw(m_sprite);
}

void FactoryMissile::Update(Vector2f &p)
{
	timeSinceLastUpdate = m_clock.getElapsedTime();
	float time = timeSinceLastUpdate.asSeconds();

	if (time >= 10.0f)
	{
		alive = false;
	}
	Pvector pPos(p.x, p.y);
	m_position += seek(pPos);
	m_sprite.setPosition(m_position);
}

bool FactoryMissile::GetAlive()
{
	return alive;
}

void FactoryMissile::SetAlive(bool myAlive)
{
	alive = myAlive;
}

Sprite FactoryMissile::GetSprite()
{
	return m_sprite;
}
Vector2f FactoryMissile::seek(Pvector t)
{
	Pvector desired_velocity;
	Pvector steering;
	Pvector normVel(t.x - m_position.x, t.y - m_position.y);
	normVel.normalize();

	desired_velocity.x = normVel.x * m_speed;
	desired_velocity.y = normVel.y * m_speed;
	Vector2f dVel(desired_velocity.x, desired_velocity.y);

	return dVel;
}
