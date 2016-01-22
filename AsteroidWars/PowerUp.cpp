#include "stdafx.h"
#include "PowerUp.h"

/*constructor for the power up, set the position and loads the assets*/
PowerUp::PowerUp(int w, int h, int t) :type(t)
{
	m_Position.x = rand() % w + 1;;
	m_Position.y = rand() % h + 1;;
	Full_HEIGHT = h;
	Full_WIDTH = w;
	LoadAssets();
}
/*deconstructor for the power up*/
PowerUp::~PowerUp()
{
}
/*Load all assets for the power up class when power ups are created*/
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
	//m_PowerUPSprite.setOrigin(m_PowerUPTexture.getSize().x / 2.f, m_PowerUPTexture.getSize().y / 2.f);

	radarTexture.loadFromFile("Pics/Yellow.png");
	radarSprite = sf::Sprite(radarTexture);
	radarSprite.setScale(4.0f, 4.0f);

}

/*get the type of power up*/
int PowerUp::GetType(){
	return type;
}
/*draw the power up on screen*/
void PowerUp::Draw(RenderWindow &win){
	win.draw(m_PowerUPSprite);
}
/*Draw the power up on the radar*/
void PowerUp::DrawRadar(RenderWindow &win)
{
	radarSprite.setPosition(m_PowerUPSprite.getPosition());
	win.draw(radarSprite);
}
