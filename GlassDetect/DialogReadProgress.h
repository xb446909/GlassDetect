#pragma once
#include "afxcmn.h"


// CDialogReadProgress �Ի���

class CDialogReadProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogReadProgress)

public:
	CDialogReadProgress(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogReadProgress();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_READ_PROGRESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	void SetRange(int nLength);
	void SetCurrent(int nPos);
};
