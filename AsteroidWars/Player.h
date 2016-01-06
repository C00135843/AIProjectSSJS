#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "Bullet.h"
#include <list>
#include "SwarmEnemy.h"

class Player
{
public:
	Player(int windowWidth, int windowHeight, int fullWidth, int fullHeight);

	void Update();
	void Draw(RenderWindow &window);

	void Shoot();

	list<Bullet*> GetBullets();

	Vector2f GetCentre();
	Vector2f GetPosition();
	Vector2f GetVelocity();
	float mod(float a, float b);

	bool CheckBulletCollision(SwarmEnemy *swarmEnemy);
	bool CheckSwarmCollision(SwarmEnemy *swarmEnemy);

	int GetHealth();
	void SetHealth(int myHealth);
private:
	Vector2f m_position;
	Vector2f m_velocity;
	float m_rotation;

	Texture m_playerTexture;
	Sprite m_playerSprite;

	Texture m_squareTexture;
	Sprite m_squareSprite;

	float m_speed;
	float m_rotationSpeed;

	list<Bullet*> m_bullets;
	int m_shootTimer;
	int m_shootTimerLimit;
	std::list<Bullet*>::iterator m_bulletIterator;

	int m_windowWidth;
	int m_windowHeight;
	int m_fullWidth;
	int m_fullHeight;

	int m_health;
	bool m_alive;
};

#endif