#pragma once
#include "GLPrimitive.h"
class CGLPoints :
	public CGLPrimitive
{
public:
	CGLPoints();
	~CGLPoints();
	void Add(vec3 pt);
	void Add(float x, float y, float z);
};

