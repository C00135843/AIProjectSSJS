#include "stdafx.h"
#include "VectorM.h"

bool VectorM::instanceFlag = false;
VectorM* VectorM::instance = NULL;

VectorM* VectorM::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new VectorM;
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}
sf::Vector2f VectorM::Normalise(sf::Vector2f v)
{
	float length = sqrt((v.x * v.x) + (v.y * v.y));
	return sf::Vector2f(v.x / length, v.y / length);
}
float VectorM::GetAngleBetween(sf::Vector2f v1, sf::Vector2f v2)
{
	float dot = GetDotProduct(v1, v2);
	float det = v1.x * v2.y - v1.y*v2.x;
	return ToRad(atan2(det, dot));
}
float VectorM::GetDistBetween(sf::Vector2f v1, sf::Vector2f v2)
{
	sf::Vector2f temp = v2 - v1;
	return GetMag(temp);
}

