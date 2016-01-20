#ifndef SWARMENEMY_H
#define SWARMENEMY_H

class SwarmEnemy
{
private:
	Vector2f m_position;
	float m_rotation;

	Texture m_texture;
	Sprite m_sprite;

	Texture m_radarTexture;
	Sprite m_radarSprite;

	int m_id;
	void SetID(int id);
public:
	SwarmEnemy(int id);

	void Draw(sf::RenderWindow &window);
	void DrawOnRadar(sf::RenderWindow &window);

	void SetPosition(int x, int y);
	void SetRotation(float theta);

	Vector2f GetPosition();
	Sprite GetSprite();

	int GetID();
};

#endif