#include "stdafx.h"
#include "OpenGL.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "../GL/lib/glew32s.lib")

COpenGL::COpenGL()
	: m_validModelviewMatrix(false)
	, m_validProjectionMatrix(false)
	, m_verticalRotationLocked(false)
	, m_bubbleViewModeEnabled(false)
{
}


COpenGL::~COpenGL()
{
	for (size_t i = 0; i < m_vecBuffer.size(); i++)
	{
		glDeleteBuffers(1, &m_vecBuffer[i].buffer);
	}

	m_vecBuffer.clear();

	glDeleteProgram(m_programID);

	HGLRC hrc = wglGetCurrentContext();
	if (wglMakeCurrent(0, 0) == FALSE)
	{
		OutputDebugString(_T("Could not make RC non-current\r\n"));
	}

	if (hrc)
	{
		if (::wglDeleteContext(hrc) == FALSE)
		{
			OutputDebugString(_T("Could not delete RC\r\n"));
		}
	}
}


void COpenGL::OutputString(char * fmt, ...)
{
	char szOutput[2048] = { 0 };
	va_list args;
	va_start(args, fmt);
	vsprintf_s(szOutput, 2048, fmt, args);
	va_end(args);
	OutputDebugStringA(szOutput);
}

BEGIN_MESSAGE_MAP(COpenGL, CView)
ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_SIZE()
ON_WM_ERASEBKGND()
ON_WM_MOUSEMOVE()
ON_WM_MOUSEWHEEL()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


BOOL COpenGL::init()
{
	ASSERT(m_hDC != NULL);

	if (!SetupPixelFormat(m_hDC)) return FALSE;

	PIXELFORMATDESCRIPTOR pfd;

	int iPixelFormat = GetPixelFormat(m_hDC);

	DescribePixelFormat(m_hDC, iPixelFormat,
		sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	HGLRC  hglrc = wglCreateContext(m_hDC);
	if (!hglrc) return FALSE;
	if (!wglMakeCurrent(m_hDC, hglrc)) return FALSE;

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		OutputDebugString(_T("Failed to initialize GLEW\r\n"));
		return FALSE;
	}


	m_programID = LoadShaders("SimpleTransform.vertexshader", "SingleColor.fragmentshader");

	m_ratio = 1;
	m_vecTranslate = vec3(0.0f);

	// Get a handle for our "MVP" uniform
	m_matrixID = glGetUniformLocation(m_programID, "MVP");

	glLineWidth(3.0f);

	return TRUE;
}

void COpenGL::SetHDC(HDC hDC)
{
	m_hDC = hDC;
}

void COpenGL::RenderScene()
{
	// Dark blue background
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
	//清除颜色缓冲区和深度缓冲区  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	// Send our transformation to the currently bound shader, 

	mat4 model = mat4(1.0f);

	//model = translate(model, m_vecTranslate);
	model = scale(model, vec3(m_ratio));

	mat4 modelViewMat = getModelViewMatrix();
	mat4 projMat = getProjectionMatrix();
	
	//projMat = ortho(1.0f, -1.0f, 1.0f, -1.0f, -200.0f, 200.0f);

	m_MVP = projMat * modelViewMat;
	// in the "MVP" uniform
	glUniformMatrix4fv(m_matrixID, 1, GL_FALSE, &m_MVP[0][0]);

	DrawBuffer();
	SwapBuffers(wglGetCurrentDC());
}

CBox COpenGL::getVisibleBox() const
{
	return CBox();
}

void COpenGL::invalidViewport()
{
	m_validProjectionMatrix = false;
}

void COpenGL::invalidateVisualization()
{
	m_validModelviewMatrix = false;
}

void COpenGL::setPivotPoint(vec3 P)
{
	m_viewportParams.pivotPoint = P;
	invalidViewport();
}

void COpenGL::setCameraPos(vec3 P)
{
	OutputString("(%f, %f, %f)\r\n", P.x, P.y, P.z);
	m_viewportParams.cameraCenter = P;
	invalidViewport();
	invalidateVisualization();

}


