#ifndef BULLET_H
#define BULLET_H

//! Bullet Class
/*!
This class is used by the player and predators to create bullets.
*/
class Bullet
{
public:
	/*!
	Bullet constructor that takes position and velocity of object that created it.
	Takes window with and height and map width and height.
	Takes a boolean to see whether it was created by a player or a predator
	*/
	Bullet(Vector2f pos, Vector2f vel, int windowWidth, int windowHeight, int fullWidth, int fullHeight,bool predator);

	/*!
	void Update method that updates bullet sprite position
	*/
	void Update();

	/*!
	void Draw method that draws the bullet sprite
	*/
	void Draw(RenderWindow &window);

	/*!
	bool OutOfBounds method that returns a boolean which tells whether the bullets is out of bounds so that it can be deleted.
	*/
	bool OutOfBounds(Vector2f);

	/*!
	float DistanceFromPlayer method that returns a float which tells us the distance of the bullet from the player.
	*/
	float DistanceFromPlayer(Vector2f);

	/*!
	Sprite GetSprite method that returns the player sprite.
	*/
	Sprite GetSprite();
private:
	Vector2f m_position;/*!< Vector2f variable m_position */
	Vector2f m_velocity;/*!< Vector2f variable m_velocity */

	Texture m_texture;/*!< sf::Texture variable m_texture. */
	Sprite m_sprite;/*!< sf::Sprite variable m_sprite. */

	float m_speed;/*!< float variable m_speed */

	int m_windowWidth;/*!< int variable m_windowWidth */
	int m_windowHeight;/*!< int variable m_windowHeight */
	int m_fullWidth;/*!< int variable m_fullWidth */
	int m_fullHeight;/*!< int variable m_fullHeight */
};

#endif