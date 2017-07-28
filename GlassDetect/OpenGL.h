#pragma once
#include "afxwin.h"

#include "Box.h"

#include <vector>
#include <map>

using namespace std;

class COpenGL :
	public CView
{
public:
	typedef struct tagModelViewPararm
	{
		tagModelViewPararm()
		{
			axisY = vec3(0, 1, 0);
			zoom = 1.0f;
			translate = vec3(0.0f);
		}

		void setAxis()
		{
			axisY = normalize(head);
			axisZ = camera - view;
			axisX = cross(axisY, axisZ);
			axisX = normalize(axisX);
		}

		vec3 axisX;
		vec3 axisY;
		vec3 axisZ;
		vec3 camera;
		vec3 view;
		vec3 head;
		float zoom;
		vec3 translate;
	}ModelViewPararm;

	typedef struct tagProjectParam
	{
		float zNear;
		float zFar;
		float orthoAspectRatio;
	}ProjectParam;

	typedef struct tagViewportParam
	{
		tagViewportParam()
		{
			pixelSize = 1.0f;
			memset(viewport, 0, 4 * sizeof(int));
		}

		float pixelSize;

		//! Point size
		float defaultPointSize;
		//! Line width
		float defaultLineWidth;

		int viewport[4];
	}ViewportParam;

	typedef struct tagDrawParam
	{
		int index;
		int type;
		GLuint buffer;
		int size;

		tagDrawParam()
		{
			index = 0;
			type = 0;
			buffer = 0;
			size = 0;
		}

		const tagDrawParam& operator=(tagDrawParam param)
		{
			index = param.index;
			type = param.type;
			buffer = param.buffer;
			size = param.size;
			return *this;
		}
	}DrawParam, *pDrawParam;
public:
	COpenGL();
	virtual ~COpenGL();
	static void OutputString(char * fmt, ...);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	BOOL init();
	void SetHDC(HDC hDC);
	virtual CBox getVisibleBox() const;
	void invalidViewport();
	void invalidateVisualization();
private:
	CDC* m_pDC;
	HDC m_hDC;
	mat4 m_MVP;
	GLuint m_matrixID;
	GLuint m_programID;
	CPoint m_lastPoint;

	vector<DrawParam> m_vecBuffer;

	ViewportParam m_viewportParams;
	ModelViewPararm m_modelViewParam;
	ProjectParam m_projParam;
	
	mat4 m_modelViewMat;
	bool m_validModelViewMatrix;
	mat4 m_projMat;
	bool m_validProjectionMatrix;

	void DrawBuffer();
	BOOL SetupPixelFormat(HDC hDC);
	mat4 computeModelViewMatrix();
	mat4 computeProjectionMatrix();
	void updateModelViewMatrix();
	void updateProjectionMatrix();
	const mat4 & getModelViewMatrix();
	const mat4 & getProjectionMatrix();
	void moveCamera(float dx, float dy, float dz);
	mat4 getRotateMatrix(float angle, const vec3 &vector);
	vec3 productRotMat(const mat4& mat, const vec3& vec);

	void computeRotVectorAngle(const float deltaX, const float deltaY, vec3& rotVec, float& angle);
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void PushBuffer(const int nIndex, int nType, const float* data, int size);
	void setPivotPoint(vec3 P);
	void setCameraPos(vec3 P);
	void RenderScene();
	void setBaseView();
	void setRotate(mat4 rotMat);
};

