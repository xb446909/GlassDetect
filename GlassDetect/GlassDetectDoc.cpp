
// GlassDetectDoc.cpp : CGlassDetectDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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


// CGlassDetectDoc 构造/析构

CGlassDetectDoc::CGlassDetectDoc()
{
	// TODO: 在此添加一次性构造代码
}

CGlassDetectDoc::~CGlassDetectDoc()
{
}

BOOL CGlassDetectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CGlassDetectDoc 序列化

void CGlassDetectDoc::Serialize(CArchive& ar)
{
	CString filePath;
	CFile* pFile = ar.GetFile();
	filePath = pFile->GetFilePath();
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
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
		AfxMessageBox(_T("打开文件错误"));
		return;
	}

	/* 获取文件大小 */
	fseek(pFile, 0, SEEK_END);
	int lSize = ftell(pFile);
	rewind(pFile);

	/* 分配内存存储整个文件 */
	char* buffer = (char*)malloc(sizeof(char)*lSize);
	memset(buffer, 0, sizeof(char)*lSize);
	if (buffer == NULL) return;

	/* 将文件拷贝到buffer中 */
	int result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		AfxMessageBox(_T("读取文件错误"));
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

	/* 结束演示，关闭文件并释放内存 */
	fclose(pFile);
	free(buffer);
}


#ifdef SHARED_HANDLERS

// 缩略图的支持
void CGlassDetectDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
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

// 搜索处理程序的支持
void CGlassDetectDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
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

// CGlassDetectDoc 诊断

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


// CGlassDetectDoc 命令


void CGlassDetectDoc::OnCloseDocument()
{
	// TODO: 在此添加专用代码和/或调用基类
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
