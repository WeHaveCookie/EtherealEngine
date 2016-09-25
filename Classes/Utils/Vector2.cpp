#include "stdafx.h"
#include "vector2.h"

#include <cstdio>
#include <cmath>
#include <string>

Vector2::Vector2() : 
	x(0), 
	y(0)
{
}

Vector2::Vector2(float _x, float _y) 
	: x(_x), 
	y(_y)
{
}

Vector2::~Vector2()
{
}

void Vector2::reassign(float _x, float _y)
{
	x = _x;
	y = _y;
}

void Vector2::reassign(Vector2 vec)
{
	reassign(vec.x, vec.y);
}

Vector2& Vector2::operator+=(const Vector2 vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2 vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

Vector2& Vector2::operator*=(float value)
{
	x *= value;
	y *= value;
	return *this;
}

Vector2& Vector2::operator/=(float value)
{
	x /= value;
	y /= value;
	return *this;
}

Vector2& Vector2::operator%=(float theta)
{
	float cs = std::cos(theta);
	float sn = std::sin(theta);

	float px = x * cs - y * sn;
	float py = x * sn + y * cs;

	x = px;
	y = py;
	return *this;
}

Vector2 Vector2::operator+(const Vector2 vec) const
{
	return Vector2(x + vec.x, y + vec.y);
}

Vector2 Vector2::operator-(const Vector2 vec) const
{
	return Vector2(x - vec.x, y - vec.y);
}

Vector2 Vector2::operator*(float value) const
{
	return Vector2(x * value, y * value);
}

Vector2 Vector2::operator/(float value) const
{
	return Vector2(x / value, y / value);
}

Vector2 Vector2::operator%(float theta) const
{
	float cs = std::cos(theta);
	float sn = std::sin(theta);

	float px = x * cs - y * sn;
	float py = x * sn + y * cs;

	return Vector2(px, py);
}

bool Vector2::operator==(const Vector2 vec)
{
	return x == vec.x && y == vec.y;
}

bool Vector2::operator!=(const Vector2 vec)
{
	return !(*this == vec);
}

float Vector2::dot(const Vector2 vec)
{
	return x * vec.x + y * vec.y;
}

Vector2 Vector2::norm()
{
	return *this / mag();
}

Vector2& Vector2::trunc(float max)
{
	if (mag() > max)
		reassign(norm() * max);
	return *this;
}

float Vector2::ang()
{
	return std::atan2(x, y);
}

float Vector2::mag()
{
	return std::sqrt(x*x + y*y);
}

const std::string Vector2::to_str()
{
	char tmpbuf[256];
	sprintf(tmpbuf, "[%f, %f] \n", x, y);
	return tmpbuf;
}