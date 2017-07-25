#pragma once
#include "afxcmn.h"


// CDialogReadProgress 对话框

class CDialogReadProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogReadProgress)

public:
	CDialogReadProgress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogReadProgress();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_READ_PROGRESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	void SetRange(int nLength);
	void SetCurrent(int nPos);
};
