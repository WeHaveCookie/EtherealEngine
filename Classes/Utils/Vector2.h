#pragma once

#include <SFML/System/Vector2.hpp>

class Vector2
{
public:
	float x, y;

	Vector2();
	Vector2(float _x, float _y);
	~Vector2();

	Vector2& operator+=(const Vector2 vec);
	Vector2 operator+(const Vector2 vec) const;
	Vector2& operator-=(const Vector2 vec);
	Vector2 operator-(const Vector2 vec) const;
	Vector2& operator*=(float value);
	Vector2 operator*(float value) const;
	Vector2& operator/=(float value);
	Vector2 operator/(float value) const;
	Vector2& operator%=(float theta);
	Vector2 operator%(float theta) const;
	bool operator==(const Vector2 vec);
	bool operator!=(const Vector2 vec);
	
	float dot(const Vector2 vec);
	Vector2 norm();
	Vector2& trunc(float max);
	float ang();
	float mag();
	void reassign(float _x, float _y);
	void reassign(Vector2 vec);
	sf::Vector2f sf() { return sf::Vector2f(x, y); }

	const std::string to_str();
};

static Vector2 RoundUp(Vector2 vec, float seuil)
{
	Vector2 res = vec;
	if (res.x < seuil && res.x > -seuil)
	{
		if (res.x >= 0.0f)
		{
			res.x = seuil;
		}
		else
		{
			res.x = -seuil;
		}
	}

	if (res.y < seuil && res.y > -seuil)
	{
		if (res.y >= 0.0f)
		{
			res.y = seuil;
		}
		else
		{
			res.y = -seuil;
		}
	}
	return res;
}

static Vector2 RoundDown(Vector2 vec, float seuil)
{
	Vector2 res = vec;
	if (res.x < seuil && res.x > -seuil)
	{
		res.x = 0.0f;
	}

	if (res.y < seuil && res.y > -seuil)
	{
		res.y = 0.0f;
	}
	return res;
}