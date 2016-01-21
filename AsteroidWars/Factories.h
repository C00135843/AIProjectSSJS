#pragma once

#include "Pvector.h"
#include "FactoryMissile.h"
#include "Predator.h"
#include "Obstacle.h"
#include <list>

using namespace std;
#define PI 3.141592635

class Factories
{
public:
	Factories(int x, int y);
	~Factories()
	{
		cout << "Deleting Factory" << endl;
	}
	void LoadAsset();
	void SetPosition(sf::Vector2f pos);
	void SetDirection(sf::Vector2f dir);
	sf::Vector2f  GetVelocity(){ return Vector2f(velocity.x, velocity.y); };
	sf::Vector2f  GetPosition(){ return m_Position; };
	bool checkifDead(){ if (health <= 0){ alive = false; } }
	bool getAlive(){ return alive; }
	void DecreaseHealth(){ health--; }
	void flock(vector<Factories*>* v);

	void Update(Vector2f playerPos, int w, int h, vector<Factories*>* v, Pvector flockPos, vector<Obstacle*> obstacles);
	void Draw(RenderWindow &window);
	void DrawOnRadar(RenderWindow &window);
	Pvector location;
	Pvector velocity;
	Pvector acceleration;
	void setFlocker(bool f){ flocker = f; }
	bool IsAlive();
	void setWander(bool w){ wander = w; }

	int GetHealth();
	void SetHealth(int myHealth);

	bool Fire();
	void SetFire(bool myFire);

	void CreateMissile(Vector2f playerPos);
	bool GetCreatePredator();
	void SetCreatePredator(bool myCreatePredator);
	bool m_avoid;
	Sprite GetSprite();

	Pvector Face(Vector2f playerPos);

	bool FactoryMissilePlayerCollision(Sprite&playerSprite);

	bool inRangeOfObs(Vector2f);

	void SetAvoid(bool a){ m_avoid = a; }

private:
	Vector2f m_Position;
	Vector2f m_Direction;

	Clock m_clock;
	Time timeSinceLastUpdate;

	Pvector oldVelocity;

	Pvector wtarget;
	Texture m_factoryTexture;
	Sprite m_factorySprite;

	Texture m_radarTexture;
	Sprite m_radarSprite;

	vector<Factories*>* factoriesInFlock;

	list<FactoryMissile*> m_factoryMissiles;
	std::list<FactoryMissile*>::iterator m_missileIterator;

	list<Predator*> m_predators;
	std::list<Predator*>::iterator m_predatorIterator;

	float SCREEN_WIDTH;
	float SCREEN_HEIGHT;

	float m_maxSpeed;
	float m_maxForce;

	float m_rotation;
	float rotationSpeed;

	int m_health;

	bool m_fire;
	bool m_createPredator;

	int fireTimer;
	int fireTime;

	//variables for wandering
	float orientation;
	float angleBetween;

	Pvector flee(sf::Vector2f target); //flee from player

	void applyForce(Pvector force);
	float mod(float, float);

	bool wander;
	Pvector Separation(vector<Factories*>* v);
	Pvector Alignment(vector<Factories*>* v);
	Pvector Cohesion(vector<Factories*>* v);
	float count = 0;
	Pvector seek(Pvector v);
	Pvector flockSeek(Pvector v);
	Pvector Wander(int, int, Pvector);
	Pvector avoid(Pvector obstalcePos, Pvector obstacleVel);
	float angle(Pvector v);
	bool flocker;
	float health;
	float alive;

};

