#include "stdafx.h"
#include "Player.h"
#include "Scene.h"
#include "Factories.h"
#include "Bullet.h"

#include "Boid.h"
#include "Flock.h"
#include "SwarmEnemy.h"
#include "Obstacle.h"
#include "Predator.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// For Boids
string action = "swarm";
Flock flock;
vector<SwarmEnemy*> swarmEnemies;
vector<Factories*> factEnemies;
vector<Predator*> predators;

void CreateBoids(int, int);
void CreateFact(int, int);
void CreatePredators(Vector2f position);
void UpdateFact(sf::RenderWindow &window, Vector2f p, int w, int h, Sprite&playerSprite);
void UpdateBoids(sf::RenderWindow &window, int, int, Vector2f playerPos, Vector2f &playerVel, vector<Obstacle*> obstacles);
void UpdatePredators(sf::RenderWindow &window, Vector2f &playerPos, int w, int h);

int flockcount = 0;
Pvector flockWanderPos;
vector<bool> IsInFlock;

int main()
{
	// initialize random seed: 
	srand(time(NULL));

	// Render window width and height
	VideoMode desktop = VideoMode::getDesktopMode();
	int windowWidth = desktop.width;
	int windowHeight = desktop.height;
	//int windowWidth = 800;
	//int windowHeight = 600;

	// Full screen width and height
	int fullWidth = windowWidth * 9;
	int fullHeight = windowHeight * 9;

	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight, 32), "Asteroid Wars!!!");

	// Create a view
	View view;
	view.reset(FloatRect(0, 0, windowWidth, windowHeight));
	view.setViewport(FloatRect(0, 0, 1.0f, 1.0f));
	Vector2f camPosition(windowWidth / 2, windowHeight / 2);

	// Create radar view
	View radarView;
	radarView.setViewport(sf::FloatRect(0.755f, 0, 0.245f, 0.245f));

	// Create radar outline view
	View radarOutlineView;
	radarOutlineView.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));

	// Window frame rate
	window.setFramerateLimit(60);

	// Class instances
	Player player(windowWidth, windowHeight, fullWidth, fullHeight);
	Scene scene(windowWidth, windowHeight, fullWidth, fullHeight);
	vector<SwarmEnemy*>::iterator m_swarmIterator;
	vector<Factories*>::iterator m_factIterator;
	vector<Predator*>::iterator m_predatorIterator;

	CreateFact(windowWidth, windowHeight);

	// Create boids
	CreateBoids(windowWidth, windowHeight);

	// Create obstalces
	vector<Obstacle*> obstacles;
	Obstacle obstacleInstance;
	vector<Obstacle*>::iterator m_obstacleIterator;
	int noOfObstacles = 50;

	/*for (int i = 0; i < noOfObstacles; i++)
	{
		obstacles = obstacleInstance.CreateObstacle(fullWidth, fullHeight, player.GetPosition(), obstacles);
	}*/

	// Start game loop 
	while (window.isOpen())
	{
		// Process events 
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit 
			if (Event.type == sf::Event::Closed)
				window.close();

			// Escape key : exit 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		//prepare frame
		window.clear();

		// Set view
		#pragma region Set Camera Position

		// Focus camera on player
		camPosition.x = player.GetPosition().x + 10 - (windowWidth / 2);
		camPosition.y = player.GetPosition().y + 10 - (windowHeight / 2);

		if (camPosition.x < 0)
		{
			camPosition.x = 0;
		}
		if (camPosition.y < 0)
		{
			camPosition.y = 0;
		}
		if (camPosition.x > fullWidth - windowWidth)
		{
			camPosition.x = fullWidth - windowWidth;
		}
		if (camPosition.y > fullHeight - windowHeight)
		{
			camPosition.y = fullHeight - windowHeight;

		}

		#pragma endregion

		radarView.reset(FloatRect(camPosition.x, camPosition.y, windowWidth, windowHeight));
		
		//window.setView(radarView);

		// Modify view
		view.reset(FloatRect(camPosition.x, camPosition.y, windowWidth, windowHeight));

		window.setView(view);

		// Update & Draw
		player.Update();
		flock.GetDisanceFromPlayer(player.GetPosition());

		#pragma region Check Collisions

		if (player.GetHealth() > 0)
		{
			// Check collision between bullet and swarm
			for (m_swarmIterator = swarmEnemies.begin(); m_swarmIterator != swarmEnemies.end(); ++m_swarmIterator)
			{
				// Bullet collision
				if (player.CheckBulletSwarmCollision((*m_swarmIterator)))
				{
					// Remove boid
					flock.removeBoid((*m_swarmIterator)->GetID());
					swarmEnemies.erase(m_swarmIterator);
					break;
				}

				// Player/Swarm collision
				if (player.CheckSwarmCollision(*m_swarmIterator))
				{
					// Remove boid
					flock.removeBoid((*m_swarmIterator)->GetID());
					swarmEnemies.erase(m_swarmIterator);
					player.SetHealth(player.GetHealth() - 20);
					break;
				}
			}// End collision checking  

			for (m_factIterator = factEnemies.begin(); m_factIterator != factEnemies.end(); ++m_factIterator)
			{
				// Bullet collision
				if (player.CheckBulletFactoryCollision((*m_factIterator)))
				{
					// Remove boid
					(*m_factIterator)->SetHealth((*m_factIterator)->GetHealth() - 25);
					if ((*m_factIterator)->GetHealth() <= 0)
					{
						factEnemies.erase(m_factIterator);
						break;
					}
				}
			}
		}

		#pragma endregion

		scene.Draw(window);
		player.Draw(window);

		//UpdateBoids(window, fullWidth, fullHeight, player.GetPosition(), player.GetVelocity(), obstacles);
		UpdateFact(window, player.GetPosition(), windowWidth, windowHeight, player.GetSprite());
		UpdatePredators(window, player.GetPosition(), windowWidth, windowHeight);

		#pragma region Obstacles

		// Update/Draw obstacles
		for (m_obstacleIterator = obstacles.begin(); m_obstacleIterator != obstacles.end(); ++m_obstacleIterator)
		{
			(*m_obstacleIterator)->Draw(window);
			(*m_obstacleIterator)->Update();

			// Bullet/Obstacle collision
			if (player.CheckBulletObstacleCollision((*m_obstacleIterator)))
			{
				// Remove boid
				obstacles.erase(m_obstacleIterator);
				break;
			}

			// Player/Obstacle collision
			if (player.CheckObstacleCollision((*m_obstacleIterator)))
			{
				obstacles.erase(m_obstacleIterator);

				// Collision with an obstacle results in death!
				//player.SetHealth(player.GetHealth() - 100);
				break;
			}

			// Get distance from a swarm boid to a obstacle *****************
			//flock.InRangeOfObstacle((*m_obstacleIterator)->GetPosition());

			if (flock.InRangeOfObstacle((*m_obstacleIterator)->GetPosition()))
			{
				(*m_obstacleIterator)->setInRangeOfBoid(true);
				//cout << "obstacle in range" << endl;
			}
			else
			{
				(*m_obstacleIterator)->setInRangeOfBoid(false);
			}

			// Collision between swarm boids and obstacles
			for (m_swarmIterator = swarmEnemies.begin(); m_swarmIterator != swarmEnemies.end(); ++m_swarmIterator)
			{
				// Collision
				if ((*m_obstacleIterator)->CollisionWithSwarm((*m_swarmIterator)))
				{
					flock.removeBoid((*m_swarmIterator)->GetID());
					swarmEnemies.erase(m_swarmIterator);
					(*m_obstacleIterator)->SetAlive(false);
					cout << swarmEnemies.size() << endl;
					break;
				}
			}

			// Erase obstacles(Will occur after swarm boid collides with obstacle)
			if ((*m_obstacleIterator)->GetAlive() == false)
			{
				obstacles.erase(m_obstacleIterator);
				break;
			}

		}// End iterator for obstacles 


#pragma endregion

		#pragma region RadarView

		window.setView(radarOutlineView);

		scene.DrawRadarOutline(window);

		window.setView(radarView);

		scene.DrawOnRadar(window);
		player.DrawOnRadar(window);

		// Update/Draw obstacles
		for (m_obstacleIterator = obstacles.begin(); m_obstacleIterator != obstacles.end(); ++m_obstacleIterator)
		{
			(*m_obstacleIterator)->DrawOnRadar(window);
		}

		for (int i = 0; i < swarmEnemies.size(); i++)
		{
			swarmEnemies[i]->DrawOnRadar(window);
		}

		for (int i = 0; i < factEnemies.size(); i++)
		{
			factEnemies[i]->DrawOnRadar(window);
		}

		for (int i = 0; i < predators.size(); i++)
		{
			predators[i]->DrawOnRadar(window);
		}

		#pragma endregion

		window.setView(window.getDefaultView());

		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}

