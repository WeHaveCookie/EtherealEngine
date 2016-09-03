#pragma once

int randIntBorned(int a, int b)
{
	return rand() % (b - a) + a;
}

float randFloatBorned(float a, float b)
{
	return (rand() / (double)RAND_MAX) * (b - a) + a;
}