void COpenGL::DrawBuffer()
{
	for (size_t i = 0; i < m_vecBuffer.size(); i++)
	{
		glEnableVertexAttribArray(m_vecBuffer[i].index);
		glBindBuffer(GL_ARRAY_BUFFER, m_vecBuffer[i].buffer);
		glVertexAttribPointer(
			m_vecBuffer[i].index,          // attribute. Must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glDrawArrays(m_vecBuffer[i].type, 0, m_vecBuffer[i].size);
		glDisableVertexAttribArray(m_vecBuffer[i].index);
		glFlush();
	}
}

BOOL COpenGL::SetupPixelFormat(HDC hDC)
{
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,                     // version number 
		PFD_DRAW_TO_WINDOW |   // support window 
		PFD_SUPPORT_OPENGL |   // support OpenGL 
		PFD_DOUBLEBUFFER,      // double buffered 
		PFD_TYPE_RGBA,         // RGBA type 
		24,                    // 24-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     // no alpha buffer 
		0,                     // shift bit ignored 
		0,                     // no accumulation buffer 
		0, 0, 0, 0,            // accum bits ignored 
		32,                    // 32-bit z-buffer 
		0,                     // no stencil buffer 
		0,                     // no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer 
		0,                     // reserved 
		0, 0, 0                // layer masks ignored 
	};
	int  iPixelFormat;

	// get the best available match of pixel format for the device context  
	if ((iPixelFormat = ChoosePixelFormat(hDC, &pfd)) == 0) return FALSE;
	if (!SetPixelFormat(hDC, iPixelFormat, &pfd)) return FALSE;
	return TRUE;
}

