#ifndef SCENE_H
#define SCENE_H

#include "stdafx.h"

class Scene
{
public:
	Scene(int windowWidth, int windowHeight, int fullWidth, int fullHeight);

	void Update();
	void Draw(RenderWindow &window);
	void DrawOnRadar(RenderWindow &window);
private:
	Texture m_backgroundTexture;
	Sprite m_backgroundSprite;

	Texture m_backgroundTextureRadar;
	Sprite m_backgroundSpriteRadar;
};

#endif