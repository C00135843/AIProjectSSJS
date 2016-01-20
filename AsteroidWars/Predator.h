#pragma once

#include <list>
#include "Pvector.h"

class Predator
{
private:
	Pvector m_acceleration;
	Pvector m_position;
	Pvector m_velocity;

	Texture m_texture;
	Sprite m_sprite;

	Texture m_radarTexture;
	Sprite m_radarSprite;

	float maxSpeed;
	float maxForce;

	bool m_alive;

	bool predator;

public:
	Predator(Vector2f position);

	void Draw(RenderWindow &window);
	void DrawOnRadar(RenderWindow &window);
	void Update(vector<Predator*>* v, Pvector &playerPos, int width, int height);

	bool GetAlive();
	void SetAlive(bool myAlive);

	sf::Vector2f  GetPosition(){ return Vector2f(m_position.x, m_position.y); };

	Sprite GetSprite();

	void applyForce(Pvector force);
	float mod(float, float);

	// Three Laws that boids follow
	Pvector Separation(vector<Predator*>* v);
	Pvector Alignment(vector<Predator*>* v);
	Pvector Cohesion(vector<Predator*>* v);

	//Functions involving SFML and visualisation linking
	Pvector flockSeek(Pvector v);
	void run(vector <Predator> v);
	void flock(vector<Predator*>* v);
	float angle(Pvector v);
};