// Create boids for swarm enemies
void CreateBoids(int window_width, int window_height)
{
	int noOfBoids = 15;

	for (int i = 0; i < noOfBoids; i++)
	{
		Boid b(window_width, window_height, i);// Create boid
		SwarmEnemy *swarmEnemy = new SwarmEnemy(i);

		// Adding the boid to the flock and adding the ships to the vector swarmEnemies
		flock.addBoid(b, i);
		swarmEnemies.push_back(swarmEnemy);
	}
}

void CreateFact(int window_width, int window_height)
{
	int noOffact = 1;

	for (int i = 0; i < noOffact; i++)
	{
		Factories* f = new Factories(window_width, window_height);// Create factory
		// Adding the boid to the flock and adding the ships to the vector swarmEnemies
		//flock.addBoid(b, i);
		factEnemies.push_back(f);
	}
}

void CreatePredators(Vector2f position)
{
	//cout << "CreatePredators" << endl;

	Predator* predator = new Predator(position);

	predators.push_back(predator);
}

void UpdatePredators(sf::RenderWindow &window, Vector2f &playerPos, int w, int h)
{
	if (predators.size() > 0)
	{
		for (int i = 0; i < predators.size(); i++)
		{
			predators[i]->Update(&predators, Pvector(playerPos.x, playerPos.y), w * 9, h * 9);
			predators[i]->Draw(window);
		}
	}
}

