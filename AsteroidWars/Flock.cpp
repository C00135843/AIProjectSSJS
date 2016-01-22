#include "stdafx.h"
#include "Boid.h"
#include "Flock.h"

using namespace std;

int Flock::getSize()
{
	return flock.size();
}

Boid Flock::getBoid(int i)
{
	return flock[i];
}

void Flock::addBoid(Boid b, int id)
{
	SetInRange(false);

	SetAvoid(false);

	flock.push_back(b);
}

void Flock::removeBoid(int id)
{
	for (flockIterator = flock.begin(); flockIterator != flock.end(); ++flockIterator)
	{
		if (flockIterator->GetID() == id)
		{
			flock.erase(flockIterator);
			break;
		}
	}
}

// Runs the run function for every boid in the flock checking against the flock
// itself. Which in turn applies all the rules to the flock.
void Flock::flocking() 
{
	for (int i = 0; i < flock.size(); i++)
	{
		flock[i].run(flock);
	}
}

// Runs the swarm function for every boid in the flock checking against the flock
// itself. Which in turn applies all the rules to the flock.
void Flock::swarming(Vector2f playerPos, Vector2f playerVel, vector<Obstacle*> obstacles)
{
	for (int i = 0; i < flock.size(); i++)
	{
		flock[i].swarm(flock, playerPos, playerVel, obstacles);
	}
}

// Check distance of each boid in the flock from the player
void Flock::GetDisanceFromPlayer(Vector2f &playerPos, int windowHeight)
{
	float distance;

	// Go through each boid in the flock
	for (flockIterator = flock.begin(); flockIterator != flock.end(); ++flockIterator)
	{
		// Get distance
		distance = sqrt(((flockIterator->location.x - playerPos.x)*(flockIterator->location.x - playerPos.x)) + ((flockIterator->location.y - playerPos.y)*(flockIterator->location.y - playerPos.y)));
		
		// Check if in range
		if (distance <= windowHeight/2)
		{
			flockIterator->SetInRange(true);
		}
		else
		{
			flockIterator->SetInRange(false);
		}
	}
}

bool Flock::InRangeOfObstacle(Vector2f &obstalcePos)
{
	float distance;

	// Go through each boid in the flock
	for (flockIterator = flock.begin(); flockIterator != flock.end(); ++flockIterator)
	{
		// Get distance
		distance = sqrt(((flockIterator->location.x - obstalcePos.x)*(flockIterator->location.x - obstalcePos.x)) + ((flockIterator->location.y - obstalcePos.y)*(flockIterator->location.y - obstalcePos.y)));

		// Check if in range
		if (distance <= 150)
		{
			flockIterator->SetAvoid(true); // THIS WILL JUST BE CHANGED!
			//cout << "boid should avoid" << endl;
			return true;
		}
		else
		{
			flockIterator->SetAvoid(false);
			return false;
		}
	}
}

// Getter/Setter for boid to be in range of player
bool Flock::InRange()
{
	return m_inRange;
}
void Flock::SetInRange(bool myInRange)
{
	m_inRange = myInRange;
}

bool Flock::Avoid()
{
	return m_avoid;
}
void Flock::SetAvoid(bool myAvoid)
{
	m_avoid = myAvoid;
}