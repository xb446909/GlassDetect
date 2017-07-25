#pragma once


// CDialogCameraSetting 对话框

class CDialogCameraSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogCameraSetting)

public:
	CDialogCameraSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogCameraSetting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAMEARA_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetPivotPoint(float x, float y, float z);
	void SetCameraCenter(float x, float y, float z);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditRx();
	afx_msg void OnEnChangeEditRy();
	afx_msg void OnEnChangeEditRz();
	void SetViewModel();
	void SetPivotPoint();
	void SetCameraPos();
	afx_msg void OnEnChangeEditCentX();
	afx_msg void OnEnChangeEditCentY();
	afx_msg void OnEnChangeEditCentZ();
	afx_msg void OnEnChangeEditCamX();
	afx_msg void OnEnChangeEditCamY();
	afx_msg void OnEnChangeEditCamZ();
};
