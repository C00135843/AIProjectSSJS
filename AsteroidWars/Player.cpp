
#include "stdafx.h"
#include "Player.h"
//#include <cmath>

Player::Player(int windowWidth, int windowHeight, int fullWidth, int fullHeight)
{
	// Width and height
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_fullWidth = fullWidth;
	m_fullHeight = fullHeight;

	// Load main menu background image
	m_playerTexture.loadFromFile("Pics/PlayerSpaceship.png");
	m_playerSprite = sf::Sprite(m_playerTexture);

	m_squareTexture.loadFromFile("Pics/Green.png");
	m_squareSprite = sf::Sprite(m_squareTexture);
	m_squareSprite.setScale(2.0f, 2.0f);

	// Set position
	m_position.x = m_windowWidth / 2;
	m_position.y = m_windowHeight / 2;
	m_playerSprite.setPosition(m_position);

	// Set velocity
	m_velocity.x = 0.1f;
	m_velocity.y = 0.1f;

	// Set origin
	m_playerSprite.setOrigin(m_playerTexture.getSize().x/2.f,m_playerTexture.getSize().y/2.f);


	// Scale
	m_playerSprite.setScale(0.125f, 0.125f);
	//m_playerSprite.setScale(0.0625f, 0.0625f);

	// Rotation
	m_rotation = 0;
	m_playerSprite.rotate(m_rotation);

	m_speed = 5.0f;
	m_rotationSpeed = 3.0f;

	// Set health & Alive
	m_health = 100;
	m_alive = true;

	// Bullets
	m_shootTimer = 30;
	m_shootTimerLimit = 30;
}

float Player::mod(float a, float b)
{
	float thing = a - b * floor(a / b);
	return a - b * floor(a / b);
}

