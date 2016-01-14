#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SwarmEnemy.h"

class Obstacle
{
private:
	bool m_alive;

	Vector2f m_position;
	Vector2f m_velocity;
	Sprite m_sprite;
	Texture m_texture;

	int m_fullWidth;
	int m_fullHeight;

	int m_speed;

public:
	Obstacle();// Default constructor
	Obstacle(float xPos, float yPos, float angle, int fullWidth, int fullHeight);

	void Draw(RenderWindow &window);
	void Update();

	bool CollisionWithSwarm(SwarmEnemy *swarmEnemy);

	void SetAlive(bool myAlive);
	bool GetAlive();

	Sprite GetSprite();
	Vector2f GetPosition();
	Vector2f GetVelocity();

	vector<Obstacle*> CreateObstacle(int width, int height, Vector2f playerPos, vector<Obstacle*> obstacles);
};

#endif