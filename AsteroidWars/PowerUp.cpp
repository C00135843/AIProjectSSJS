#include "stdafx.h"
#include "PowerUp.h"
//! PowerUp cpp
/*!
Constructor for the power up, set the position and loads the assets
*/
PowerUp::PowerUp(int w, int h, int t) :type(t)
{
	m_Position.x = rand() % w + 1;;
	m_Position.y = rand() % h + 1;;
	Full_HEIGHT = h;
	Full_WIDTH = w;
	LoadAssets();
}
/*!
Deconstructor for the power up
*/
PowerUp::~PowerUp()
{
}
/*!
Load all assets for the power up class when power ups are created
*/
void PowerUp::LoadAssets()
{
	if (type == HEALTH)
	{
		m_PowerUPTexture.loadFromFile("Pics/health.png");

	}
	else if (type == ROF)
	{
		m_PowerUPTexture.loadFromFile("Pics/speed.png");

	}
	else if (type == SPEED)
	{
		m_PowerUPTexture.loadFromFile("Pics/rof.png");

	}
	m_PowerUPSprite.setTexture(m_PowerUPTexture);
	m_PowerUPSprite.setPosition(m_Position);

	radarTexture.loadFromFile("Pics/Yellow.png");
	radarSprite = sf::Sprite(radarTexture);
	radarSprite.setScale(4.0f, 4.0f);

}

/*!
Get the type of power up
*/
int PowerUp::GetType(){
	return type;
}
/*!
Draw the power up on screen
*/
void PowerUp::Draw(RenderWindow &win){
	win.draw(m_PowerUPSprite);
}
/*!
Draw the power up on the radar
*/
void PowerUp::DrawRadar(RenderWindow &win)
{
	radarSprite.setPosition(m_PowerUPSprite.getPosition());
	win.draw(radarSprite);
}
