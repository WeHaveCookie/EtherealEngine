#pragma once

Vector2 RoundUp(Vector2 vec, float seuil)
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

Vector2 RoundDown(Vector2 vec, float seuil)
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