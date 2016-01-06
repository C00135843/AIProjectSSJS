// This file defines the boid class. This includes the attributes found in
// boids -- speed, location on the board, acceleration, etc.
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "SFML/Graphics.hpp"
#include "Boid.h"

// Global Variables for borders()
// desktopTemp gets screen resolution of PC running the program
sf::VideoMode desktopTemp = sf::VideoMode::getDesktopMode(); 
const int window_height = desktopTemp.height;
const int window_width = desktopTemp.width;

#define w_height window_height
#define w_width window_width
#define PI 3.141592635

using namespace std;

// Adds force Pvector to current force Pvector
void Boid::applyForce(Pvector force)
{
	acceleration.addVector(force);
}

// Function that checks and modifies the distance
// of a boid if it breaks the law of separation.
Pvector Boid::Separation(vector<Boid> boids)
{
	// If the boid we're looking at is a predator, do not run the separation
	// algorithm
	
	// Distance of field of vision for separation between boids
	float desiredseparation = 20;

	Pvector steer(0, 0);
	int count = 0;
	// For every boid in the system, check if it's too close
	for (int i = 0; i < boids.size(); i++)
	{
		// Calculate distance from current boid to boid we're looking at
		float d = location.distance(boids[i].location);
		// If this is a fellow boid and it's too close, move away from it
		if ((d > 0) && (d < desiredseparation))
		{
			Pvector diff(0,0);
			diff = diff.subTwoVector(location, boids[i].location); 
			diff.normalize();
			diff.divScalar(d);      // Weight by distance
			steer.addVector(diff);
			count++;
		}
		// If current boid is a predator and the boid we're looking at is also
		// a predator, then separate only slightly 
		if ((d > 0) && (d < desiredseparation) && predator == true && boids[i].predator == true)
		{
			Pvector pred2pred(0, 0);
			pred2pred = pred2pred.subTwoVector(location, boids[i].location);
			pred2pred.normalize();
			pred2pred.divScalar(d);
			steer.addVector(pred2pred);
			count++;
		} 
		// If current boid is not a predator, but the boid we're looking at is
		// a predator, then create a large separation Pvector
		else if ((d > 0) && (d < desiredseparation+70) && boids[i].predator == true)
		{
			Pvector pred(0, 0);
			pred = pred.subTwoVector(location, boids[i].location);
			pred.mulScalar(900);
			steer.addVector(pred);
			count++;
		}
	}
	// Adds average difference of location to acceleration
	if (count > 0)
		steer.divScalar((float)count);
	if (steer.magnitude() > 0) 
	{
		// Steering = Desired - Velocity
		steer.normalize();
		steer.mulScalar(maxSpeed);
		steer.subVector(velocity);
		steer.limit(maxForce);
	}
	return steer;
}

// Alignment calculates the average velocity in the field of view and 
// manipulates the velocity of the Boid passed as parameter to adjust to that
// of nearby boids.
Pvector Boid::Alignment(vector<Boid> Boids)
{
	float neighbordist = 50;

	Pvector sum(0, 0);	
	int count = 0;
	for (int i = 0; i < Boids.size(); i++)
	{
		float d = location.distance(Boids[i].location);
		if ((d > 0) && (d < neighbordist)) // 0 < d < 50
		{
			sum.addVector(Boids[i].velocity);
			count++;
		}
	}
	// If there are boids close enough for alignment...
	if (count > 0)
	{
		sum.divScalar((float)count);// Divide sum by the number of close boids (average of velocity)
		sum.normalize();	   		// Turn sum into a unit vector, and
		sum.mulScalar(maxSpeed);    // Multiply by maxSpeed
		// Steer = Desired - Velocity
		Pvector steer; 
		steer = steer.subTwoVector(sum, velocity); //sum = desired(average)  
		steer.limit(maxForce);
		return steer;
	} else {
		Pvector temp(0, 0);
		return temp;
	}
}

// Cohesion finds the average location of nearby boids and manipulates the 
// steering force to move in that direction.
Pvector Boid::Cohesion(vector<Boid> Boids)
{
	float neighbordist = 50;

	Pvector sum(0, 0);

	int count = 0;
	for (int i = 0; i < Boids.size(); i++)
	{
		float d = location.distance(Boids[i].location);
		if ((d > 0) && (d < neighbordist))
		{
			sum.addVector(Boids[i].location);
			count++;
		}
	}
	if (count > 0)
	{
		sum.divScalar(count);
		return seek(sum);
	} else {
		Pvector temp(0,0);
		return temp;
	}
}

// Seek function limits the maxSpeed, finds necessary steering force and
// normalizes the vectors.
Pvector Boid::seek(Pvector v)
{
	Pvector desired;
	desired.subVector(v);  // A vector pointing from the location to the target

	// Normalize desired and scale to maximum speed
	desired.normalize();
	desired.mulScalar(maxSpeed);

	//cout << desired.x << ", " << desired.y << endl;

	// Steering = Desired minus Velocity
	acceleration.subTwoVector(desired, velocity);
	acceleration.limit(maxForce);  // Limit to maximum steering force
	return acceleration;
}

