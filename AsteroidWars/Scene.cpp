#include "stdafx.h"
#include "Scene.h"

Scene::Scene(int windowWidth, int windowHeight, int fullWidth, int fullHeight)
{
	// Load main menu background image
	m_backgroundTexture.loadFromFile("Pics/space2.jpg");
	m_backgroundSprite = sf::Sprite(m_backgroundTexture);

	// Load main menu background image
	m_backgroundTextureRadar.loadFromFile("Pics/Black.png");
	m_backgroundSpriteRadar = sf::Sprite(m_backgroundTextureRadar);

	// Load main menu background image
	m_backgroundTextureRadarOutline.loadFromFile("Pics/Radar.png");
	m_backgroundSpriteRadarOutline = sf::Sprite(m_backgroundTextureRadarOutline);

	// Scale image to window
	m_backgroundSprite.setScale(fullWidth / m_backgroundSprite.getLocalBounds().width,
		fullHeight / m_backgroundSprite.getLocalBounds().height);

	// Scale image to window
	m_backgroundSpriteRadar.setScale(fullWidth / m_backgroundSpriteRadar.getLocalBounds().width,
		fullHeight / m_backgroundSpriteRadar.getLocalBounds().height);

	// Scale image to window
	m_backgroundSpriteRadarOutline.setScale(fullWidth / m_backgroundSpriteRadarOutline.getLocalBounds().width,
		fullHeight / m_backgroundSpriteRadarOutline.getLocalBounds().height);
}

void Scene::Update()
{

}

void Scene::Draw(RenderWindow &window)
{
	window.draw(m_backgroundSprite);
}

void Scene::DrawOnRadar(RenderWindow &window)
{
	window.draw(m_backgroundSpriteRadar);
}

void Scene::DrawRadarOutline(RenderWindow &window)
{
	window.draw(m_backgroundSpriteRadarOutline);
}