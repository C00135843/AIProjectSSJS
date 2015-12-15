#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "Bullet.h"
#include <list>

class Player
{
public:
	Player(int windowWidth, int windowHeight);

	void Update();
	void Draw(RenderWindow &window);

	void Shoot();

	Vector2f GetCentre();
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
};

#endif