bool COpenGL::InvertMatrix(const mat4& m, mat4& out)
{
	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;
	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = m[0][0], r0[1] = m[0][1],
		r0[2] = m[0][2], r0[3] = m[0][3],
		r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
		r1[0] = m[1][0], r1[1] = m[1][1],
		r1[2] = m[1][2], r1[3] = m[1][3],
		r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
		r2[0] = m[2][0], r2[1] = m[2][1],
		r2[2] = m[2][2], r2[3] = m[2][3],
		r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
		r3[0] = m[3][0], r3[1] = m[3][1],
		r3[2] = m[3][2], r3[3] = m[3][3],
		r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	//choose pivot - or die
	if (std::abs(r3[0]) > std::abs(r2[0]))
	{
		float* r = r3;
		r3 = r2;
		r2 = r;
	}
	if (std::abs(r2[0]) > std::abs(r1[0]))
	{
		float* r = r2;
		r2 = r1;
		r1 = r;
	}
	if (std::abs(r1[0]) > std::abs(r0[0]))
	{
		float* r = r1;
		r1 = r0;
		r0 = r;
	}
	if (0.0 == r0[0])
		return false;

	//eliminate first variable
	m1 = r1[0] / r0[0];
	m2 = r2[0] / r0[0];
	m3 = r3[0] / r0[0];
	s = r0[1];
	r1[1] -= m1 * s;
	r2[1] -= m2 * s;
	r3[1] -= m3 * s;
	s = r0[2];
	r1[2] -= m1 * s;
	r2[2] -= m2 * s;
	r3[2] -= m3 * s;
	s = r0[3];
	r1[3] -= m1 * s;
	r2[3] -= m2 * s;
	r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0)
	{
		r1[4] -= m1 * s;
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r0[5];
	if (s != 0.0)
	{
		r1[5] -= m1 * s;
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r0[6];
	if (s != 0.0)
	{
		r1[6] -= m1 * s;
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r0[7];
	if (s != 0.0)
	{
		r1[7] -= m1 * s;
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}

	//choose pivot - or die
	if (std::abs(r3[1]) > std::abs(r2[1]))
	{
		float* r = r3;
		r3 = r2;
		r2 = r;
	}
	if (std::abs(r2[1]) > std::abs(r1[1]))
	{
		float* r = r2;
		r2 = r1;
		r1 = r;
	}
	if (0.0 == r1[1])
		return false;

	//eliminate second variable
	m2 = r2[1] / r1[1];
	m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2];
	r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];
	r3[3] -= m3 * r1[3];
	s = r1[4];
	if (0.0 != s)
	{
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r1[5];
	if (0.0 != s)
	{
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r1[6];
	if (0.0 != s)
	{
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r1[7];
	if (0.0 != s)
	{
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}

	//choose pivot - or die
	if (std::abs(r3[2]) > std::abs(r2[2]))
	{
		float* r = r3;
		r3 = r2;
		r2 = r;
	}
	if (0.0 == r2[2])
		return false;

	//eliminate third variable
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
		r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];

	//last check
	if (0.0 == r3[3])
		return false;

	s = 1.0 / r3[3]; //now back substitute row 3
	r3[4] *= s;
	r3[5] *= s;
	r3[6] *= s;
	r3[7] *= s;
	m2 = r2[3]; //now back substitute row 2
	s = 1.0 / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
		r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
		r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
	m1 = r1[2]; //now back substitute row 1
	s = 1.0 / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
		r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
		r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
	m0 = r0[1]; //now back substitute row 0
	s = 1.0 / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
		r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

	out[0][0] = r0[4];
	out[0][1] = r0[5], out[0][2] = r0[6];
	out[0][3] = r0[7], out[1][0] = r1[4];
	out[1][1] = r1[5], out[1][2] = r1[6];
	out[1][3] = r1[7], out[2][0] = r2[4];
	out[2][1] = r2[5], out[2][2] = r2[6];
	out[2][3] = r2[7], out[3][0] = r3[4];
	out[3][1] = r3[5], out[3][2] = r3[6];
	out[3][3] = r3[7];

	return true;
}

mat4 COpenGL::initMatFromParameter(float phi_rad, float theta_rad, float psi_rad, const vec3 & t3D)
{
	mat4 mat;

	float c1 = cos(phi_rad);
	float c2 = cos(theta_rad);
	float c3 = cos(psi_rad);

	float s1 = sin(phi_rad);
	float s2 = sin(theta_rad);
	float s3 = sin(psi_rad);

	//1st column
	mat[0][0] = c2*c1;
	mat[1][0] = c2*s1;
	mat[2][0] = -s2;
	mat[3][0]= 0;

	//2nd column
	mat[0][1] = s3*s2*c1 - c3*s1;
	mat[1][1] = s3*s2*s1 + c3*c1;
	mat[2][1] = s3*c2;
	mat[3][1] = 0;

	//3rd column
	mat[0][2] = c3*s2*c1 + s3*s1;
	mat[1][2] = c3*s2*s1 - s3*c1;
	mat[2][2] = c3*c2;
	mat[3][2] = 0;

	//4th column
	mat[0][3] = t3D.x;
	mat[1][3] = t3D.y;
	mat[2][3] = t3D.z;
	mat[3][3] = static_cast<float>(1.0);

	return mat;
}

bool COpenGL::Unproject(const vec3& input2D, const mat4& modelview, const mat4& projection, const int* viewport, vec3& output3D)
{
	//compute projection x modelview
	mat4 A = projection * modelview;
	mat4 m;

	if (!InvertMatrix(A, m))
	{
		return false;
	}

	mat4 mA = m * A;

	//Transformation of normalized coordinates between -1 and 1
	vec4 in;
	in.x = static_cast<float>((input2D.x - static_cast<float>(viewport[0])) / viewport[2] * 2 - 1);
	in.y = static_cast<float>((input2D.y - static_cast<float>(viewport[1])) / viewport[3] * 2 - 1);
	in.z = static_cast<float>(2 * input2D.z - 1);
	in.w = 1;

	//Objects coordinates
	vec4 out = m * in;
	if (out.w == 0)
	{
		return false;
	}

	output3D = vec3(out[0], out[1], out[2]) / out.w;

	return true;
}

bool COpenGL::Project(const vec3& input3D, const mat4& modelview, const mat4& projection, const int* viewport, vec3& output2D)
{
	//Modelview transform
	vec4 Pm;
	{
		Pm.x = static_cast<float>(modelview[0][0] * input3D.x + modelview[0][1] * input3D.y + modelview[0][2] * input3D.z + modelview[0][3]);
		Pm.y = static_cast<float>(modelview[1][0] * input3D.x + modelview[1][1] * input3D.y + modelview[1][2] * input3D.z + modelview[1][3]);
		Pm.z = static_cast<float>(modelview[2][0] * input3D.x + modelview[2][1] * input3D.y + modelview[2][2] * input3D.z + modelview[2][3]);
		Pm.w = static_cast<float>(modelview[3][0] * input3D.x + modelview[3][1] * input3D.y + modelview[3][2] * input3D.z + modelview[3][3]);
	};

	//Projection transform
	vec4 Pp;
	{
		Pp.x = static_cast<float>(projection[0][0] * Pm.x + projection[0][1] * Pm.y + projection[0][2] * Pm.z + projection[0][3] * Pm.w);
		Pp.y = static_cast<float>(projection[1][0] * Pm.x + projection[1][1] * Pm.y + projection[1][2] * Pm.z + projection[1][3] * Pm.w);
		Pp.z = static_cast<float>(projection[2][0] * Pm.x + projection[2][1] * Pm.y + projection[2][2] * Pm.z + projection[2][3] * Pm.w);
		Pp.w = static_cast<float>(projection[3][0] * Pm.x + projection[3][1] * Pm.y + projection[3][2] * Pm.z + projection[3][3] * Pm.w);
	};

	//The result normalizes between -1 and 1
	if (Pp.w == 0.0)
	{
		return false;
	}
	//Perspective division
	Pp.x /= Pp.w;
	Pp.y /= Pp.w;
	Pp.z /= Pp.w;
	//Window coordinates
	//Map x, y to range 0-1
	output2D.x = (1.0 + Pp.x) / 2 * viewport[2] + viewport[0];
	output2D.y = (1.0 + Pp.y) / 2 * viewport[3] + viewport[1];
	//This is only correct when glDepthRange(0.0, 1.0)
	output2D.z = (1.0 + Pp.z) / 2;	//Between 0 and 1

	return true;
}

vec3 COpenGL::convertMousePositionToOrientation(int x, int y)
{
	CRect rect;
	GetClientRect(&rect);
	float xc = static_cast<double>(rect.Width() / 2);
	float yc = static_cast<double>(rect.Height() / 2);

	vec3 Q2D;

	//project the current pivot point on screen
	CameraParam camera;
	getGLCameraParameters(camera);

	if (!camera.project(m_viewportParams.pivotPoint, Q2D))
	{
		//arbitrary direction
		return vec3(0, 0, 1);
	}
	//we set the virtual rotation pivot closer to the actual one (but we always stay in the central part of the screen!)
	Q2D.x = min<GLfloat>(Q2D.x, 3 * rect.Width() / 4);
	Q2D.x = max<GLfloat>(Q2D.x, rect.Width() / 4);

	Q2D.y = min<GLfloat>(Q2D.y, 3 * rect.Height() / 4);
	Q2D.y = max<GLfloat>(Q2D.y, rect.Height() / 4);

	//invert y
	y = rect.Height() - 1 - y;

	vec3 v(x - Q2D.x, y - Q2D.y, 0);

	v.x = max<float>(min<float>(v.x / xc, 1), -1);
	v.y = max<float>(min<float>(v.y / yc, 1), -1);

	if (m_verticalRotationLocked || m_bubbleViewModeEnabled)
	{
		v.y = 0;
	}

	//square 'radius'
	double d2 = v.x*v.x + v.y*v.y;

	//projection on the unit sphere
	if (d2 > 1)
	{
		double d = sqrt(d2);
		v.x /= d;
		v.y /= d;
	}
	else
	{
		v.z = sqrt(1.0 - d2);
	}

	return v;
}

vec3 COpenGL::getRealCameraCenter() const
{

	//in orthographic mode, we put the camera at the center of the
	//visible objects (along the viewing direction)

	CBox box = getVisibleBox();
	return vec3(m_viewportParams.cameraCenter.x,
		m_viewportParams.cameraCenter.y,
		box.getCenter().z);
}


mat4 COpenGL::computeModelViewMatrix(const vec3& cameraCenter) const
{
	mat4 viewMatd;
	viewMatd[0][0] = viewMatd[1][1] = viewMatd[2][2] = viewMatd[3][3] = 1.0f;

	//place origin on pivot point
	//viewMatd.setTranslation(/*viewMatd.getTranslationAsVec3D()*/ -m_viewportParams.pivotPoint);
	viewMatd = translate(viewMatd, -m_viewportParams.pivotPoint);
	//viewMatd[3] = vec4(-m_viewportParams.pivotPoint, 1.0f);

	//rotation (viewMat is simply a rotation matrix around the pivot here!)
	viewMatd = m_viewportParams.viewMat * viewMatd;

	//go back to initial origin
	//then place origin on camera center
	//viewMatd.setTranslation(viewMatd.getTranslationAsVec3D() + m_viewportParams.pivotPoint - cameraCenter);
	vec3 translation = vec3(viewMatd[3].x, viewMatd[3].y, viewMatd[3].z);
	viewMatd[3] = vec4(translation + m_viewportParams.pivotPoint - cameraCenter, 1.0f);

	mat4 scaleMatd;
	scaleMatd[0][0] = scaleMatd[1][1] = scaleMatd[2][2] = scaleMatd[3][3] = 1.0f;
	//apply zoom
	float totalZoom = m_viewportParams.zoom / m_viewportParams.pixelSize;
	//glScalef(totalZoom,totalZoom,totalZoom);
	scaleMatd[0][0] = totalZoom;
	scaleMatd[1][1] = totalZoom;
	scaleMatd[2][2] = totalZoom;

	return scaleMatd * viewMatd;
}

mat4 COpenGL::computeProjectionMatrix(const vec3 & cameraCenter, bool withGLfeatures, ProjectionMetrics * metrics, double * eyeOffset) const
{
	float bbHalfDiag = 0.0f;
	vec3 bbCenter(0, 0, 0);

	CBox box = getVisibleBox();
	bbCenter = box.getCenter();
	bbHalfDiag = box.getNorm() / 2.0f;

	if (metrics)
	{
		metrics->bbHalfDiag = bbHalfDiag;
		metrics->cameraToBBCenterDist = sqrt(dot(cameraCenter - bbCenter, cameraCenter - bbCenter));
	}
	//virtual pivot point (i.e. to handle viewer-based mode smoothly)
	vec3 pivotPoint = m_viewportParams.pivotPoint;

	//distance between the camera center and the pivot point
	//warning: in orthographic mode it's important to get the 'real' camera center
	//(i.e. with z == bbCenter(z) and not z == anything)
	//otherwise we (sometimes largely) overestimate the distance between the camera center
	//and the displayed objects if the camera has been shifted in the Z direction (e.g. after
	//switching from perspective to ortho. view).
	//While the user won't see the difference this has a great influence on GL filters
	//(as normalized depth map values depend on it)
	float CP = sqrt(dot(cameraCenter - pivotPoint, cameraCenter - pivotPoint));

	//distance between the pivot point and DB farthest point
	float MP = sqrt(dot(bbCenter - pivotPoint, bbCenter - pivotPoint)) + bbHalfDiag;

	//pivot symbol should always be visible in object-based mode (if enabled)
	/*if (m_pivotSymbolShown && m_pivotVisibility != PIVOT_HIDE && withGLfeatures && m_viewportParams.objectCenteredView)
	{
		double pivotActualRadius = CC_DISPLAYED_PIVOT_RADIUS_PERCENT * std::min(m_glViewport.width(), m_glViewport.height()) / 2;
		double pivotSymbolScale = pivotActualRadius * computeActualPixelSize();
		MP = std::max<double>(MP, pivotSymbolScale);
	}*/
	MP *= 1.1; //for round-off issues

	//max distance (camera to 'farthest' point)
	float maxDist = CP + MP;
	maxDist = max<float>(maxDist, 0.1f);
	float halfDist = maxDist;

	//OutputString("half dist: %f\r\n", halfDist);
	mat4 projMat;

	if (m_glViewport.Width() <= m_glViewport.Height())
		projMat = ortho(
			-halfDist, 
			halfDist, 
			-halfDist * (float)m_glViewport.Height() / (float)m_glViewport.Width(), 
			halfDist * (float)m_glViewport.Height() / (GLfloat)m_glViewport.Width(), 
			-halfDist, 
			halfDist);
	else
		projMat = ortho(
			-halfDist * (float)m_glViewport.Width() / (float)m_glViewport.Height(),
			halfDist * (float)m_glViewport.Width() / (float)m_glViewport.Height(),
			-halfDist,
			halfDist,
			-halfDist,
			halfDist);

	if (metrics)
	{
		metrics->zNear = -halfDist;
		metrics->zFar = halfDist;
	}
	return projMat;
}

void COpenGL::updateModelViewMatrix()
{
	//we save visualization matrix
	m_viewMatd = computeModelViewMatrix(getRealCameraCenter());

	m_validModelviewMatrix = true;
}

void COpenGL::updateProjectionMatrix()
{
	ProjectionMetrics metrics;

	m_projMatd = computeProjectionMatrix
	(
		getRealCameraCenter(),
		true,
		&metrics,
		0
	); //no stereo vision by default!
	m_viewportParams.zNear = metrics.zNear;
	m_viewportParams.zFar = metrics.zFar;
	//m_cameraToBBCenterDist = metrics.cameraToBBCenterDist;
	//m_bbHalfDiag = metrics.bbHalfDiag;

	m_validProjectionMatrix = true;
}

const mat4& COpenGL::getModelViewMatrix()
{
	if (!m_validModelviewMatrix)
		updateModelViewMatrix();

	return m_viewMatd;
}

const mat4& COpenGL::getProjectionMatrix()
{
	if (!m_validProjectionMatrix)
		updateProjectionMatrix();

	return m_projMatd;
}

mat4 COpenGL::FromToRotation(const vec3 from, const vec3 to)
{
	float c = dot(from, to);
	float f = (c < 0 ? -c : c);
	mat4 result;

	if (1.0 - f < 1e-6) //"from" and "to"-vector almost parallel
	{
		// "to" vector most nearly orthogonal to "from"
		vec3 x(0, 0, 0);
		if (fabs(from.x) < fabs(from.y))
		{
			if (fabs(from.x) < fabs(from.z))
				x.x = static_cast<float>(1);
			else
				x.z = static_cast<float>(1);
		}
		else
		{
			if (fabs(from.y) < fabs(from.z))
				x.y = static_cast<float>(1);
			else
				x.z = static_cast<float>(1);
		}

		vec3 u = x - from;
		vec3 v = x - to;

		float c1 = 2 / dot(u, u);
		float c2 = 2 / dot(v, v);
		float c3 = c1 * c2  * dot(u, v);

		for (unsigned i = 0; i<3; i++)
		{
			for (unsigned j = 0; j<3; j++)
			{
				result[j][i] = c3 * v[i] * u[j]
					- c2 * v[i] * v[j]
					- c1 * u[i] * u[j];
			}
			result[i][i] += static_cast<float>(1);
		}
	}
	else  // the most common case, unless "from"="to", or "from"=-"to"
	{
		//see Efficiently Building a Matrix to Rotate One Vector to Another
		//T. Moller and J.F. Hugues (1999)
		vec3 v = cross(from, to);
		float h = 1 / (1 + c);
		float hvx = h * v.x;
		float hvz = h * v.z;
		float hvxy = hvx * v.y;
		float hvxz = hvx * v.z;
		float hvyz = hvz * v.y;

		result[0][0] = c + hvx * v.x;
		result[1][0] = hvxy + v.z;
		result[2][0] = hvxz - v.y;

		result[0][1] = hvxy - v.z;
		result[1][1] = c + h * v.y * v.y;
		result[2][1] = hvyz + v.x;

		result[0][2] = hvxz + v.y;
		result[1][2] = hvyz - v.x;
		result[2][2] = c + hvz * v.z;
	}

	return result;
}

void COpenGL::getGLCameraParameters(CameraParam & params)
{
	//get/compute the modelview matrix
	{
		params.modelViewMat = getModelViewMatrix();
	}

	//get/compute the projection matrix
	{
		params.projectionMat = getProjectionMatrix();
	}

	params.viewport[0] = m_glViewport.left;
	params.viewport[1] = m_glViewport.top;
	params.viewport[2] = m_glViewport.Width();
	params.viewport[3] = m_glViewport.Height();

	params.pixelSize = m_viewportParams.pixelSize;
}

int COpenGL::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_pDC = new CClientDC(this);
	SetHDC(m_pDC->GetSafeHdc());
	if (!init())
		MessageBox(_T("Init opengl error"));
	return 0;
}


void COpenGL::rotateBaseViewMat(const mat4 rotMat)
{
	if (dot(rotMat[0], rotMat[0]) < 1e-6)
		return;
	m_viewportParams.viewMat = rotMat * m_viewportParams.viewMat;
	invalidateVisualization();
}

void COpenGL::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_pDC != NULL)
		delete m_pDC;
	m_pDC = NULL;
}


