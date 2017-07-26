#include "stdafx.h"
#include "GLPoints.h"


CGLPoints::CGLPoints()
	: CGLPrimitive(GL_POINTS)
{
}


CGLPoints::~CGLPoints()
{
}

void CGLPoints::Add(vec3 pt)
{
	CGLPoint* point = new CGLPoint(pt.x, pt.y, pt.z);
	m_vecPoints.push_back(point);
}

void CGLPoints::Add(float x, float y, float z)
{
	CGLPoint* point = new CGLPoint(x, y, z);
	m_vecPoints.push_back(point);
}
