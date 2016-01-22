#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "Bullet.h"
#include <list>
#include "SwarmEnemy.h"
#include "Obstacle.h"
#include "Factories.h"
#include "PowerUp.h"

//!  Player class 
/*!
This class defines methods that draw and update the player and draw/update player bullets
*/
class Player
{
public:
	/*!
	Player constructor that takes the width and height of the window and full map as parameters.
	*/
	Player(int windowWidth, int windowHeight, int fullWidth, int fullHeight);

	/*!
	void Update method that updates player sprite and bullet sprites in bullet list
	*/
	void Update();

	/*!
	void Draw method that draws the player sprite and bullet sprites in bullet list
	*/
	void Draw(RenderWindow &window);

	/*!
	void DrawOnRadar method that draws the player on the radar
	*/
	void DrawOnRadar(RenderWindow &window);

	/*!
	void DrawHealthBar method that draws the health bar showing the player's health
	*/
	void DrawHealthBar(RenderWindow &window);

	/*!
	void Shoot method that creates a bullet when called
	*/
	void Shoot();

	Vector2f GetCentre();/*!< Vector2f GetCentre method that returns the centre of the player sprite */
	Vector2f GetPosition();/*!< Vector2f GetPosition method that returns the player position */
	Vector2f GetVelocity();/*!< Vector2f GetVelocity method that returns the player velocity */
	float mod(float a, float b);/*!< float mod method that create a wrap-around world */

	bool CheckBulletSwarmCollision(SwarmEnemy *swarmEnemy);/*!< bool CheckBulletSwarmCollision method that returns true if a player bullet collides with a swarm boid */
	bool CheckSwarmCollision(SwarmEnemy *swarmEnemy);/*!< bool CheckSwarmCollision method that returns true if player collides with a swarm boid */
	bool CheckPlayerFactoryCollision(Sprite &factory);/*!< bool CheckPlayerFactoryCollision method that returns true if player collides with a factory */
	bool CheckPlayerPredatorCollision(Sprite &predator);/*!< bool CheckPlayerPredatorCollision method that returns true if player collides with a predator */
	bool CheckObstacleCollision(Obstacle *obstacle);/*!< bool CheckObstacleCollision method that returns true if player collides with an obstacle */
	bool CheckBulletObstacleCollision(Obstacle *obstacle);/*!< bool CheckBulletObstacleCollision method that returns true if a player bullet collides with an obstacle */
	bool CheckBulletFactoryCollision(Factories *factory);/*!< bool CheckBulletFactoryCollision method that returns true if a player bullet collides with a factory */
	bool CheckBulletPredatorCollision(Sprite &predator);/*!< bool CheckBulletPredatorCollision method that returns true if a player bullet collides with a predator */
	bool CheckPowerUpCollision(PowerUp *powerUp);/*!< bool CheckPowerUpCollision method that returns true if a player bullet collides with a powerup */

	int GetHealth();/*!< int GetHealth method that returns the player's health */
	void SetHealth(int myHealth);/*!< void SetHealth method that sets the player's health */
	Sprite currentHealth;/*!< Sprite currentHealth method that returns the currentHealth sprite */
	Sprite fullHealth;/*!< Sprite fullHealth method that returns the fullHealth sprite */
	Texture cHealth;/*!< Texture cHealth method that returns the cHealth texture */
	Texture fHealth;/*!< Texture fHealth method that returns the fHealth texture */

	Sprite GetSprite();/*!< Sprite GetSprite method that returns the player sprite */
private:
	Vector2f m_position;/*!< Vector2f variable m_position */
	Vector2f m_velocity;/*!< Vector2f variable m_velocity */
	Vector2f m_direction;/*!< Vector2f variable m_direction */
	float m_rotation;/*!< float variable m_rotation */

	Texture m_playerTexture;/*!< sf::Texture variable m_playerTexture. */
	Sprite m_playerSprite;/*!< sf::Sprite variable m_playerSprite. */

	Texture m_squareTexture;/*!< sf::Texture variable m_squareTexture. */
	Sprite m_squareSprite;/*!< sf::Sprite variable m_squareSprite. */
	float maxSpeed;/*!< float variable maxSpeed */
	float m_speed;/*!< float variable m_speed */
	float m_rotationSpeed;/*!< float variable m_rotationSpeed */
	Clock m_clock;/*!< sf::Clock variable m_clock */
	Time timeSinceLastUpdate;/*!< sf::Time variable timeSinceLastUpdate */

	Clock m_Rclock;/*!< sf::Clock variable m_Rclock */
	Time timeSinceLastUpdate1;/*!< sf::Time variable timeSinceLastUpdate1 */

	list<Bullet*> m_bullets;/*!< list of type Bullet variable m_bullets */
	int m_shootTimer;/*!< int variable m_shootTimer */
	int m_shootTimerLimit;/*!< int variable m_shootTimerLimit */
	std::list<Bullet*>::iterator m_bulletIterator;/*!< list iterator of type Bullet variable m_bulletIterator */
	bool increaseSpeed;/*!< bool variable increaseSpeed */
	bool increaseROF;/*!< bool variable increaseROF */

	int m_windowWidth;/*!< int variable m_windowWidth */
	int m_windowHeight;/*!< int variable m_windowHeight */
	int m_fullWidth;/*!< int variable m_fullWidth */
	int m_fullHeight;/*!< int variable m_fullHeight */

	/*!
	void SpeedIncreaseTimer method that sets player speed back to normal speed once speed powerup timer runs out of time.
	*/
	void SpeedIncreaseTimer();

	/*!
	void ROFIncreaseTimer method that sets player rate of fire back to normal rate of fire once rate of fire powerup timer runs out of time.
	*/
	void ROFIncreaseTimer();

	int m_health;/*!< int variable m_health */
	bool m_alive;/*!< bool variable m_alive */
};

#endif