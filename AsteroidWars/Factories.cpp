#include "stdafx.h"
#include "Factories.h"

Factories::Factories(int x, int y) 
{
	m_Position.x = rand() % x + 1;
	m_Position.y = rand() % y + 1;
	//m_Position.x = 500;
	//m_Position.y = 300;
	SCREEN_WIDTH = x * 9;
	SCREEN_HEIGHT = y * 9;
	wander = true;
	LoadAsset();
	acceleration = Pvector(0, 0);
	location = Pvector(m_Position.x, m_Position.y);
	//m_speed = 200;
	m_maxSpeed = 5.0;
	//rotationSpeed = 150;
	orientation = 0;
	angleBetween = 0;
	m_maxForce = 0.5;
	acceleration = Pvector(0, 0);
	velocity = Pvector(rand() % 3 - 2, rand() % 3 - 2); // Allows for range of -2 -> 2
	health = 4;
	alive = true;
	flocker = false;
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
	m_factorySprite.setPosition(m_Position);



}
float Factories::mod(float a, float b)
{

		float thing = a - b * floor(a / b);
		return a - b * floor(a / b);

}
void Factories::applyForce(Pvector force)
{
	acceleration.addVector(force);
}

void Factories::Update(Player* p, int w, int h, vector<Factories*>* v, Pvector flockPos)
{
	Pvector fPos(m_Position.x, m_Position.y);
	Pvector pPos(p->GetPosition().x, p->GetPosition().y);

	if (fPos.distance(pPos) < 200) // if the factory gets in range of the player evade
	{
		applyForce(flee(p->GetPosition()));

	}
	else // wander or flock
	{
		if (wander)
		{
			Pvector wanderVel = Wander(w, h, flockPos);
			applyForce(wanderVel);
		}
		else{

			flock(v);
		}

	}
	acceleration.mulScalar(.4); // NI
	// Update velocity
	velocity.addVector(acceleration);
	// Limit speed
	velocity.limit(m_maxSpeed);// NI
	location.addVector(velocity);// // NI Updates position
	// Reset accelertion to 0 each cycle
	acceleration.mulScalar(0);// NI
	//Boundary();
	m_Position = sf::Vector2f(location.x, location.y);
	m_Position = Vector2f(mod(m_Position.x,w), mod(m_Position.y,h));
	float a = angle(velocity);
	m_factorySprite.setRotation(a);
	m_factorySprite.setPosition(m_Position);
}

float Factories::angle(Pvector v)
{
	// From the definition of the dot product
	float angle = (float)(atan2(v.x, -v.y) * 180 / PI);
	return angle;
}

Pvector Factories::Wander(int w, int h,Pvector p)
{
	
		if (count == 0)
		{
			wtarget.x = rand() % w + 1;
			wtarget.y = rand() % h + 1;
		}
		else if (count >= 60)
		{
			wtarget.x = rand() % w + 1;
			wtarget.y = rand() % h + 1;
			count = 1;
		}

	count++;
	Pvector targetForce(seek(wtarget));
	return targetForce;
}


Pvector Factories::flee(sf::Vector2f t){
	Pvector desired_velocity;
	Pvector steering;
	Pvector normVel(m_Position.x - t.x,m_Position.y-t.y);
	normVel.normalize();

	desired_velocity.x = normVel.x * m_maxSpeed;
	desired_velocity.y = normVel.y * m_maxSpeed;

	steering = steering.subTwoVector( desired_velocity, velocity);
	steering.limit(m_maxForce);
	return steering;
}

void Factories::flock(vector<Factories*>* v)
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
void Factories::updateflocking(float time)
{

	//To make the slow down not as abrupt
	acceleration.mulScalar(.4);
	// Update velocity
	velocity.addVector(acceleration);
	// Limit speed
	velocity.limit(m_maxSpeed * time);
	location.addVector(velocity);
	// Reset accelertion to 0 each cycle
	acceleration.mulScalar(0);
}
Pvector Factories::Separation(vector<Factories*>* v)
{
	// If the boid we're looking at is a predator, do not run the separation
	// algorithm

	// Distance of field of vision for separation between boids
	float desiredseparation = 50;

	Pvector steer(0, 0);
	int count = 0;
	// For every factory in the system, check if it's too close
	for each(Factories* f in *v)
	{
		float d = location.distance(f->location);
		if ((d > 0) && (d < desiredseparation))
		{
			Pvector diff(0, 0);
			diff = diff.subTwoVector(location, f->location);
			diff.normalize();
			diff.divScalar(d);      // Weight by distance
			steer.addVector(diff);
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
		steer.mulScalar(m_maxSpeed);
		steer.subVector(velocity);
		steer.limit(m_maxForce);
	}
	return steer;
}

// Alignment calculates the average velocity in the field of view and 
// manipulates the velocity of the Boid passed as parameter to adjust to that
// of nearby factories.
Pvector Factories::Alignment(vector<Factories*>* v)
{
	float neighbordist = 100;

	Pvector sum(0, 0);
	int count = 0;
	for each(Factories* f in *v)
	{
		float d = location.distance(f->location);
		if ((d > 0) && (d < neighbordist)) // 0 < d < 50
		{
			sum.addVector(f->velocity);
			count++;
		}
	}
	// If there are boids close enough for alignment...
	if (count > 0)
	{
		sum.divScalar((float)count);// Divide sum by the number of close boids (average of velocity)
		sum.normalize();	   		// Turn sum into a unit vector, and
		sum.mulScalar(m_maxSpeed);    // Multiply by maxSpeed
		// Steer = Desired - Velocity
		Pvector steer;
		steer = steer.subTwoVector(sum, velocity); //sum = desired(average)  
		steer.limit(m_maxForce);
		return steer;
	}
	else {
		Pvector temp(0, 0);
		return temp;
	}
}
// Cohesion finds the average location of nearby factories and manipulates the 
// steering force to move in that direction.
Pvector Factories::Cohesion(vector<Factories*>* v)
{
	float neighbordist = 100;

	Pvector sum(0, 0);
	int count = 0;
	for each(Factories* f in *v)
	{
		float d = location.distance(f->location);
		if ((d > 0) && (d < neighbordist))
		{
			sum.addVector(f->location);
			count++;
		}
	}
	if (count > 0)
	{
		sum.divScalar(count);
		return flockSeek(sum);
	}
	else {
		Pvector temp(0, 0);
		return temp;
	}
}
Pvector Factories::flockSeek(Pvector v)
{
	Pvector desired;
	desired.subVector(v);  // A vector pointing from the location to the target
	// Normalize desired and scale to maximum speed
	desired.normalize();
	desired.mulScalar(m_maxSpeed);
	// Steering = Desired minus Velocity
	acceleration.subTwoVector(desired, velocity);
	acceleration.limit(m_maxForce);  // Limit to maximum steering force
	return acceleration;
}
// Seek function limits the maxSpeed, finds necessary steering force and
// normalizes the vectors.
Pvector Factories::seek(Pvector t)
{
	Pvector desired_velocity;
	Pvector steering;
	Pvector normVel(t.x - m_Position.x, t.y- m_Position.y);
	normVel.normalize();

	desired_velocity.x = normVel.x * m_maxSpeed;
	desired_velocity.y = normVel.y * m_maxSpeed;

	steering = steering.subTwoVector(desired_velocity, velocity);
	steering.limit(m_maxForce);
	return steering;
}
void Factories::Draw(RenderWindow &win)
{
	win.draw(m_factorySprite);
}
