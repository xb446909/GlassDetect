#include "stdafx.h"
#include "Box.h"


CBox::CBox()
{
	m_leftTop = vec3(0, 0, 0);
	m_rightBottom = vec3(0, 0, 0);
}


CBox::~CBox()
{
}

CBox::CBox(vec3 vecLT, vec3 vecRB)
{
	m_leftTop = vecLT;
	m_rightBottom = vecRB;
}

const CBox & CBox::operator=(const CBox & box)
{
	m_leftTop = box.m_leftTop;
	m_rightBottom = box.m_rightBottom;
	return *this;
}

const CBox & CBox::operator+(const CBox & box)
{
	float x = min(box.m_leftTop.x, m_leftTop.x);
	float y = min(box.m_leftTop.y, m_leftTop.y);
	float z = min(box.m_leftTop.z, m_leftTop.z);
	m_leftTop = vec3(x, y, z);

	x = max(box.m_rightBottom.x, m_rightBottom.x);
	y = max(box.m_rightBottom.y, m_rightBottom.y);
	z = max(box.m_rightBottom.z, m_rightBottom.z);
	m_rightBottom = vec3(x, y, z);
	return *this;
}

const CBox & CBox::operator+=(const CBox & box)
{
	float x = min(box.m_leftTop.x, m_leftTop.x);
	float y = min(box.m_leftTop.y, m_leftTop.y);
	float z = min(box.m_leftTop.z, m_leftTop.z);
	m_leftTop = vec3(x, y, z);

	x = max(box.m_rightBottom.x, m_rightBottom.x);
	y = max(box.m_rightBottom.y, m_rightBottom.y);
	z = max(box.m_rightBottom.z, m_rightBottom.z);
	m_rightBottom = vec3(x, y, z);
	return *this;
}

float CBox::getNorm() const
{
	return sqrt(dot(m_rightBottom - m_leftTop, m_rightBottom - m_leftTop));
}
