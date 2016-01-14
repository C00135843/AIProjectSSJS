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

	m_squareTexture.loadFromFile("Pics/Square.png");
	m_squareSprite = sf::Sprite(m_squareTexture);

	// Set position
	m_position.x = m_windowWidth / 2;
	m_position.y = m_windowHeight / 2;
	m_playerSprite.setPosition(m_position);

	// Set velocity
	m_velocity.x = 0.1f;
	m_velocity.y = 0.1f;

	// Set origin
	m_playerSprite.setOrigin(
		m_position.x - 10,
		m_position.y + 10
		);

	// Scale
	m_playerSprite.setScale(0.125f, 0.125f);

	// Rotation
	m_rotation = 0;
	m_playerSprite.rotate(m_rotation);

	m_speed = 5.0f;
	m_rotationSpeed = 3.0f;

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
	// Shoot timer
	if (m_shootTimer <= m_shootTimerLimit)
		m_shootTimer++;

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

	m_squareSprite.setPosition(m_playerSprite.getPosition());

	cout << m_bullets.size() << endl;

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

void Player::Shoot()
{
	Vector2f bulletPos;
	bulletPos.x = m_position.x;
	bulletPos.y = m_position.y;

	Bullet* bullet = new Bullet(bulletPos, m_velocity, m_windowWidth, m_windowHeight, m_fullWidth, m_fullHeight);

	m_bullets.push_back(bullet);
}

void Player::Draw(RenderWindow &window)
{
	window.draw(m_playerSprite);
	//window.draw(m_squareSprite);

	// Draw bullets
	if (m_bullets.size() > 0)
	{
		for each(Bullet* bullet in m_bullets)
		{
			bullet->Draw(window);
		}
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