#include <iostream>
#include <vector>
#include "Boid.h"
#include "Obstacle.h"

using namespace std;

#ifndef FLOCK_H_
#define FLOCK_H_

/*
Brief description of Flock Class:
// This file contains the class needed to create flock of boids. It utilizes
// the boids class and initializes boid flocks with parameters that can be
// specified. This class will be utilized in main.
*/

class Flock
{
private:
	int m_id;
	bool m_inRange;
	bool m_avoid;
	vector<Boid>::iterator flockIterator;
public:
	//Public Variables
	vector<Boid> flock;

	//Constructors
	Flock() {}

	// Accessor functions
	int getSize();
	Boid getBoid(int i);

	bool Avoid();
	void SetAvoid(bool myAvoid);

	// Mutator Functions
	void addBoid(Boid b, int id);
	void removeBoid(int id);
	void flocking();
	void swarming(Vector2f playerPos, Vector2f playerVel, vector<Obstacle*> obstacles);

	void GetDisanceFromPlayer(Vector2f &playerPos, int windowHeight);
	bool InRangeOfObstacle(Vector2f &obstalcePos);

	bool InRange();
	void SetInRange(bool myInRange);
};

#endif