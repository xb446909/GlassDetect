#pragma once

#define	GLEW_STATIC		1

#include "../GL/include/glew.h"
#include <gl/GLU.h>
#include "../common/shader.hpp"
#include "../common/texture.hpp"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

using namespace glm;

class CBox
{
public:
	CBox();
	~CBox();
	CBox(vec3 vecLT, vec3 vecRB);
	const CBox& operator=(const CBox& box);
	const CBox& operator+(const CBox &box);
	const CBox& operator+=(const CBox &box);
	vec3 getMinPoint() { return m_leftTop; }
	vec3 getMaxPoint() { return m_rightBottom; }
	vec3 getCenter() { return (m_leftTop + m_rightBottom) / 2.0f; }
	float getNorm();

private:
	vec3 m_leftTop;
	vec3 m_rightBottom;
};