void UpdateFact(sf::RenderWindow &window, Vector2f p, int w, int h, Sprite&playerSprite)
{
	//check for flocking
	for (int i = 0; i < factEnemies.size(); i++)
	{
		Pvector fPos(factEnemies[i]->GetPosition().x, factEnemies[i]->GetPosition().y); // inner fact
		for (int j = 0; j < factEnemies.size(); j++)
		{
			if (j != i)
			{
				Pvector pPos(factEnemies[j]->GetPosition().x, factEnemies[j]->GetPosition().y); //outer fact

				if (fPos.distance(pPos) < 100)
				{
					factEnemies[i]->setWander(false);
					factEnemies[j]->setWander(false);
				}

			}
		}
		if (factEnemies[i]->getAlive() == true)
		{
			factEnemies[i]->Update(p, w * 9, h * 9, &factEnemies, Pvector(0, 0));
			factEnemies[i]->FactoryMissilePlayerCollision(playerSprite);
			factEnemies[i]->Draw(window);

			// Call create a predator if we can create a predator
			if (factEnemies[i]->GetCreatePredator())
			{
				CreatePredators(factEnemies[i]->GetPosition());
				factEnemies[i]->SetCreatePredator(false);
			}
		}
	}
}

// Update the boids of the swarm enemies
void UpdateBoids(sf::RenderWindow &window, int window_width, int window_height, Vector2f &playerPos, Vector2f &playerVel, vector<Obstacle*> obstacles)
{
	// Draw and Update boids
	for (int i = 0; i < swarmEnemies.size(); i++)
	{
		// Matches up the location of the swarm enemy to the boid
		swarmEnemies[i]->SetPosition(flock.getBoid(i).location.x, flock.getBoid(i).location.y);

		// Calculates the angle where the velocity is pointing so that the triangle turns towards it.
		float theta;
		theta = flock.getBoid(i).angle(flock.getBoid(i).velocity);
		swarmEnemies[i]->SetRotation(theta);

		// These if statements prevent boids from moving off the screen through wrapping

		// If boid exits right boundary
		if (swarmEnemies[i]->GetPosition().x > window_width)
			swarmEnemies[i]->SetPosition(swarmEnemies[i]->GetPosition().x - window_width, swarmEnemies[i]->GetPosition().y);

		// If boid exits bottom boundary
		if (swarmEnemies[i]->GetPosition().y > window_height)
			swarmEnemies[i]->SetPosition(swarmEnemies[i]->GetPosition().x, swarmEnemies[i]->GetPosition().y - window_height);

		// If boid exits left boundary
		if (swarmEnemies[i]->GetPosition().x < 0)
			swarmEnemies[i]->SetPosition(swarmEnemies[i]->GetPosition().x + window_width, swarmEnemies[i]->GetPosition().y);

		// If boid exits top boundary
		if (swarmEnemies[i]->GetPosition().y < 0)
			swarmEnemies[i]->SetPosition(swarmEnemies[i]->GetPosition().x, swarmEnemies[i]->GetPosition().y + window_height);

		// Draw boids
		swarmEnemies[i]->Draw(window);
	}// End for

	// Applies the three rules to each boid in the flock and changes them accordingly.
	if (action == "flock")
		flock.flocking();// Updates flock
	else
		flock.swarming(playerPos, playerVel, obstacles);// Updates flock
}


