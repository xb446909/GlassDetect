#pragma once
#include "OpenGL.h"
#include "Box.h"

class CGLPoint
{
public:
	CGLPoint();
	CGLPoint(float x, float y, float z);
	CBox getBox();
	~CGLPoint();
	float x() { return m_point.x; }
	float y() { return m_point.y; }
	float z() { return m_point.z; }
private:
	vec3 m_point;
};

