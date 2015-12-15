#ifndef SCENE_H
#define SCENE_H

#include "stdafx.h"

class Scene
{
public:
	Scene(int windowWidth, int windowHeight);

	void Update();
	void Draw(RenderWindow &window);
private:
	Texture m_backgroundTexture;
	Sprite m_backgroundSprite;
};

#endif