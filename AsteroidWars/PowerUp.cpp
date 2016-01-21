#include "stdafx.h"
#include "PowerUp.h"


PowerUp::PowerUp(int w, int h, int t) :type(t)
{
	m_Position.x = rand() % w + 1;;
	m_Position.y = rand() % h + 1;;
	Full_HEIGHT = h;
	Full_WIDTH = w;
	LoadAssets();
}

PowerUp::~PowerUp()
{
}

void PowerUp::LoadAssets()
{
	if (type == HEALTH)
	{
		m_PowerUPTexture.loadFromFile("Pics/health.png");

	}
	else if (type == ROF)
	{
		m_PowerUPTexture.loadFromFile("Pics/rof.png");

	}
	else if (type == SPEED)
	{
		m_PowerUPTexture.loadFromFile("Pics/speed.png");

	}
	m_PowerUPSprite.setTexture(m_PowerUPTexture);
	m_PowerUPSprite.setPosition(m_Position);
	//m_PowerUPSprite.setOrigin(m_PowerUPTexture.getSize().x / 2.f, m_PowerUPTexture.getSize().y / 2.f);

	radarTexture.loadFromFile("Pics/FactoryRadar.png");
	radarSprite = sf::Sprite(radarTexture);
	radarSprite.setScale(4.0f, 4.0f);



}
void PowerUp::Draw(RenderWindow &win){
	win.draw(m_PowerUPSprite);
}
void PowerUp::DrawRadar(RenderWindow &win)
{
	radarSprite.setPosition(m_PowerUPSprite.getPosition());
	win.draw(radarSprite);
}
