#include "stdafx.h"
#include "Scene.h"

Scene::Scene(int windowWidth, int windowHeight, int fullWidth, int fullHeight)
{
	// Load main menu background image
	m_backgroundTexture.loadFromFile("Pics/space.jpg");
	m_backgroundSprite = sf::Sprite(m_backgroundTexture);

	// Scale image to window
	m_backgroundSprite.setScale(fullWidth / m_backgroundSprite.getLocalBounds().width,
		fullHeight / m_backgroundSprite.getLocalBounds().height);
}

void Scene::Update()
{

}

void Scene::Draw(RenderWindow &window)
{
	window.draw(m_backgroundSprite);
}