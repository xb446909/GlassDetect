// DialogReadProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "GlassDetect.h"
#include "DialogReadProgress.h"
#include "afxdialogex.h"


// CDialogReadProgress 对话框

IMPLEMENT_DYNAMIC(CDialogReadProgress, CDialogEx)

CDialogReadProgress::CDialogReadProgress(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_READ_PROGRESS, pParent)
{

}

CDialogReadProgress::~CDialogReadProgress()
{
}

void CDialogReadProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogReadProgress, CDialogEx)
END_MESSAGE_MAP()


// CDialogReadProgress 消息处理程序


void CDialogReadProgress::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}

void CDialogReadProgress::SetRange(int nLength)
{
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS))->SetRange32(0, nLength);
}

void CDialogReadProgress::SetCurrent(int nPos)
{
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS))->SetPos(nPos);
}
