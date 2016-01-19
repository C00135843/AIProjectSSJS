#include "stdafx.h"
#include "Player.h"
#include "Scene.h"
#include "Bullet.h"

#include "Boid.h"
#include "Flock.h"
#include "SwarmEnemy.h"
#include "Obstacle.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// For Boids
string action = "swarm";
Flock flock;
vector<SwarmEnemy*> swarmEnemies;

void CreateBoids(int, int);
void UpdateBoids(sf::RenderWindow &window, int, int, Vector2f &playerPos, Vector2f &playerVel, vector<Obstacle*> obstacles);

int main()
{
	// initialize random seed: 
	srand(time(NULL));

	// Render window width and height
	VideoMode desktop = VideoMode::getDesktopMode();
	int windowWidth = desktop.width;
	int windowHeight = desktop.height;

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

	// Create boids
	CreateBoids(windowWidth, windowHeight);

	// Create obstalces
	vector<Obstacle*> obstacles;
	Obstacle obstacleInstance;
	vector<Obstacle*>::iterator m_obstacleIterator;
	int noOfObstacles = 100;

	for (int i = 0; i < noOfObstacles; i++)
	{
		//obstacles = obstacleInstance.CreateObstacle(windowWidth, windowHeight, player.GetPosition(), obstacles);
		obstacles = obstacleInstance.CreateObstacle(fullWidth, fullHeight, player.GetPosition(), obstacles);
	}

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
		}

		#pragma endregion

		scene.Draw(window);
		player.Draw(window);

		UpdateBoids(window, fullWidth, fullHeight, player.GetPosition(), player.GetVelocity(), obstacles);

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


