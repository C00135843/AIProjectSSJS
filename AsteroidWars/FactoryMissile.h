#pragma once
#include "Pvector.h"
#define PI 3.141592635

class FactoryMissile
{
private:
	Clock m_clock;
	Time timeSinceLastUpdate;

	Vector2f m_position;
	Vector2f m_velocity;

	Texture m_texture;
	Sprite m_sprite;

	float m_speed;
	Vector2f seek(Pvector t);
	bool alive;
public:
	FactoryMissile(Vector2f position, Vector2f velocity);

	void Draw(RenderWindow &window);
	void Update(Vector2f &p);

	bool GetAlive();
	void SetAlive(bool myAlive);

	Sprite GetSprite();
};