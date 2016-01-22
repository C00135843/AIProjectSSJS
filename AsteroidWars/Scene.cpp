#include "stdafx.h"
#include "Scene.h"

Scene::Scene(int windowWidth, int windowHeight, int fullWidth, int fullHeight)
{
	m_backgroundTexture.loadFromFile("Pics/space2.jpg");
	m_backgroundSprite = sf::Sprite(m_backgroundTexture);

	m_backgroundTextureRadar.loadFromFile("Pics/Black.png");
	m_backgroundSpriteRadar = sf::Sprite(m_backgroundTextureRadar);

	m_backgroundTextureRadarOutline.loadFromFile("Pics/Radar.png");
	m_backgroundSpriteRadarOutline = sf::Sprite(m_backgroundTextureRadarOutline);

	/*!< Scale background sprite to the window */
	m_backgroundSprite.setScale(fullWidth / m_backgroundSprite.getLocalBounds().width,
		fullHeight / m_backgroundSprite.getLocalBounds().height);

	/*!< Scale background sprite for the radar to the window */
	m_backgroundSpriteRadar.setScale(fullWidth / m_backgroundSpriteRadar.getLocalBounds().width,
		fullHeight / m_backgroundSpriteRadar.getLocalBounds().height);

	/*!< Scale background sprite for the radar outline to the window */
	m_backgroundSpriteRadarOutline.setScale(fullWidth / m_backgroundSpriteRadarOutline.getLocalBounds().width,
		fullHeight / m_backgroundSpriteRadarOutline.getLocalBounds().height);
}

/*!
void Draw method that draws the scene background.
*/
void Scene::Draw(RenderWindow &window)
{
	window.draw(m_backgroundSprite);
}

/*!
void DrawOnRadar method that draws the scene background for the radar.
*/
void Scene::DrawOnRadar(RenderWindow &window)
{
	window.draw(m_backgroundSpriteRadar);
}

/*!
void DrawRadarOutline method that draws the scene background for the radar.
*/
void Scene::DrawRadarOutline(RenderWindow &window)
{
	window.draw(m_backgroundSpriteRadarOutline);
}