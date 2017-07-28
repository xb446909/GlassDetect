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
	typedef struct tagViewportParam
	{
		tagViewportParam()
		{
			zoom = 1.0f;
			pixelSize = 1.0f;
			orthoAspectRatio = 1.0f;
			viewMat[0][0] = viewMat[1][1] = viewMat[2][2] = viewMat[3][3] = 1.0f;
		}

		//! Current pixel size (in 'current unit'/pixel)
		/** This scale is valid eveywhere in ortho. mode
		or at the focal distance in perspective mode.
		Warning: doesn't take current zoom into account!
		**/
		float pixelSize;

		//! Current zoom
		float zoom;

		//! Visualization matrix (rotation only)
		mat4 viewMat;

		//! Point size
		float defaultPointSize;
		//! Line width
		float defaultLineWidth;

		//! Actual perspective 'zNear' value
		double zNear;
		//! Actual perspective 'zFar' value
		double zFar;

		//! Rotation pivot point (for object-centered view modes)
		vec3 pivotPoint;

		//! Camera center (for perspective mode)
		vec3 cameraCenter;

		//! 3D view aspect ratio (ortho mode only)
		/** AR = width / height
		**/
		float orthoAspectRatio;
	}ViewParam;

	typedef struct tagCameraParam
	{
		tagCameraParam()
			: pixelSize(1.0f)
		{
			memset(viewport, 0, 4 * sizeof(int));
		}

		//! Projects a 3D point in 2D (+ normalized 'z' coordinate)
		inline bool project(const vec3& input3D, vec3& output2D) const { return COpenGL::Project(input3D, modelViewMat, projectionMat, viewport, output2D); }
		
		//! Unprojects a 2D point (+ normalized 'z' coordinate) in 3D
		inline bool unproject(const vec3& input2D, vec3& output3D) const { return COpenGL::Unproject(input2D, modelViewMat, projectionMat, viewport, output3D); }

		//! Model view matrix (GL_MODELVIEW)
		mat4 modelViewMat;
		//! Projection matrix (GL_PROJECTION)
		mat4 projectionMat;
		//! Viewport (GL_VIEWPORT)
		int viewport[4];
		//! Pixel size (i.e. zoom) - non perspective mode only
		float pixelSize;
	}CameraParam;

	//! Optional output metrics (from computeProjectionMatrix)
	struct ProjectionMetrics
	{
		ProjectionMetrics() : zNear(0), zFar(0), cameraToBBCenterDist(0), bbHalfDiag(0) {}
		double zNear, zFar, cameraToBBCenterDist, bbHalfDiag;
	};

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

	vec3 m_vecCamera;
	vec3 m_vecView;
	vec3 m_vecHead;

	vec3 m_vecAxisX;
	vec3 m_vecAxisY;
	vec3 m_vecAxisZ;
	mat4 m_rotateMatrix;


	float m_ratio;
	vec3 m_vecTranslate;

	ViewParam m_viewportParams;

	bool m_verticalRotationLocked;
	bool m_bubbleViewModeEnabled;
	
	CRect m_glViewport;
	mat4 m_viewMatd;
	bool m_validModelviewMatrix;
	mat4 m_projMatd;
	bool m_validProjectionMatrix;

	void DrawBuffer();
	BOOL SetupPixelFormat(HDC hDC);
	vec3 convertMousePositionToOrientation(int x, int y);
	void getGLCameraParameters(CameraParam& params);
	vec3 getRealCameraCenter() const;
	mat4 computeModelViewMatrix(const vec3 & cameraCenter) const;
	mat4 computeProjectionMatrix(const vec3& cameraCenter,
		bool withGLfeatures,
		ProjectionMetrics* metrics = 0,
		double* eyeOffset = 0) const;
	void updateModelViewMatrix();
	void updateProjectionMatrix();
	const mat4 & getModelViewMatrix();
	const mat4 & getProjectionMatrix();
	mat4 FromToRotation(const vec3 from, const vec3 to);
	vec3 m_currentMouseOrientation;
	vec3 m_lastMouseOrientation;
	void rotateBaseViewMat(const mat4 rotMat);
	mat4 getFromParameters(float phi_rad, float theta_rad, float psi_rad, const vec3& t3D);
	void moveCamera(float dx, float dy, float dz);
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	static bool Project(const vec3& input3D, const mat4& modelview, const mat4& projection, const int* viewport, vec3& output2D);
	static bool Unproject(const vec3& input2D, const mat4& modelview, const mat4& projection, const int* viewport, vec3& output3D);

	static bool InvertMatrix(const mat4& m, mat4& out);
	static mat4 initMatFromParameter(float phi_rad, float theta_rad, float psi_rad, const vec3& t3D);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void PushBuffer(const int nIndex, int nType, const float* data, int size);
	void setBaseViewMat(mat4& mat);
	void setPivotPoint(vec3 P);
	void setCameraPos(vec3 P);
	void RenderScene();
};

