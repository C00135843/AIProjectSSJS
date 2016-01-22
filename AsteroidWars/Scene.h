#ifndef SCENE_H
#define SCENE_H

#include "stdafx.h"

//!  Scene class 
/*!
This class defines methods that create the scene.
*/
class Scene
{
public:
	/*!
	Scene constructor that takes the width and height of the window and full map as parameters.
	*/
	Scene(int windowWidth, int windowHeight, int fullWidth, int fullHeight);

	/*! 
	void Draw method that draws the scene background.
	*/
	void Draw(RenderWindow &window);

	/*!
	void DrawOnRadar method that draws the scene background for the radar.
	*/
	void DrawOnRadar(RenderWindow &window);

	/*!
	void DrawRadarOutline method that draws the scene background for the radar.
	*/
	void DrawRadarOutline(RenderWindow &window);
private:
	Texture m_backgroundTexture;/*!< sf::Texture variable m_backgroundTexture. */
	Sprite m_backgroundSprite;/*!< sf::Sprite variable m_backgroundSprite. */

	Texture m_backgroundTextureRadar;/*!< sf::Texture variable m_backgroundTextureRadar. */
	Sprite m_backgroundSpriteRadar;/*!< sf::Sprite variable m_backgroundSpriteRadar. */

	Texture m_backgroundTextureRadarOutline;/*!< sf::Texture variable m_backgroundTextureRadarOutline. */
	Sprite m_backgroundSpriteRadarOutline;/*!< sf::Sprite variable m_backgroundSpriteRadarOutline. */
};

#endif