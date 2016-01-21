#include "stdafx.h"
#include "Predator.h"

#define PI 3.141592635

Predator::Predator(Vector2f position)
{
	m_acceleration = Pvector(0, 0);
	m_velocity = Pvector(rand() % 3 - 2, rand() % 3 - 2); // Allows for range of -2 -> 2;
	m_position = Pvector(position.x, position.y);
	stop = false;
	m_texture.loadFromFile("Pics/Predator.png");
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_texture.getSize().x / 2.f, m_texture.getSize().y / 2.f);

	m_radarTexture.loadFromFile("Pics/Red.png");
	m_radarSprite.setTexture(m_radarTexture);

	m_sprite.setPosition(Vector2f(m_position.x, m_position.y));
	m_sprite.setScale(0.75f, 0.75f);

	m_alive = true;

	maxSpeed = 3.5f;
	maxForce = 0.5;
}

float Predator::mod(float a, float b)
{
	float thing = a - b * floor(a / b);
	return a - b * floor(a / b);
}

void Predator::Draw(RenderWindow &window)
{
	window.draw(m_sprite);

	if (m_bullets.size() > 0)
	{
		for each(Bullet* bullet in m_bullets)
		{
			bullet->Draw(window);
		}
	}
}

void Predator::DrawOnRadar(RenderWindow &window)
{
	m_radarSprite.setPosition(Vector2f(m_position.x, m_position.y));
	window.draw(m_radarSprite);
}

// Adds force Pvector to current force Pvector
void Predator::applyForce(Pvector force)
{
	m_acceleration.addVector(force);
}