void COpenGL::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (cy == 0)
	{
		cy = 1;
	}

	m_glViewport = CRect(0, 0, cx, cy);
	glViewport(0, 0, cx, cy);

	invalidViewport();
	invalidateVisualization();
}


BOOL COpenGL::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}

//! get transformation from 3 rotation angles and a translation
/** See http://en.wikipedia.org/wiki/Euler_angles (Tait-Bryan Z1Y2X3)
\param[in] phi_rad Phi angle (in radians)
\param[in] theta_rad Theta angle (in radians)
\param[in] psi_rad Psi angle (in radians)
\param[in] t3D translation
**/
mat4 COpenGL::getFromParameters(float phi_rad, float theta_rad, float psi_rad,	const vec3& t3D)
{
	float c1 = cos(phi_rad);
	float c2 = cos(theta_rad);
	float c3 = cos(psi_rad);

	float s1 = sin(phi_rad);
	float s2 = sin(theta_rad);
	float s3 = sin(psi_rad);

	mat4 mat;
	//1st column
	mat[0][0] = c2*c1;
	mat[1][0] = c2*s1;
	mat[2][0] = -s2;
	mat[3][0] = 0;

	//2nd column
	mat[0][1] = s3*s2*c1 - c3*s1;
	mat[1][1] = s3*s2*s1 + c3*c1;
	mat[2][1] = s3*c2;
	mat[3][1] = 0;

	//3rd column
	mat[0][2] = c3*s2*c1 + s3*s1;
	mat[1][2] = c3*s2*s1 - s3*c1;
	mat[2][2] = c3*c2;
	mat[3][2] = 0;

	//4th column
	mat[0][3] = t3D.x;
	mat[1][3] = t3D.y;
	mat[2][3] = t3D.z;
	mat[3][3] = static_cast<float>(1.0);

	return mat;
}

