#pragma once

#include "stdafx.h"
class Factories
{
public:
	Factories(float x, float y);
	~Factories();
	void LoadAsset();
	void Update();
	void Draw(RenderWindow &window);
	void Wander();

private:
	Vector2f m_position;
	Vector2f m_velocity;
	Texture m_factoryTexture;
	Sprite m_factorySprite;

	float const m_maxSpeed = 10.f;
	float m_speed;
	float m_rotation;
	float m_rotationSpeed;

	//variables for wandering
	float wanderOffset = 1200.f;
	float wanderRadius = 600.f;
	float wanderRate = 5.f;
	float wanderOrientation = 1000.f;
	float maxAcc = 1.f;

};