// Function that checks and modifies the distance
// of a boid if it breaks the law of separation.
Pvector Predator::Separation(vector<Predator*>* v)
{
	// If the boid we're looking at is a predator, do not run the separation
	// algorithm

	// Distance of field of vision for separation between boids
	float desiredseparation = 65;

	Pvector steer(0, 0);
	int count = 0;
	// For every factory in the system, check if it's too close
	for each(Predator* f in *v)
	{
		float d = m_position.distance(f->m_position);
		if ((d > 0) && (d < desiredseparation))
		{
			Pvector diff(0, 0);
			diff = diff.subTwoVector(m_position, f->m_position);
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
		steer.mulScalar(maxSpeed);
		steer.subVector(m_velocity);
		steer.limit(maxForce);
	}
	return steer;
}

// Alignment calculates the average velocity in the field of view and 
// manipulates the velocity of the Boid passed as parameter to adjust to that
// of nearby boids.
Pvector Predator::Alignment(vector<Predator*>* v)
{
	float neighbordist = 100;

	Pvector sum(0, 0);
	int count = 0;
	for each(Predator* f in *v)
	{
		float d = m_position.distance(f->m_position);
		if ((d > 0) && (d < neighbordist)) // 0 < d < 50
		{
			sum.addVector(f->m_velocity);
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
		steer = steer.subTwoVector(sum, m_velocity); //sum = desired(average)  
		steer.limit(maxForce);
		return steer;
	}
	else {
		Pvector temp(0, 0);
		return temp;
	}
}

// Cohesion finds the average location of nearby boids and manipulates the 
// steering force to move in that direction.
Pvector Predator::Cohesion(vector<Predator*>* v)
{
	float neighbordist = 100;

	Pvector sum(0, 0);
	int count = 0;
	for each(Predator* f in *v)
	{
		float d = m_position.distance(f->m_position);
		if ((d > 0) && (d < neighbordist))
		{
			sum.addVector(f->m_position);
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

Pvector Predator::flockSeek(Pvector v)
{
	Pvector desired_velocity;
	Pvector steering;
	Pvector normVel(v.x - m_position.x, v.y - m_position.y);
	normVel.normalize();

	desired_velocity.x = normVel.x * maxSpeed;
	desired_velocity.y = normVel.y * maxSpeed;

	steering = steering.subTwoVector(desired_velocity, m_velocity);
	steering.limit(maxForce);
	return steering;
}

//Run runs flock on the flock of boids for each boid.
//Which applies the three rules, modifies accordingly, updates data, checks if data is
//out of range, fixes that for SFML, and renders it on the window.
void Predator::run(vector <Predator> v)
{
	//flock(v);
}

//Update modifies velocity, location, and resets acceleration with values that
//are given by the three laws.
void Predator::Update(vector<Predator*>* v, Pvector &playerPos, int w, int h)
{
	timeSinceLastUpdate = m_clock.getElapsedTime();
	float time = timeSinceLastUpdate.asSeconds();

	Vector2f pPos(playerPos.x, playerPos.y);

	if (Distance(pPos) < w / 18.f && Distance(pPos) > 200) //seek and fire
	{
		setFire(true);
		stop = false;
		flock(v);
		applyForce(flockSeek(playerPos));
	}
	else if (Distance(pPos) < 200){ // just fire
		setFire(true);
		m_velocity = Pvector((Face(pPos)).x, (Face(pPos)).y);
		float a = angle(m_velocity);
		m_sprite.setRotation(a);
		stop = true;
	}
	else //just seek
	{
		stop = false;
		flock(v);
		applyForce(flockSeek(playerPos));
		setFire(false);
	}

	if (Fire())
	{
		if (time > 3.0f)
		{
			createMissile(w, h);
		}
	}
	if (!stop)
	{
		//To make the slow down not as abrupt
		m_acceleration.mulScalar(.4); // NI

		// Update velocity
		m_velocity.addVector(m_acceleration);

		// Limit speed
		m_velocity.limit(maxSpeed);// NI
		m_position.addVector(m_velocity);// // NI Updates position

		// Reset accelertion to 0 each cycle
		m_acceleration.mulScalar(0);// NI
		float a = angle(m_velocity);
		m_sprite.setRotation(a);
	}
	m_position = Pvector(mod(m_position.x, w), mod(m_position.y, h));

	m_sprite.setPosition(Vector2f(m_position.x, m_position.y));

	UpdateBullets();


}
void Predator::UpdateBullets()
{
	if (m_bullets.size() > 0)
	{
		// Iterate through list of bullets
		for (m_bulletIterator = m_bullets.begin(); m_bulletIterator != m_bullets.end(); ++m_bulletIterator)
		{
			(*m_bulletIterator)->Update();

			// Remove bullet if out of bounds
			if ((*m_bulletIterator)->OutOfBounds(GetPosition()))
			{
				m_bullets.erase(m_bulletIterator);
				break;
			}
		}
	}
}
// Applies all three laws for the flock of boids and modifies to keep them from
// breaking the laws.
void Predator::flock(vector<Predator*>* v)
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

float Predator::Distance(Vector2f &p){
	float distance;

	distance = sqrt(((m_position.x - p.x)*(m_position.x - p.x)) + ((m_position.y - p.y)*(m_position.y - p.y)));

	return distance;
}
Vector2f Predator::Face(Vector2f &v){
	
	Pvector desired_velocity;
	Pvector steering;
	Pvector normVel(v.x - m_position.x, v.y - m_position.y);
	normVel.normalize();

	desired_velocity.x = normVel.x * maxSpeed;
	desired_velocity.y = normVel.y * maxSpeed;

	Vector2f vel(desired_velocity.x, desired_velocity.y);
	return vel;

}

void Predator::createMissile(int w,int h)
{
	Vector2f bulletPos;
	bulletPos.x = m_position.x;
	bulletPos.y = m_position.y;

	Vector2f vel(m_velocity.x, m_velocity.y);
	Bullet* bullet = new Bullet(bulletPos, vel, w/9, h/9, w, h,true);

	m_bullets.push_back(bullet);
	m_clock.restart();
}



// Calculates the angle for the velocity of a boid which allows the visual 
// image to rotate in the direction that it is going in.
float Predator::angle(Pvector v)
{
	// From the definition of the dot product
	float angle = (float)(atan2(v.x, -v.y) * 180 / PI);
	return angle;
}

bool Predator::Fire()
{
	return m_fire;
}

void Predator::setFire(bool myFire)
{
	m_fire = myFire;
}


bool Predator::GetAlive()
{
	return m_alive;
}

void Predator::SetAlive(bool myAlive)
{
	m_alive = myAlive;
}

Sprite Predator::GetSprite()
{
	return m_sprite;
}