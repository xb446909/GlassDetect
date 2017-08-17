
// GlassDetectView.cpp : CGlassDetectView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_COMMAND(IDM_CAMERA_SETTING, &CGlassDetectView::OnCameraSetting)
END_MESSAGE_MAP()

// CGlassDetectView ����/����

CGlassDetectView::CGlassDetectView()
{
	// TODO: �ڴ˴���ӹ������

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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CGlassDetectView ����

void CGlassDetectView::OnDraw(CDC* /*pDC*/)
{
	CGlassDetectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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


// CGlassDetectView ��ӡ

BOOL CGlassDetectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CGlassDetectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CGlassDetectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CGlassDetectView ���

#ifdef _DEBUG
void CGlassDetectView::AssertValid() const
{
	CView::AssertValid();
}

void CGlassDetectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGlassDetectDoc* CGlassDetectView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGlassDetectDoc)));
	return (CGlassDetectDoc*)m_pDocument;
}
#endif //_DEBUG


// CGlassDetectView ��Ϣ�������


int CGlassDetectView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COpenGL::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	glPointSize(5);
	return 0;
}


void CGlassDetectView::OnCameraSetting()
{
	// TODO: �ڴ���������������
	if (!IsWindow(m_dialogCameraSetting.GetSafeHwnd()))
		m_dialogCameraSetting.Create(IDD_DIALOG_CAMEARA_SETTING, this);
	m_dialogCameraSetting.ShowWindow(SW_SHOWNORMAL);
}
