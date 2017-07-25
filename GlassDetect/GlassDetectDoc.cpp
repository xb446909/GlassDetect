
// GlassDetectDoc.cpp : CGlassDetectDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "GlassDetect.h"
#endif

#include "GlassDetectDoc.h"
#include "DialogReadProgress.h"
#include "GlassDetectView.h"

#include <propkey.h>
#include <fstream>
#include <sstream>

using namespace std;

#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGlassDetectDoc

IMPLEMENT_DYNCREATE(CGlassDetectDoc, CDocument)

BEGIN_MESSAGE_MAP(CGlassDetectDoc, CDocument)
END_MESSAGE_MAP()


// CGlassDetectDoc ����/����

CGlassDetectDoc::CGlassDetectDoc()
{
	// TODO: �ڴ����һ���Թ������
}

CGlassDetectDoc::~CGlassDetectDoc()
{
}

BOOL CGlassDetectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CGlassDetectDoc ���л�

void CGlassDetectDoc::Serialize(CArchive& ar)
{
	CString filePath;
	CFile* pFile = ar.GetFile();
	filePath = pFile->GetFilePath();
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
		pFile->Close();
		for (auto it = m_mapPrimitives.begin(); it != m_mapPrimitives.end(); it++)
		{
			delete it->second;
		}
		m_mapPrimitives.clear();

		CGLPrimitive* pPrimitive = CGLPrimitive::Create(GL_POINTS);
		m_mapPrimitives.insert(pair<int, CGLPrimitive*>(GL_POINTS, pPrimitive));

		ReadFile(CT2A(filePath));
		pFile->Open(filePath, CFile::modeRead);
		UpdateView();
	}
}

void CGlassDetectDoc::ReadFile(const char * szPath)
{
	FILE* pFile = fopen(szPath, "rb");
	if (pFile == NULL)
	{
		AfxMessageBox(_T("���ļ�����"));
		return;
	}

	/* ��ȡ�ļ���С */
	fseek(pFile, 0, SEEK_END);
	int lSize = ftell(pFile);
	rewind(pFile);

	/* �����ڴ�洢�����ļ� */
	char* buffer = (char*)malloc(sizeof(char)*lSize);
	memset(buffer, 0, sizeof(char)*lSize);
	if (buffer == NULL) return;

	/* ���ļ�������buffer�� */
	int result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		AfxMessageBox(_T("��ȡ�ļ�����"));
		free(buffer);
		return;
	}

	int nPos = 0;
	CDialogReadProgress readProgress;
	readProgress.Create(IDD_DIALOG_READ_PROGRESS);
	readProgress.ShowWindow(SW_SHOWNORMAL);
	readProgress.SetRange(lSize);

	CRect rect;
	CRect rectProgress;
	readProgress.GetClientRect(rectProgress);
	AfxGetMainWnd()->GetClientRect(rect);
	AfxGetMainWnd()->ClientToScreen(rect);
	readProgress.SetWindowPos(
		NULL,
		rect.left + (rect.Width() - rectProgress.Width()) / 2,
		rect.top + (rect.Height() - rectProgress.Height()) / 2, 0, 0, SWP_NOSIZE);

	float tempx, tempy, tempz;
	char delims[] = "\n";
	char *r = NULL;
	r = strtok(buffer, delims);
	while (r != NULL) {
		nPos += strlen(r);
		MSG msg;
		readProgress.SetCurrent(nPos);
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			if (msg.message == WM_NCLBUTTONDOWN)
				continue;
			DispatchMessage(&msg);
		}

		if (sscanf(r, "%f;%f;%f", &tempx, &tempy, &tempz) == 3)
			m_mapPrimitives[GL_POINTS]->PushPoint(tempx, tempy, tempz);
		r = strtok(NULL, delims);
	}

	/* ������ʾ���ر��ļ����ͷ��ڴ� */
	fclose(pFile);
	free(buffer);
}


#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CGlassDetectDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CGlassDetectDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CGlassDetectDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGlassDetectDoc ���

#ifdef _DEBUG
void CGlassDetectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGlassDetectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGlassDetectDoc ����


void CGlassDetectDoc::OnCloseDocument()
{
	// TODO: �ڴ����ר�ô����/����û���
	for (auto it = m_mapPrimitives.begin(); it != m_mapPrimitives.end(); it++)
	{
		delete it->second;
	}
	m_mapPrimitives.clear();
	CDocument::OnCloseDocument();
}

void CGlassDetectDoc::UpdateView()
{
	SetValid(false);
	POSITION pos = GetFirstViewPosition();
	CGlassDetectView* pGlassDetectView = reinterpret_cast<CGlassDetectView*>(GetNextView(pos));
	pGlassDetectView->UpdateView();
}
