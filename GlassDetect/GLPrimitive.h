#pragma once

#include "OpenGL.h"
#include "GLPoint.h"

#include <vector>

using namespace std;

class CGLPrimitive
{
public:
	virtual ~CGLPrimitive();
	static CGLPrimitive* Create(int nType);
	void PushPoint(float x, float y, float z);
	CBox getBox();
	int getType() { return m_nType; }
	const float* getData() { UpdateData(); return m_data; }
	int getDataLength() { UpdateData(); return m_nlen; }
	void updateBox() { m_validBox = false; }
protected:
	CGLPrimitive(int nType);
	void UpdateData();
	vector<CGLPoint*> m_vecPoints;
	int m_nType;
	float* m_data;
	int m_nlen;
	CBox m_box;
	bool m_validBox;
};

