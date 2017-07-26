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
	CBox getBox();
	int getType() { return m_nType; }
	const float* getData() { UpdateData(); return m_data; }
	int getDataLength() { UpdateData(); return m_nlen; }
	void updateBox() { m_validBox = false; m_validData = false; }
protected:
	CGLPrimitive(int nType);
	void UpdateData();
	vector<CGLPoint*> m_vecPoints;
	int m_nType;
	float* m_data;
	int m_nlen;
	CBox m_box;
	bool m_validBox;
	bool m_validData;
};

