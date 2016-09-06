#pragma once

int randIntBorned(int a, int b)
{
	if (a == b)
	{
		return a;
	}
	return rand() % (b - a) + a;
}

float randFloatBorned(float a, float b)
{
	if (a == b)
	{
		return a;
	}
	return (rand() / (double)RAND_MAX) * (b - a) + a;
}