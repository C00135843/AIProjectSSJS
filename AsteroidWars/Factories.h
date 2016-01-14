#pragma once

#include "Player.h"
#include "Pvector.h"
//#include "VectorM.h"
#include <list>

using namespace std;
#define PI 3.141592635

class Factories
{
public:
	Factories(float x, float y);
	~Factories()
	{
		cout << "Deleteing Factory" << endl;
	}
	void LoadAsset();
	void SetPosition(sf::Vector2f pos);
	void SetDirection(sf::Vector2f dir);
	sf::Vector2f  GetPosition();
	
	
	void Update(Player* p,int w,int h);
	void Draw(RenderWindow &window);
	Pvector location;
	Pvector velocity;
	Pvector acceleration;

	bool IsAlive();

private:
	Vector2f m_Position;
	Vector2f m_Direction;
	float m_speed;
	float m_acceleration;


	Vector2f m_velocity;
	Texture m_factoryTexture;
	Sprite m_factorySprite;

	float m_maxSpeed;
	float m_maxForce;

	float m_rotation;
	float rotationSpeed;

	//variables for wandering
	float orientation;
	float angleBetween;

	Pvector flee(sf::Vector2f target); //flee from player
	void Rotate(float t);
	void applyForce(Pvector force);

	void flock(list<Factories*>* v);
	Pvector Separation(list<Factories*>* v);
	Pvector Alignment(list<Factories*>* v);
	Pvector Cohesion(list<Factories*>* v);
	float count = 0;
	void updateflocking(float time);
	Pvector seek(Pvector v);
	Pvector Wander(int,int);
	float angle(Pvector v);
};


