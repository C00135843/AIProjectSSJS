#ifndef SWARMENEMY_H
#define SWARMENEMY_H

//! SwarmEnemy Class
/*!
This class draws the swarm enemies and updates their position.
Position of the swarm enemy is based on position of the corresponding ID of a boid from the Boid class
*/
class SwarmEnemy
{
private:
	Vector2f m_position;/*!< Vector2f variable m_position */
	float m_rotation;/*!< float variable m_rotation */

	Texture m_texture;/*!< sf::Texture variable m_texture. */
	Sprite m_sprite;/*!< sf::Sprite variable m_sprite. */

	Texture m_radarTexture;/*!< sf::Texture variable m_radarTexture. */
	Sprite m_radarSprite;/*!< sf::Sprite variable m_radarSprite. */

	int m_id;/*!< int variable m_id. */

	/*!
	void SetID method that sets the swarm enemy id.
	*/
	void SetID(int id);
public:
	/*!
	SwarmEnemy constructor that takes an id number which corresponds to a boid from the boid class
	*/
	SwarmEnemy(int id);

	/*!
	void Draw method that draws the swarm enemy sprite
	*/
	void Draw(sf::RenderWindow &window);

	/*!
	void DrawOnRadar method that draws the swarm enemy on the radar
	*/
	void DrawOnRadar(sf::RenderWindow &window);

	/*!
	void SetPosition method that sets the position of the swarm enemy
	*/
	void SetPosition(int x, int y);

	/*!
	void SetRotation method that sets the rotation angle of the swarm enemy sprite
	*/
	void SetRotation(float theta);

	Vector2f GetPosition();/*!< Vector2f GetPosition that returns the swarm enemy position */
	Sprite GetSprite();/*!< sf::Sprite method GetSprite that returns the swarm enemy sprite. */

	int GetID();/*!< int method GetID that returns the swarm enemy id */
};

#endif