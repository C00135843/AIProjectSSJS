#include <iostream>
#include <vector>
#include "Boid.h"
#include "Obstacle.h"

using namespace std;

#ifndef FLOCK_H_
#define FLOCK_H_

//! Flock Class
/*!
 This file contains the class needed to create flock of boids. It utilizes
 the boids class and initializes boid flocks with parameters that can be
 specified. This class will be utilized in main.
*/
class Flock
{
private:
	int m_id;/*!< int variable m_id */
	bool m_inRange;/*!< bool variable m_inRange */
	bool m_avoid;/*!< bool variable m_avoid */
	vector<Boid>::iterator flockIterator;/*!< vector iterator of type Boid variable flockIterator */
public:
	//Public Variables
	vector<Boid> flock;/*!< vector of type Boid variable flock, contains all Boids */

	Flock() {}/*!< Default constructor */

	// Accessor functions
	int getSize();/*!< int method getSize that returns the number of boids in the flock */
	Boid getBoid(int i);/*!< Boid method getBoid that takes an integer which is used to return the boid that corresponds to that integer */

	bool Avoid();/*!< bool method Avoid that returns the boolean m_avoid */
	void SetAvoid(bool myAvoid);/*!< void method SetAvoid that sets the boolean of m_avoid */

	// Mutator Functions
	void addBoid(Boid b, int id);/*!< void method addBoid that adds a boid to the flock */
	void removeBoid(int id);/*!< void method removeBoid that removes a boid from the flock */
	void flocking();/*!< void method flocking that tells the boids to flock */
	void swarming(Vector2f playerPos, Vector2f playerVel, vector<Obstacle*> obstacles);/*!< void method swarming that tells the boids to swarm */

	/*!
	void method GetDisanceFromPlayer that gets the distance of each boid from the player
	*/
	void GetDisanceFromPlayer(Vector2f &playerPos, int windowHeight);

	/*!
	bool method InRangeOfObstacle that checks whether each boid in the flock is in range of an obstacle
	*/
	bool InRangeOfObstacle(Vector2f &obstalcePos);

	bool InRange();/*!< bool method InRange that returns the m_inRange boolean*/
	void SetInRange(bool myInRange);/*!< void method SetInRange that sets the m_inRange boolean*/
};

#endif