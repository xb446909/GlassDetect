
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
		ULONG64 nPos = 0;
		ULONG64 nTotalLength = pFile->GetLength();
		CDialogReadProgress readProgress;
		readProgress.Create(IDD_DIALOG_READ_PROGRESS);
		readProgress.ShowWindow(SW_SHOWNORMAL);
		readProgress.SetRange(nTotalLength);

		CRect rect;
		CRect rectProgress;
		readProgress.GetClientRect(rectProgress);
		AfxGetMainWnd()->GetClientRect(rect);
		AfxGetMainWnd()->ClientToScreen(rect);
		readProgress.SetWindowPos(
			NULL, 
			rect.left + (rect.Width() - rectProgress.Width()) / 2, 
			rect.top + (rect.Height() - rectProgress.Height()) / 2, 0, 0, SWP_NOSIZE);

		string str;
		ifstream ifs;
		ifs.open(CT2A(filePath));
		if (!ifs.bad())
		{
			for (auto it = m_mapPrimitives.begin(); it != m_mapPrimitives.end(); it++)
			{
				delete it->second;
			}
			m_mapPrimitives.clear();

			int n = ifs.end;
			while (getline(ifs, str))
			{
				MSG msg;
				readProgress.SetCurrent(nPos);
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					if (msg.message == WM_NCLBUTTONDOWN)
						continue;
					DispatchMessage(&msg);
				}
				nPos += str.length();

				size_t pos1 = str.find_first_of(';');
				size_t pos2 = str.find_last_of(';');
				if ((pos1 == pos2) || (pos1 == string::npos) || (pos2 == string::npos))
				{
					OutputDebugStringA("Load file failed!\r\n");
					break;
				}
				string szX = str.substr(0, pos1);
				string szY = str.substr(pos1 + 1, pos2 - pos1 - 1);
				string szZ = str.substr(pos2 + 1, str.length() - pos2);
				if (m_mapPrimitives.find(GL_POINTS) == m_mapPrimitives.end())
				{
					CGLPrimitive* pPrimitive = CGLPrimitive::Create(GL_POINTS);
					m_mapPrimitives.insert(pair<int, CGLPrimitive*>(GL_POINTS, pPrimitive));
				}
				m_mapPrimitives[GL_POINTS]->PushPoint(
					atof(szX.c_str()),
					atof(szY.c_str()),
					atof(szZ.c_str()));
			}
			m_mapPrimitives[GL_POINTS]->updateBox();
		}
		UpdateView();
	}
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
