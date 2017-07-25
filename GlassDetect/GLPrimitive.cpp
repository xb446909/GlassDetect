#include "stdafx.h"
#include "GLPrimitive.h"

#include "GLPoints.h"


CGLPrimitive * CGLPrimitive::Create(int nType)
{
	CGLPrimitive* pObj = nullptr;
	switch (nType)
	{
	case GL_POINTS:
		pObj = new CGLPoints();
		break;
	default:
		break;
	}
	return pObj;
}

void CGLPrimitive::PushPoint(float x, float y, float z)
{
	CGLPoint* point = new CGLPoint(x, y, z);
	m_vecPoints.push_back(point);
}

CBox CGLPrimitive::getBox()
{
	if (m_validBox)
		return m_box;

	CBox box;
	for (size_t i = 0; i < m_vecPoints.size(); i++)
	{
		box += m_vecPoints[i]->getBox();
	}
	m_box = box;
	m_validBox = true;
	return m_box;
}

CGLPrimitive::CGLPrimitive(int nType)
	: m_nType(nType)
	, m_data(nullptr)
	, m_nlen(0)
{
}

void CGLPrimitive::UpdateData()
{
	if (m_data != nullptr)
	{
		delete[] m_data;
		m_data = nullptr;
	}
	m_nlen = m_vecPoints.size();
	if (m_nlen > 0)
	{
		m_data = new float[m_nlen * 3];
		for (int i = 0; i < m_nlen; i++)
		{
			m_data[3 * i] = m_vecPoints[i]->x();
			m_data[3 * i + 1] = m_vecPoints[i]->y();
			m_data[3 * i + 2] = m_vecPoints[i]->z();
		}
	}
}


CGLPrimitive::~CGLPrimitive()
{
	for (size_t i = 0; i < m_vecPoints.size(); i++)
	{
		delete m_vecPoints[i];
	}
	m_vecPoints.clear();

	if (m_data != nullptr)
	{
		delete[] m_data;
		m_data = nullptr;
	}
	m_nlen = 0;
}
