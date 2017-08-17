
// GlassDetectView.cpp : CGlassDetectView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "GlassDetect.h"
#endif

#include "GlassDetectDoc.h"
#include "GlassDetectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGlassDetectView

IMPLEMENT_DYNCREATE(CGlassDetectView, COpenGL)

BEGIN_MESSAGE_MAP(CGlassDetectView, COpenGL)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_COMMAND(IDM_CAMERA_SETTING, &CGlassDetectView::OnCameraSetting)
END_MESSAGE_MAP()

// CGlassDetectView 构造/析构

CGlassDetectView::CGlassDetectView()
{
	// TODO: 在此处添加构造代码

}

void CGlassDetectView::UpdateView()
{
	ClearBuffer();
	setBaseView();
	setPivotPoint(getVisibleBox().getCenter());
	setCameraPos(getVisibleBox().getCenter());

	invalidateVisualization();
	invalidViewport();
}

CGlassDetectView::~CGlassDetectView()
{
}

BOOL CGlassDetectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGlassDetectView 绘制

void CGlassDetectView::OnDraw(CDC* /*pDC*/)
{
	CGlassDetectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (!pDoc->IsValid())
	{
		for (size_t i = 0; i < pDoc->m_vecPrimitives.size(); i++)
		{
			CGLPrimitive* pPrimitive = pDoc->m_vecPrimitives[i];
			if ((pPrimitive->getData() != nullptr) && (pPrimitive->getDataLength() != 0))
			{
				PushBuffer(0, pPrimitive->getType(), pPrimitive->getData(), pPrimitive->getDataLength());
			}
		}
		pDoc->SetValid(true);
	}

	RenderScene();
}


// CGlassDetectView 打印

BOOL CGlassDetectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGlassDetectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGlassDetectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

CBox CGlassDetectView::getVisibleBox() const
{
	CGlassDetectDoc* pDoc = GetDocument();
	if (!pDoc) return CBox();
	CBox box;
	for (int i = 0; i < pDoc->m_vecPrimitives.size(); i++)
	{
		CGLPrimitive* pPrimitive = pDoc->m_vecPrimitives[i];
		box += pPrimitive->getBox();
	}
	return box;
}


// CGlassDetectView 诊断

#ifdef _DEBUG
void CGlassDetectView::AssertValid() const
{
	CView::AssertValid();
}

void CGlassDetectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGlassDetectDoc* CGlassDetectView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGlassDetectDoc)));
	return (CGlassDetectDoc*)m_pDocument;
}
#endif //_DEBUG


// CGlassDetectView 消息处理程序


int CGlassDetectView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COpenGL::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	glPointSize(5);
	return 0;
}


void CGlassDetectView::OnCameraSetting()
{
	// TODO: 在此添加命令处理程序代码
	if (!IsWindow(m_dialogCameraSetting.GetSafeHwnd()))
		m_dialogCameraSetting.Create(IDD_DIALOG_CAMEARA_SETTING, this);
	m_dialogCameraSetting.ShowWindow(SW_SHOWNORMAL);
}