void Player::Update()
{
	if (m_alive)
	{
		// Shoot timer
		if (m_shootTimer <= m_shootTimerLimit)
			m_shootTimer++;

		// Set alive
		if (m_health <= 0)
		{
			m_alive = false;
		}

		// Keyboard input
		if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
		{
			m_rotation = -m_rotationSpeed;
			m_playerSprite.rotate(m_rotation);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
		{
			m_rotation = m_rotationSpeed;
			m_playerSprite.rotate(m_rotation);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
		{
			m_position += m_velocity;
			m_position = Vector2f(mod(m_position.x, m_fullWidth), mod(m_position.y, m_fullHeight));
			m_playerSprite.setPosition(m_position);
		}
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (m_shootTimer >= m_shootTimerLimit)
			{
				Shoot();
				m_shootTimer = 0;
			}
		}

		// Set velocity
		m_velocity.x = (float)sin(m_playerSprite.getRotation() *3.14159265 / 180) * m_speed;
		m_velocity.y = -(float)cos(m_playerSprite.getRotation() *3.14159265 / 180) * m_speed;

		//cout << m_velocity.x << ", " << m_velocity.y << endl;

		m_squareSprite.setPosition(m_playerSprite.getPosition());

		// Update bullets
		if (m_bullets.size() > 0)
		{
			// Iterate through list of bullets
			for (m_bulletIterator = m_bullets.begin(); m_bulletIterator != m_bullets.end(); ++m_bulletIterator)
			{
				(*m_bulletIterator)->Update();

				// Remove bullet if out of bounds
				if ((*m_bulletIterator)->OutOfBounds(GetPosition()))
				{
					m_bullets.erase(m_bulletIterator);
					break;
				}
			}
		}
	}
}

void Player::Shoot()
{
	Vector2f bulletPos;
	bulletPos.x = m_position.x;
	bulletPos.y = m_position.y;

	Bullet* bullet = new Bullet(bulletPos, m_velocity, m_windowWidth, m_windowHeight, m_fullWidth, m_fullHeight,false);

	m_bullets.push_back(bullet);
}

void Player::Draw(RenderWindow &window)
{
	if (m_alive)
	{
		window.draw(m_playerSprite);
	}

	// Draw bullets
	if (m_bullets.size() > 0)
	{
		for each(Bullet* bullet in m_bullets)
		{
			bullet->Draw(window);
		}
	}
}

void Player::DrawOnRadar(RenderWindow &window)
{
	if (m_alive)
	{
		m_squareSprite.setPosition(GetPosition());
		window.draw(m_squareSprite);
	}
}

Vector2f Player::GetCentre()
{
	Vector2f centre;
	centre.x = m_playerSprite.getPosition().x + (m_playerSprite.getGlobalBounds().width / 2);
	centre.y = m_playerSprite.getPosition().y + (m_playerSprite.getGlobalBounds().height / 2);

	return centre;
}

Vector2f Player::GetPosition()
{
	return m_position;
}

Vector2f Player::GetVelocity()
{
	return m_velocity;
}

list<Bullet*> Player::GetBullets()
{
	return m_bullets;
}

bool Player::CheckBulletSwarmCollision(SwarmEnemy *swarmEnemy)
{
	if (m_bullets.size() > 0)
	{
		for (m_bulletIterator = m_bullets.begin(); m_bulletIterator != m_bullets.end(); ++m_bulletIterator)
		{
			if ( swarmEnemy->GetSprite().getGlobalBounds().intersects( (*m_bulletIterator)->GetSprite().getGlobalBounds() ) )
			{
				m_bullets.erase(m_bulletIterator);
				return true;
				break;
			}
		}
	}
	return false;
}

bool Player::CheckSwarmCollision(SwarmEnemy *swarmEnemy)
{
	if (m_playerSprite.getGlobalBounds().intersects(swarmEnemy->GetSprite().getGlobalBounds()))
	{
		return true;
	}
	return false;
}

bool Player::CheckPlayerFactoryCollision(Sprite &factory)
{
	if (m_playerSprite.getGlobalBounds().intersects(factory.getGlobalBounds()))
	{
		return true;
	}
	return false;
}

bool Player::CheckPlayerPredatorCollision(Sprite &predator)
{
	if (m_playerSprite.getGlobalBounds().intersects(predator.getGlobalBounds()))
	{
		return true;
	}
	return false;
}

bool Player::CheckObstacleCollision(Obstacle *obstacle)
{
	if (m_playerSprite.getGlobalBounds().intersects(obstacle->GetSprite().getGlobalBounds()))
	{
		return true;
	}
	return false;
}

bool Player::CheckBulletObstacleCollision(Obstacle *obstacle)
{
	if (m_bullets.size() > 0)
	{
		for (m_bulletIterator = m_bullets.begin(); m_bulletIterator != m_bullets.end(); ++m_bulletIterator)
		{
			if (obstacle->GetSprite().getGlobalBounds().intersects((*m_bulletIterator)->GetSprite().getGlobalBounds()))
			{
				m_bullets.erase(m_bulletIterator);
				return true;
				break;
			}
		}
	}
	return false;
}

bool Player::CheckBulletFactoryCollision(Factories *factory)
{
	if (m_bullets.size() > 0)
	{
		for (m_bulletIterator = m_bullets.begin(); m_bulletIterator != m_bullets.end(); ++m_bulletIterator)
		{
			if (factory->GetSprite().getGlobalBounds().intersects((*m_bulletIterator)->GetSprite().getGlobalBounds()))
			{
				m_bullets.erase(m_bulletIterator);
				return true;
				break;
			}
		}
	}
	return false;
}

bool Player::CheckBulletPredatorCollision(Sprite &predator)
{
	if (m_bullets.size() > 0)
	{
		for (m_bulletIterator = m_bullets.begin(); m_bulletIterator != m_bullets.end(); ++m_bulletIterator)
		{
			if (predator.getGlobalBounds().intersects((*m_bulletIterator)->GetSprite().getGlobalBounds()))
			{
				m_bullets.erase(m_bulletIterator);
				return true;
				break;
			}
		}
	}
	return false;
}

int Player::GetHealth()
{
	return m_health;
}

void Player::SetHealth(int myHealth)
{
	m_health = myHealth;
}

Sprite Player::GetSprite()
{
	return m_playerSprite;
}