#ifndef BULLET_H
#define BULLET_H

class Bullet
{
public:
	Bullet(Vector2f pos, Vector2f vel, int windowWidth, int windowHeight, int fullWidth, int fullHeight);

	void Update();
	void Draw(RenderWindow &window);

	bool OutOfBounds(Vector2f);
	float DistanceFromPlayer(Vector2f);
private:
	Vector2f m_position;
	Vector2f m_velocity;

	Texture m_texture;
	Sprite m_sprite;

	float m_speed;

	int m_windowWidth;
	int m_windowHeight;
	int m_fullWidth;
	int m_fullHeight;
};

#endif