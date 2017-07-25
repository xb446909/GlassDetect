// DialogReadProgress.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GlassDetect.h"
#include "DialogReadProgress.h"
#include "afxdialogex.h"


// CDialogReadProgress �Ի���

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


// CDialogReadProgress ��Ϣ�������


void CDialogReadProgress::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

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