void COpenGL::moveCamera(float dx, float dy, float dz)
{
	vec3 V(dx, dy, dz);

	setCameraPos(m_viewportParams.cameraCenter + V);
}

void COpenGL::setBaseViewMat(mat4 & mat)
{
	m_viewportParams.viewMat = mat;
	invalidateVisualization();
}


void COpenGL::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nFlags & MK_LBUTTON)
	{
		m_currentMouseOrientation = convertMousePositionToOrientation(point.x, point.y);
		mat4 rotateMat = FromToRotation(m_lastMouseOrientation, m_currentMouseOrientation);
		m_lastMouseOrientation = m_currentMouseOrientation;

		rotateBaseViewMat(rotateMat);
		invalidViewport();
		RenderScene();
	}
	if (nFlags & MK_RBUTTON)
	{
		int dx = point.x - m_lastPoint.x;
		int dy = point.y - m_lastPoint.y;

		vec3 u(dx * 0.001, dy * 0.001, 0);
		moveCamera(u.x, u.y, u.z);

		//vec3 vec(0, fDiffX * 0.005f, fDiffY * 0.005f);
		//m_vecTranslate = m_vecTranslate + vec;
		//m_vecTranslate = product(m_rotateMatrix, m_vecTranslate);
		////m_vecView = m_vecView + vec;
		////m_vecCamera = m_vecCamera + vec;
		//OutputString("%lf, %lf, %lf\r\n", vec[0], vec[1], vec[2]);
		//OutputString("%lf, %lf, %lf\r\n", m_vecTranslate[0], m_vecTranslate[1], m_vecTranslate[2]);
		RenderScene();
	}

	m_lastPoint = point;

	CView::OnMouseMove(nFlags, point);
}


BOOL COpenGL::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_ratio *= (zDelta > 0) ? 0.9f : 1.1f;
	RenderScene();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void COpenGL::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_lastMouseOrientation = convertMousePositionToOrientation(point.x, point.y);
	CView::OnLButtonDown(nFlags, point);
}

void COpenGL::PushBuffer(const int nIndex, int nType, const float * data, int size)
{
	DrawParam drawParam;
	GLuint vertexBuffer;

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float), data, GL_STATIC_DRAW);

	drawParam.index = nIndex;
	drawParam.buffer = vertexBuffer;
	drawParam.size = size;
	drawParam.type = nType;
	m_vecBuffer.push_back(drawParam);
}
