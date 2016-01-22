#include "stdafx.h"
#include "Factories.h"

Factories::Factories(int x, int y)
{
	// Random position
	m_Position.x = rand() % x + 1;
	m_Position.y = rand() % y + 1;

	// Screen width and height
	SCREEN_WIDTH = x * 9;
	SCREEN_HEIGHT = y * 9;

	wander = true;
	m_avoid = false;
	LoadAsset();
	acceleration = Pvector(0, 0);
	location = Pvector(m_Position.x, m_Position.y);

	m_maxSpeed = 3.0;

	orientation = 0;
	angleBetween = 0;
	m_maxForce = 0.5;
	acceleration = Pvector(0, 0);
	velocity = Pvector(rand() % 3 - 2, rand() % 3 - 2); // Allows for range of -2 -> 2
	health = 4;
	alive = true;
	flocker = false;
	m_health = 100;

	m_fire = false;

	fireTimer = 100;
	fireTime = 200;

	oldVelocity = velocity;

	m_createPredator = false;
}

void Factories::LoadAsset()
{
	m_factoryTexture.loadFromFile("Pics/FactorySpaceShip.png");
	m_factorySprite.setTexture(m_factoryTexture);

	m_radarTexture.loadFromFile("Pics/FactoryRadar.png");
	m_radarSprite = sf::Sprite(m_radarTexture);
	m_radarSprite.setScale(4.0f, 4.0f);

	//set Origin
	m_factorySprite.setOrigin(m_factoryTexture.getSize().x / 2.f, m_factoryTexture.getSize().y / 2.f);

	//set Scale
	m_factorySprite.setScale(.2, .2);

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

void Factories::Update(Vector2f playerPos, int w, int h, vector<Factories*>* v, Pvector flockPos, vector<Obstacle*> obstacles)
{
	timeSinceLastUpdate = m_clock.getElapsedTime();
	float time = timeSinceLastUpdate.asSeconds();

	if (time > 5.0f)
		SetCreatePredator(true);

	Pvector fPos(m_Position.x, m_Position.y);
	Pvector pPos(playerPos.x, playerPos.y);

	// if the factory gets in range of the player evade
	if (fPos.distance(pPos) > 200 && fPos.distance(pPos) < 600 && fireTimer >= fireTime)// FIRE
	{
		SetFire(true);
		fireTimer = 0;
	}
	else if (fPos.distance(pPos) < 200) // EVADE
	{
		applyForce(flee(playerPos));
		SetFire(false);
	}
	else if (wander) // WANDER
	{
		Pvector wanderVel = Wander(w, h, flockPos);
		applyForce(wanderVel);
		SetFire(false);
	}
	else // FLOCK
	{
		flock(v);
		SetFire(false);
	}

	Pvector avoidVector;
	avoidVector.x = 0;
	avoidVector.y = 0;

	for each(Obstacle* obstacle in obstacles)
	{
		if (obstacle->getInRangeOfFactory())
		{
			Pvector obstaclePos;
			obstaclePos.x = obstacle->GetPosition().x;
			obstaclePos.y = obstacle->GetPosition().y;

			Pvector obstalceVel;
			obstalceVel.x = obstacle->GetVelocity().x;
			obstalceVel.y = obstacle->GetVelocity().y;

			avoidVector = avoid(obstaclePos, obstalceVel);
		}
	}

	// If in range fire missile
	if (Fire())
	{
		if (m_factoryMissiles.size() < 5)
			CreateMissile(playerPos);
		SetFire(false);
	}

	fireTimer++;

	acceleration.mulScalar(.4);

	// Update velocity
	velocity.addVector(acceleration);

	// Limit speed
	velocity.limit(m_maxSpeed);
	location.addVector(velocity);

	// Reset accelertion to 0 each cycle
	acceleration.mulScalar(0);

	m_Position = sf::Vector2f(location.x, location.y);
	m_Position = Vector2f(mod(m_Position.x, w), mod(m_Position.y, h));

	float a = angle(velocity);
	m_factorySprite.setRotation(a);
	m_factorySprite.setPosition(m_Position);

	// Update missiles
	if (m_factoryMissiles.size() > 0)
	{
		// Iterate through list of bullets
		for (m_missileIterator = m_factoryMissiles.begin(); m_missileIterator != m_factoryMissiles.end(); ++m_missileIterator)
		{
			(*m_missileIterator)->Update(playerPos);

			// Remove bullet if out of bounds
			if (!(*m_missileIterator)->GetAlive())
			{
				m_factoryMissiles.erase(m_missileIterator);
				break;
			}
		}
	}
	applyForce(avoidVector);
}

void Factories::CreateMissile(Vector2f playerPos)
{
	// Missile position
	Vector2f missilePos = m_Position;

	// Missile velocity(points towards player initially)
	Vector2f missileVel;
	missileVel = playerPos - m_Position;
	float length = sqrt((missileVel.x * missileVel.x) + (missileVel.y * missileVel.y));
	missileVel = missileVel / length;

	FactoryMissile* factoryMissile = new FactoryMissile(missilePos, missileVel);

	m_factoryMissiles.push_back(factoryMissile);
}

float Factories::angle(Pvector v)
{
	// From the definition of the dot product
	float angle = (float)(atan2(v.x, -v.y) * 180 / PI);
	return angle;
}

Pvector Factories::Wander(int w, int h, Pvector p)
{
	if (count == 0)
	{
		wtarget.x = rand() % w + 1;
		wtarget.y = rand() % h + 1;
	}
	else if (count >= 120)
	{
		wtarget.x = rand() % w + 1;
		wtarget.y = rand() % h + 1;
		count = 1;
	}

	count++;
	Pvector targetForce(seek(wtarget));
	return targetForce;
}

Pvector Factories::flee(sf::Vector2f t)
{
	Pvector desired_velocity;
	Pvector steering;
	Pvector normVel(m_Position.x - t.x, m_Position.y - t.y);
	normVel.normalize();

	desired_velocity.x = normVel.x * m_maxSpeed;
	desired_velocity.y = normVel.y * m_maxSpeed;

	steering = steering.subTwoVector(desired_velocity, velocity);
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
	Pvector normVel(t.x - m_Position.x, t.y - m_Position.y);
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

	// Draw bullets
	if (m_factoryMissiles.size() > 0)
	{
		for each(FactoryMissile* factoryMissile in m_factoryMissiles)
		{
			factoryMissile->Draw(win);
		}
	}
}

void Factories::DrawOnRadar(RenderWindow &win)
{
	m_radarSprite.setPosition(m_factorySprite.getPosition());
	win.draw(m_radarSprite);
}

Pvector Factories::Face(Vector2f playerPos)
{
	Pvector desired_velocity;
	Pvector steering;
	Pvector normVel(m_Position.x - playerPos.x, m_Position.y - playerPos.y);
	normVel.normalize();

	desired_velocity.x = normVel.x * m_maxSpeed;
	desired_velocity.y = normVel.y * m_maxSpeed;

	steering = steering.subTwoVector(desired_velocity, velocity);
	steering.limit(m_maxForce);
	return steering;
}

Pvector Factories::avoid(Pvector obstalcePos, Pvector obstacleVel)
{
	Pvector ahead;
	Pvector ahead2;

	Pvector obstacleCentre;
	obstacleCentre.x = obstalcePos.x + (35.75f / 2);
	obstacleCentre.y = obstalcePos.y + (39.25f / 2);

	float radius = 35.75f / 2;

	float MAX_SEE_AHEAD = 150;

	Pvector normalizedVelocity;
	normalizedVelocity = velocity;
	normalizedVelocity.normalize();

	Pvector lengthVelocity;
	lengthVelocity = velocity;
	lengthVelocity.normalize();

	Pvector dynamic_length;
	dynamic_length.x = lengthVelocity.x / m_maxSpeed;
	dynamic_length.y = lengthVelocity.y / m_maxSpeed;

	ahead.x = location.x + normalizedVelocity.x * dynamic_length.x;
	ahead.y = location.y + normalizedVelocity.y * dynamic_length.y;

	ahead2.x = location.x + normalizedVelocity.x * dynamic_length.x * 0.5f;
	ahead2.y = location.y + normalizedVelocity.y * dynamic_length.y * 0.5f;

	float distance = ahead.distance(obstacleCentre);
	float distance2 = ahead2.distance(obstacleCentre);

	bool intersectedCircle = false;

	if (distance <= radius || distance2 <= radius)
	{
		intersectedCircle = true;
	}

	Pvector avoidance_force;

	avoidance_force.x = ahead.x - obstacleCentre.x;
	avoidance_force.y = ahead.y - obstacleCentre.y;

	float MAX_AVOID_FORCE = 30.5f;

	Pvector normalized_avoidance_force;
	normalized_avoidance_force = avoidance_force;
	normalized_avoidance_force.normalize();

	avoidance_force.x = normalized_avoidance_force.x * MAX_AVOID_FORCE;
	avoidance_force.y = normalized_avoidance_force.y * MAX_AVOID_FORCE;

	Pvector steering;
	steering = avoidance_force;
	steering.limit(m_maxForce);

	return steering;
}


bool Factories::inRangeOfObs(Vector2f obstalcePos)
{
	float distance = sqrt(((m_Position.x - obstalcePos.x)*(m_Position.x - obstalcePos.x)) + ((m_Position.y - obstalcePos.y)*(m_Position.y - obstalcePos.y)));

	// Check if in range
	if (distance <= 150)
	{
		SetAvoid(true); // THIS WILL JUST BE CHANGED!
		//cout << "boid should avoid" << endl;
		return true;
	}
	else
	{
		SetAvoid(false);
		return false;
	}
}

bool Factories::FactoryMissilePlayerCollision(Sprite&playerSprite)
{
	if (m_factoryMissiles.size() > 0)
	{
		// Iterate through list of bullets
		for (m_missileIterator = m_factoryMissiles.begin(); m_missileIterator != m_factoryMissiles.end(); ++m_missileIterator)
		{
			if ((*m_missileIterator)->GetSprite().getGlobalBounds().intersects(playerSprite.getGlobalBounds()))
			{
				m_factoryMissiles.erase(m_missileIterator);
				return true;
				break;
			}
		}
	}
	return false;
}

#pragma region Getters/Setters

int Factories::GetHealth()
{
	return m_health;
}

void Factories::SetHealth(int myHealth)
{
	m_health = myHealth;
}

Sprite Factories::GetSprite()
{
	return m_factorySprite;
}

bool Factories::Fire()
{
	return m_fire;
}

void Factories::SetFire(bool myFire)
{
	m_fire = myFire;
}

bool Factories::GetCreatePredator()
{
	return m_createPredator;
}

void Factories::SetCreatePredator(bool myCreatePredator)
{
	if (!myCreatePredator)
	{
		m_clock.restart();
	}

	m_createPredator = myCreatePredator;
}

#pragma endregion

