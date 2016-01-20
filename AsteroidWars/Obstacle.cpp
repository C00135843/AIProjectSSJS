#include "stdafx.h"
#include "Obstacle.h"

float mod(float a, float b)
{
	float thing = a - b * floor(a / b);
	return a - b * floor(a / b);
}


// Default constructor
Obstacle::Obstacle()
{

}

Obstacle::Obstacle(float xPos, float yPos, float angle, int fullWidth, int fullHeight)
{
	m_alive = true;

	setInRangeOfBoid(false);

	// Screen width and height
	m_fullWidth = fullWidth;
	m_fullHeight = fullHeight;

	// Load sprite image
	m_texture.loadFromFile("Pics/Asteroid.png");
	m_sprite = sf::Sprite(m_texture);

	// Radar
	m_radarTexture.loadFromFile("Pics/Yellow.png");
	m_radarSprite = sf::Sprite(m_radarTexture);

	m_position.x = xPos;
	m_position.y = yPos;

	m_velocity = sf::Vector2f(cos(angle) / 50, sin(angle) / 50);

	m_sprite.setPosition(m_position);

	m_sprite.setScale(0.25f, 0.25f);

	m_sprite.setOrigin(
		m_sprite.getScale().x / 2.0f + (m_sprite.getGlobalBounds().width / 2), 
		m_sprite.getScale().y / 2.0f + (m_sprite.getGlobalBounds().height / 2)
	);

	m_speed = 20;

	m_velocity.x = m_velocity.x * m_speed;
	m_velocity.y = m_velocity.y * m_speed;
}

void Obstacle::Draw(RenderWindow &window)
{
	if (m_alive)
		window.draw(m_sprite);
}

void Obstacle::DrawOnRadar(RenderWindow &window)
{
	if (m_alive)
	{
		m_radarSprite.setPosition(m_sprite.getPosition());
		window.draw(m_radarSprite);
	}
}

void Obstacle::Update()
{
	m_sprite.rotate(0.5f);

	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;

	m_position = Vector2f(mod(m_position.x, m_fullWidth), mod(m_position.y, m_fullHeight));

	m_sprite.setPosition(m_position);
}

bool Obstacle::CollisionWithSwarm(SwarmEnemy *swarmEnemy)
{
	if (swarmEnemy->GetSprite().getGlobalBounds().intersects(GetSprite().getGlobalBounds()))
	{
		return true;
	}
	return false;
}

vector<Obstacle*> Obstacle::CreateObstacle(int width, int height, Vector2f playerPos, vector<Obstacle*> obstacles)
{
	if (obstacles.size() < 100)
	{
		// Generate random number
		Vector2f obstaclePosition;
		obstaclePosition.x = rand() % width + 1;
		obstaclePosition.y = rand() % height + 1;

		float distance = 0;

		distance = sqrt(((obstaclePosition.x - playerPos.x)*(obstaclePosition.x - playerPos.x)) +
			((obstaclePosition.y - playerPos.y)*(obstaclePosition.y - playerPos.y)));

		while (distance <= 400)
		{
			obstaclePosition.x = rand() % width + 1;
			obstaclePosition.y = rand() % height + 1;

			distance = sqrt(((obstaclePosition.x - playerPos.x)*(obstaclePosition.x - playerPos.x)) +
				((obstaclePosition.y - playerPos.y)*(obstaclePosition.y - playerPos.y)));
		}

		// Generate random angle
		float angle = ((rand() % 360) / 100.0f) * 3.1415f;

		Obstacle *obstacle = new Obstacle(obstaclePosition.x, obstaclePosition.y, angle, width, height);

		obstacles.push_back(obstacle);
	}

	return obstacles;
}

Vector2f Obstacle::GetPosition()
{
	return m_position;
}

Vector2f Obstacle::GetVelocity()
{
	return m_velocity;
}

Sprite Obstacle::GetSprite()
{
	return m_sprite;
}

void Obstacle::SetAlive(bool myAlive)
{
	m_alive = myAlive;
}

bool Obstacle::GetAlive()
{
	return m_alive;
}

bool Obstacle::getInRangeOfBoid()
{
	return m_inRangeOfBoid;

}
void Obstacle::setInRangeOfBoid(bool myInRangeOfBoid)
{
	m_inRangeOfBoid = myInRangeOfBoid;
}