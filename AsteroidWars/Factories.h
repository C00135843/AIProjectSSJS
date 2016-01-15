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
	Factories(int x, int y);
	~Factories()
	{
		cout << "Deleteing Factory" << endl;
	}
	void LoadAsset();
	void SetPosition(sf::Vector2f pos);
	void SetDirection(sf::Vector2f dir);
	sf::Vector2f  GetPosition();
	bool checkifDead(){ if (health <= 0){ alive = false; } }
	bool getAlive(){ return alive; }
	void DecreaseHealth(){ health--; }
	
	
	void Update(Player* p,int w,int h);
	void Draw(RenderWindow &window);
	Pvector location;
	Pvector velocity;
	Pvector acceleration;

	bool IsAlive();

private:
	Vector2f m_Position;
	Vector2f m_Direction;

	Pvector wtarget;
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

	void applyForce(Pvector force);

	void flock(list<Factories*>* v);
	Pvector Separation(list<Factories*>* v);
	Pvector Alignment(list<Factories*>* v);
	Pvector Cohesion(list<Factories*>* v);
	float count = 0;
	void updateflocking(float time);
	Pvector seek(Pvector v);
	Pvector Wander(int,int,Pvector);
	float angle(Pvector v);

	float health;
	float alive;

};


