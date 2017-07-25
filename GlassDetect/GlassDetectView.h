
// GlassDetectView.h : CGlassDetectView ��Ľӿ�
//

#pragma once

#include "OpenGL.h"
#include "DialogCameraSetting.h"

class CGlassDetectView : public COpenGL
{
protected: // �������л�����
	CGlassDetectView();
	DECLARE_DYNCREATE(CGlassDetectView)

// ����
public:
	CGlassDetectDoc* GetDocument() const;
	void UpdateView();
// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
private:
	CDialogCameraSetting m_dialogCameraSetting;

	virtual CBox getVisibleBox() const;

// ʵ��
public:
	virtual ~CGlassDetectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCameraSetting();
};

#ifndef _DEBUG  // GlassDetectView.cpp �еĵ��԰汾
inline CGlassDetectDoc* CGlassDetectView::GetDocument() const
   { return reinterpret_cast<CGlassDetectDoc*>(m_pDocument); }
#endif

