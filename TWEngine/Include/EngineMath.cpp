#include "stdafx.h"
#include "EngineMath.h"

float RadianToDegree(float fRadian)
{
	return fRadian / PUN_PI * 180.f;
}

float DegreeToRadian(float fDegree)
{
	return fDegree / 180.f * PUN_PI;
}