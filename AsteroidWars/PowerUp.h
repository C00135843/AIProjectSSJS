#pragma once

enum e_type
{
	HEALTH = 1,
	ROF = 2,
	SPEED = 3
};
class PowerUp
{
public:
	PowerUp(int w, int h,int t);
	~PowerUp();
	void LoadAssets();
	void Draw(RenderWindow &win);
	void DrawRadar(RenderWindow &win);
	void resetPos(){
		m_Position.x = rand() % Full_WIDTH + 1;
		m_Position.y = rand() % Full_HEIGHT + 1;
	}
	Sprite GetSprite(){ return m_PowerUPSprite; }
	int GetType();
private:
	Vector2f m_Position;
	int type;
	int Full_WIDTH;
	int Full_HEIGHT;
	sf::Texture m_PowerUPTexture;
	sf::Sprite m_PowerUPSprite;
	sf::Texture radarTexture;
	sf::Sprite radarSprite;
	bool alive;
	


};

