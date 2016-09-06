#pragma once

sf::Vector2f RoundUp(sf::Vector2f vec, float seuil)
{
	sf::Vector2f res = vec;
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

sf::Vector2f RoundDown(sf::Vector2f vec, float seuil)
{
	sf::Vector2f res = vec;
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