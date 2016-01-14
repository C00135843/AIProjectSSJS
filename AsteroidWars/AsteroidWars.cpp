// AsteroidWars.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Player.h"
#include "Scene.h"
#include "Factories.h"


int main()
{
	// Render window width and height
	int windowWidth = 800;
	int windowHeight = 600;

	// Full screen width and height
	int fullWidth = windowWidth * 9;
	int fullHeight = windowHeight * 9;

	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight, 32), "SFML First Program");

	// Create a view
	View view;
	view.reset(FloatRect(0, 0, windowWidth, windowHeight));
	view.setViewport(FloatRect(0, 0, 1.0f, 1.0f));
	Vector2f camPosition(windowWidth / 2, windowHeight / 2);

	// Window frame rate
	window.setFramerateLimit(60);
	srand(time(NULL));
	// Class instances
	Player player(windowWidth, windowHeight, fullWidth, fullHeight);
	Factories factory(200, 200);//for test purposes
	Scene scene(windowWidth, windowHeight, fullWidth, fullHeight);


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

		view.reset(FloatRect(camPosition.x, camPosition.y, windowWidth, windowHeight));

		//view.setCenter(camPosition);

		window.setView(view);

		// Update
		player.Update();
		factory.Update();
		
		// Draw
		scene.Draw(window);
		player.Draw(window);
		factory.Draw(window);

		window.setView(window.getDefaultView());

		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}

