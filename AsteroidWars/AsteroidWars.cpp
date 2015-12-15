// AsteroidWars.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Player.h"
#include "Scene.h"


int main()
{
	// Render window width and height
	int windowWidth = 800;
	int windowHeight = 600;

	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight, 32), "SFML First Program");

	// Window frame rate
	window.setFramerateLimit(60);

	// Class instances
	Player player(windowWidth, windowHeight);
	Scene scene(windowWidth, windowHeight);

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

		// Update
		player.Update();

		// Draw
		scene.Draw(window);
		player.Draw(window);

		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}

