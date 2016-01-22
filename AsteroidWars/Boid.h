#include "Pvector.h"
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "Obstacle.h"

using namespace std;

#ifndef BOID_H_
#define BOID_H_

//! Boid Class
/*!
//The boid class creates an object which has multiple vectors (location, velocity, acceleration)
//Boids are given certain properties and rules to follow when interacting with other boids.
//They are being used to simulate flocking patterns and properties

For predators:
Predators should act by themselves. Thus, their maxSpeed should be different,
and they should not adhere to the three laws. However, they should adhere to
cohesion -- how should they seek prey? Also, prey should be able to run away
from predators. There are several ways to do this. The easiest way:

Predators are part of the flock. This means that when the boids check
through the vector of other boids, the predators are included. Check if
the boid being looked at is a predator, and if it is, calculate a Pvector
that maximizes separation.
*/
class Boid
{
public:
	int m_id;/*!< int variable m_id */
	int m_inRange;/*!< int variable m_inRange */
	bool predator;/*!< bool variable predator */
	Pvector location;/*!< Pvector variable location */
	Pvector velocity;/*!< Pvector variable velocity */
	Pvector acceleration;/*!< Pvector variable acceleration */
	float maxSpeed;/*!< float variable maxSpeed */
	float maxForce;/*!< float variable maxForce */

	bool InRange();/*!< bool method InRange that returns the m_inRange boolean*/
	void SetInRange(bool myInRange);/*!< void method SetInRange that sets the m_inRange boolean */

	int GetID();/*!< int method GetID that returns the m_id integer*/
	void SetID(int id);/*!< void method SetID that sets the m_id integer*/

	int timeToTend;/*!< int variable timeToTend, tends boids towards the player */
	int timer;/*!< int variable timer*/
	bool tendTowardsPlayer;/*!< bool variable tendTowardsPlayer*/

	bool m_avoid;/*!< bool variable m_avoid, used to determine when to avoid boids*/

	Boid() {}/*!< Default constructor */ 

	/*!
	void Boid method that sets the position/velocity/acceleration/maxpSpeed/maxForce of the boid
	*/
	Boid(float xPos, float yPos, int id)
	{
		SetInRange(false);
		SetID(id);
		SetAvoid(false);

		acceleration = Pvector(0, 0);
		velocity = Pvector( rand()%3-2, rand()%3-2 ); // Allows for range of -2 -> 2
		location = Pvector(xPos, yPos);
		maxSpeed = 3.5f;
		maxForce = 0.5;

		timer = 0;
		timeToTend = 500;
		tendTowardsPlayer = false;
	}

	/*!
	void applyForce method that applies a force to the Boids acceleration
	*/
	void applyForce(Pvector force);

	// Three Laws that boids follow
	Pvector Separation(vector<Boid> Boids);/*!< Pvector method that separates the boids*/
	Pvector Alignment(vector<Boid> Boids);/*!< Pvector method that aligns the boids*/
	Pvector Cohesion(vector<Boid> Boids);/*!< Pvector method that makes the boids follow the cohesion method*/

	//Functions involving SFML and visualisation linking
	Pvector seek(Pvector v);
	void run(vector <Boid> v);
	void update(Pvector v);
	void flock(vector <Boid> v);
	float angle(Pvector v);
	void swarm(vector <Boid> v, Vector2f playerPos, Vector2f playerVel, vector<Obstacle*> obstacles);

	Pvector seekPlayer(Pvector v);
	Pvector pursuePlayer(Pvector pos, Pvector vel);
	Pvector avoid(Pvector obstaclePos, Pvector obstacleVel);
	void Distance(Vector2f obstaclePos);

	bool Avoid();
	void SetAvoid(bool myAvoid);
};

#endif