//Update modifies velocity, location, and resets acceleration with values that
//are given by the three laws.
void Boid::update(Pvector v)
{
	/*timer++;
	cout << timer << endl;
	if (timer >= timeToTend)
	{
		timer = 0;
		tendTowardsPlayer = true;
	}
	else
	{
		tendTowardsPlayer = false;
	}*/


	//To make the slow down not as abrupt
	acceleration.mulScalar(.4); // NI

	// Update velocity
	velocity.addVector(acceleration);

	// Limit speed
	velocity.limit(maxSpeed);// NI
	location.addVector(velocity);// // NI Updates position

	// Reset accelertion to 0 each cycle
	acceleration.mulScalar(0);// NI
}

//Run runs flock on the flock of boids for each boid.
//Which applies the three rules, modifies accordingly, updates data, checks if data is
//out of range, fixes that for SFML, and renders it on the window.
void Boid::run(vector <Boid> v)
{
	flock(v);
	
	Pvector temp;

	update(temp);
}

// Applies all three laws for the flock of boids and modifies to keep them from
// breaking the laws.
void Boid::flock(vector<Boid> v) 
{
	Pvector sep = Separation(v);
	Pvector ali = Alignment(v);
	Pvector coh = Cohesion(v);

	// Arbitrarily weight these forces
	sep.mulScalar(1.5);
	ali.mulScalar(1.0); // Might need to alter weights for different characteristics
	coh.mulScalar(1.0);

	// Add the force vectors to acceleration
	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
}

// Calculates the angle for the velocity of a boid which allows the visual 
// image to rotate in the direction that it is going in.
float Boid::angle(Pvector v)
{
	// From the definition of the dot product
	float angle = (float)(atan2(v.x, -v.y) * 180 / PI);
	return angle;
}

void Boid::swarm(vector <Boid> v, Vector2f playerPos, Vector2f playerVel)
{
	Pvector playerPosVector;
	playerPosVector.x = playerPos.x;
	playerPosVector.y = playerPos.y;

	Pvector playerVelVector;
	playerVelVector.x = playerVel.x;
	playerVelVector.y = playerVel.y;

	Pvector seekVector;

	if (!InRange())
		seekVector = seekPlayer(playerPosVector);
	else
		seekVector = pursuePlayer(playerPosVector, playerVelVector);

	Pvector	R;// 'R' is distance between current boid and boid being looked at in for loop
	Pvector sum(0, 0);

	// Constants
	int A = 100;// Strength of attraction
	int B = 5000;// Strength of repulsion
	int N = 1;
	int M = 2;

	// Go through each boid in the flock
	for (int i = 0; i < v.size(); i++)
	{
		// Compare location of current boid to all the other boids in the flock
		if (!(location.x == v[i].location.x && location.y == v[i].location.y))// If locations are NOT equal
		{
			R = location;
			R.subVector(v[i].location);

			float D = R.magnitude();

			// U is the potential energy of attraction and repulsion.
			float U = -A / pow(D, N) + B / pow(D, M);// 'pow' is 'power' e.g. 2 to the power of 2 = 4

			R.normalize();
			R.mulScalar(U);

			sum.addVector(R);
		}
	}

	sum.divScalar(v.size());

	applyForce(sum);
	applyForce(seekVector);// Makes boid go towards player

	update(playerPosVector);
}

// Seek algorithm implemented
Pvector Boid::seekPlayer(Pvector v)
{
	Pvector desired_velocity;
	desired_velocity.x = v.x - location.x;
	desired_velocity.y = v.y - location.y;

	desired_velocity.normalize();
	desired_velocity.mulScalar(maxSpeed);

	Pvector steering;
	steering.x = desired_velocity.x - velocity.x;
	steering.y = desired_velocity.y - velocity.y;

	// Intercept player if in range(i.e move fast)
	if (InRange())
	{
		steering.x = steering.x / 5;
		steering.y = steering.y / 5;
	}
	else // Tend towards player(i.e. move slowly)
	{
		steering.x = steering.x / 100;
		steering.y = steering.y / 100;
	}

	steering.limit(maxForce);

	return steering;
}

Pvector Boid::pursuePlayer(Pvector pos, Pvector vel)
{
	Pvector direction;
	direction.x = pos.x - location.x;
	direction.y = pos.y - location.y;

	Pvector distance;
	distance = direction;

	float distanceScalar = distance.magnitude();

	Pvector speed;
	speed = velocity;
	speed.magnitude();

	float speedScaler = speed.magnitude();

	float maxTimePrediction = 3.5f;
	float timePrediction;

	if (speedScaler <= (distanceScalar / maxTimePrediction))
	{
		timePrediction = maxTimePrediction;
	}
	else
	{
		timePrediction = distanceScalar / speedScaler;
	}

	Pvector newTarget;
	newTarget.x = pos.x + vel.x * timePrediction;
	newTarget.y = pos.y + vel.y * timePrediction;

	Pvector seekVector;
	seekVector = seekPlayer(newTarget);

	return seekVector;
		
}

#pragma region Getters/Setters

bool Boid::InRange()
{
	return m_inRange;
}

void Boid::SetInRange(bool myInRange)
{
	m_inRange = myInRange;
}

int Boid::GetID()
{
	return m_id;
}

void Boid::SetID(int id)
{
	m_id = id;
}

#pragma endregion

