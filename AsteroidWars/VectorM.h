#pragma once
class VectorM
{
public:
	VectorM();
	static VectorM* GetInstance();
	~VectorM(){
		instanceFlag = false;
	}

	float ToRad(float deg)
	{
		return (deg* M_PI) / 180;
	}
	float ToDeg(float rad){
		return (rad * 180) / M_PI;
	}
	float GetMag(Vector2f v)
	{
		return sqrt(v.x * v.x + v.y*v.y);
	}
	float GetAngleBetween(Vector2f v1, Vector2f v2);
	float GetDotProduct(Vector2f v1, Vector2f v2){
		return (v1.x*v2.x + v1.y *v2.y);
	}
	float GetDistBetween(Vector2f v1, Vector2f v2);
	Vector2f Normalise(Vector2f v);
private:
	static bool instanceFlag;
	static VectorM* instance;
};

