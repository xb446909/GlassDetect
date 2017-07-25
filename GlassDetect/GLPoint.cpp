#include "stdafx.h"
#include "GLPoint.h"


CGLPoint::CGLPoint()
{
}

CGLPoint::CGLPoint(float x, float y, float z)
	: m_point(vec3(x, y, z))
{
}

CBox CGLPoint::getBox()
{
	return CBox(m_point, m_point);
}


CGLPoint::~CGLPoint()
{
}
