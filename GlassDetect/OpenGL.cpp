#include "stdafx.h"
#include "OpenGL.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "../GL/lib/glew32s.lib")

COpenGL::COpenGL()
	: m_validModelViewMatrix(false)
	, m_validProjectionMatrix(false)
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

	mat4 modelViewMat = getModelViewMatrix();
	mat4 projMat = getProjectionMatrix();

	m_MVP = projMat * modelViewMat;
	// in the "MVP" uniform
	glUniformMatrix4fv(m_matrixID, 1, GL_FALSE, &m_MVP[0][0]);

	DrawBuffer();
	SwapBuffers(wglGetCurrentDC());
}

void COpenGL::setBaseView()
{
	updateModelViewMatrix();
	updateProjectionMatrix();
	m_modelViewParam.camera = vec3(0.0f, 0.0f, 4.0f);
	m_modelViewParam.view = vec3(0.0f);
	m_modelViewParam.head = vec3(0.0f, 1.0f, 0.0f);
	m_modelViewParam.setAxis();
}

void COpenGL::setRotate(mat4 rotMat)
{
	m_modelViewParam.camera = productRotMat(rotMat, m_modelViewParam.camera);
	m_modelViewParam.head = productRotMat(rotMat, m_modelViewParam.head);
	m_modelViewParam.head = normalize(m_modelViewParam.head);
	m_modelViewParam.setAxis();
	invalidateVisualization();
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
	m_validModelViewMatrix = false;
}

void COpenGL::setPivotPoint(vec3 P)
{
	invalidViewport();
}

void COpenGL::setCameraPos(vec3 P)
{
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


mat4 COpenGL::computeModelViewMatrix()
{
	mat4 view = lookAt(
		m_modelViewParam.camera,
		m_modelViewParam.view,
		m_modelViewParam.head);
	mat4 model = translate(mat4(), m_modelViewParam.translate);
	model = scale(model, vec3(m_modelViewParam.zoom));
	return model * view;
}

mat4 COpenGL::computeProjectionMatrix()
{
	CBox box = getVisibleBox();
	float half_dis = box.getNorm() / 2.0f;
	half_dis += length(box.getCenter());
	m_projParam.zNear = -10.0f * half_dis;
	m_projParam.zFar = m_projParam.zNear * -1.0f;

	mat4 projMat;
	if (m_viewportParams.viewport[2] <= m_viewportParams.viewport[3])
		projMat = ortho(
			-half_dis, 
			half_dis, 
			-half_dis * (GLfloat)m_viewportParams.viewport[3] / (GLfloat)m_viewportParams.viewport[2], 
			half_dis * (GLfloat)m_viewportParams.viewport[3] / (GLfloat)m_viewportParams.viewport[2], 
			m_projParam.zNear, 
			m_projParam.zFar);
	else
		projMat = ortho(
			-half_dis * (GLfloat)m_viewportParams.viewport[2] / (GLfloat)m_viewportParams.viewport[3], 
			half_dis * (GLfloat)m_viewportParams.viewport[2] / (GLfloat)m_viewportParams.viewport[3], 
			-half_dis, 
			half_dis, 
			m_projParam.zNear, 
			m_projParam.zFar);

	return projMat;
}

void COpenGL::updateModelViewMatrix()
{
	//we save visualization matrix
	m_modelViewMat = computeModelViewMatrix();

	m_validModelViewMatrix = true;
}

void COpenGL::updateProjectionMatrix()
{

	m_projMat = computeProjectionMatrix(); 

	m_validProjectionMatrix = true;
}

const mat4& COpenGL::getModelViewMatrix()
{
	if (!m_validModelViewMatrix)
		updateModelViewMatrix();

	return m_modelViewMat;
}

const mat4& COpenGL::getProjectionMatrix()
{
	if (!m_validProjectionMatrix)
		updateProjectionMatrix();

	return m_projMat;
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

	m_viewportParams.viewport[0] = 0;
	m_viewportParams.viewport[1] = 0;
	m_viewportParams.viewport[2] = cx;
	m_viewportParams.viewport[3] = cy;
	glViewport(0, 0, cx, cy);

	invalidViewport();
	invalidateVisualization();
}


BOOL COpenGL::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}

void COpenGL::moveCamera(float dx, float dy, float dz)
{
	vec3 V(dx, dy, dz);

	//setCameraPos(m_viewportParams.cameraCenter + V);
}

mat4 COpenGL::getRotateMatrix(float angle, const vec3 & vector)
{
	angle = radians(angle);
	vec3 vec = normalize(vector);
	float cosa = cos(angle);
	float sina = sin(angle);

	double a = vec[0] * sina;
	double b = vec[1] * sina;
	double c = vec[2] * sina;

	mat4 matrix;

	matrix[0][0] = 1.0 - 2.0*(b*b + c*c);
	matrix[1][1] = 1.0 - 2.0*(c*c + a*a);
	matrix[2][2] = 1.0 - 2.0*(a*a + b*b);

	matrix[0][1] = 2.0 * (a*b - c*cosa);
	matrix[0][2] = 2.0 * (a*c + b*cosa);

	matrix[1][0] = 2.0 * (a*b + c*cosa);
	matrix[1][2] = 2.0 * (b*c - a*cosa);

	matrix[2][0] = 2.0 * (a*c - b*cosa);
	matrix[2][1] = 2.0 * (b*c + a*cosa);

	return matrix;
}

vec3 COpenGL::productRotMat(const mat4 & mat, const vec3 & vec)
{
	float x, y, z;
	x = vec[0] * mat[0][0] +
		vec[1] * mat[0][1] +
		vec[2] * mat[0][2];

	y = vec[0] * mat[1][0] +
		vec[1] * mat[1][1] +
		vec[2] * mat[1][2];

	z = vec[0] * mat[2][0] +
		vec[1] * mat[2][1] +
		vec[2] * mat[2][2];

	return vec3(x, y, z);
}

void COpenGL::computeRotVectorAngle(const float deltaX, const float deltaY, vec3 & rotVec, float & angle)
{
	vec3 mouse = m_modelViewParam.axisX * deltaX + m_modelViewParam.axisY * deltaY;
	rotVec = cross(mouse, m_modelViewParam.axisZ);
	rotVec += m_modelViewParam.view;
	angle = length(mouse);
}


void COpenGL::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	int dx = point.x - m_lastPoint.x;
	int dy = point.y - m_lastPoint.y;

	if (nFlags & MK_LBUTTON)
	{
		vec3 rotVec;
		float angle;
		computeRotVectorAngle(0.1f * dx, -0.1f * dy, rotVec, angle);
		mat4 rotateMat = getRotateMatrix(angle, rotVec);
		setRotate(rotateMat);
		invalidViewport();
		RenderScene();
	}
	if (nFlags & MK_RBUTTON)
	{
		

		vec3 u(dx * 0.001, dy * 0.001, 0);
		moveCamera(u.x, u.y, u.z);

		RenderScene();
	}

	m_lastPoint = point;

	CView::OnMouseMove(nFlags, point);
}


BOOL COpenGL::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RenderScene();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void COpenGL::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
