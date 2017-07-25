
// GlassDetectView.h : CGlassDetectView 类的接口
//

#pragma once

#include "OpenGL.h"
#include "DialogCameraSetting.h"

class CGlassDetectView : public COpenGL
{
protected: // 仅从序列化创建
	CGlassDetectView();
	DECLARE_DYNCREATE(CGlassDetectView)

// 特性
public:
	CGlassDetectDoc* GetDocument() const;
	void UpdateView();
// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
private:
	CDialogCameraSetting m_dialogCameraSetting;

	virtual CBox getVisibleBox() const;

// 实现
public:
	virtual ~CGlassDetectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCameraSetting();
};

#ifndef _DEBUG  // GlassDetectView.cpp 中的调试版本
inline CGlassDetectDoc* CGlassDetectView::GetDocument() const
   { return reinterpret_cast<CGlassDetectDoc*>(m_pDocument); }
#endif

