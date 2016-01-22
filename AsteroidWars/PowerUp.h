#pragma once

/*!
An Enum for the type of power available
*/
enum e_type
{
	HEALTH = 1, /*!< Enum value Health*/
	ROF = 2,/*!< Enum value Rate of Fire*/
	SPEED = 3/*!< Enum value max speed power up*/
};

//! PowerUp Class
/*!
\This class is used to create power ups. There are three types of power ups Health,
\Rate of fire and max speed increase. 
*/
class PowerUp
{
public:
	/*!
	\Constructor uses full width and height of the map to set position.
	*/
	PowerUp(int w, int h,int t);
	~PowerUp(); //!< Deconstructor
	/*!
	\Void LoadAssets method loads all of the textures and assigns them to the sprite.
	*/
	void LoadAssets();
	/*!
	\ Void Draw method receives a renderWindow parameter and Draws the sprite on the screen using the position created in the Constructor
	*/
	void Draw(RenderWindow &win);
	/*!
	\ void DrawRadar Method receives a renderWindow parameter Draws the radar sprite on the radar in the location related to the overall map
	*/
	void DrawRadar(RenderWindow &win);
	/*!
	\ 
	*/
	Sprite GetSprite(){ return m_PowerUPSprite; }
	int GetType();
private:
	Vector2f m_Position; /*!< */
	int type;
	int Full_WIDTH;
	int Full_HEIGHT;
	sf::Texture m_PowerUPTexture;
	sf::Sprite m_PowerUPSprite;
	sf::Texture radarTexture;
	sf::Sprite radarSprite;
	bool alive;
	


};

