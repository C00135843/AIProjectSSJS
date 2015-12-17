#include "stdafx.h"
#include "Factories.h"
//#include <cmath>


Factories::Factories(float x, float y) :m_position(x,y)
{
	LoadAsset();
}
Factories::~Factories()
{
}
void Factories::LoadAsset()
{
	m_factoryTexture.loadFromFile("Pics/FactorySpaceShip.png");
	m_factorySprite.setTexture(m_factoryTexture);
	//set Origin
	m_factorySprite.setOrigin(m_factoryTexture.getSize().x / 2.f, m_factoryTexture.getSize().y / 2.f);
	//set Scale
	m_factorySprite.setScale(.1, .1);
	//set rotation
	m_rotation = 45;
	m_factorySprite.rotate(m_rotation);
	//set Position
	m_factorySprite.setPosition(m_position);
	//set Speed and Velocity
	m_speed = .50f;
	m_rotationSpeed = 3.f;
	m_velocity = Vector2f(0.1f, 0.1f);

}
void Factories::Update()
{
	Wander();
	m_factorySprite.setPosition(m_position);
}
void Factories::Wander()
{
	// *3.14159265 / 180
	float LO = -1,HI=1;
	float r = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	wanderOrientation += r * wanderRate;
	float targetOrientation = wanderOrientation + m_factorySprite.getRotation();
	
	
	Vector2f myOrientVector = Vector2f((float)sin(m_factorySprite.getRotation()*3.14159265 / 180), -(float)cos(m_factorySprite.getRotation()*3.14159265 / 180));
	Vector2f targetOrientVector = Vector2f((float)sin(targetOrientation*3.14159265 / 180), -(float)cos(targetOrientation*3.14159265 / 180));
	
	Vector2f target = m_factorySprite.getPosition() + wanderOffset *  myOrientVector;
	target += wanderRadius * targetOrientVector;
	float t = atan2(target.x, target.y)*180/3.14159;
	m_factorySprite.setRotation(t);
	m_velocity = maxAcc * Vector2f((float)sin(m_factorySprite.getRotation()*3.14159265 / 180), -(float)cos(m_factorySprite.getRotation()*3.14159265 / 180));
	m_position += m_velocity;

}
void Factories::Draw(RenderWindow &win)
{
	win.draw(m_factorySprite);
}
