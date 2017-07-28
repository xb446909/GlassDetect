// DialogCameraSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "GlassDetect.h"
#include "DialogCameraSetting.h"
#include "afxdialogex.h"
#include "OpenGL.h"
#include "MainFrm.h"

// CDialogCameraSetting 对话框

IMPLEMENT_DYNAMIC(CDialogCameraSetting, CDialogEx)

CDialogCameraSetting::CDialogCameraSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CAMEARA_SETTING, pParent)
{

}

CDialogCameraSetting::~CDialogCameraSetting()
{
}

void CDialogCameraSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogCameraSetting, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_RX, &CDialogCameraSetting::OnEnChangeEditRx)
	ON_EN_CHANGE(IDC_EDIT_RY, &CDialogCameraSetting::OnEnChangeEditRy)
	ON_EN_CHANGE(IDC_EDIT_RZ, &CDialogCameraSetting::OnEnChangeEditRz)
	ON_EN_CHANGE(IDC_EDIT_CENT_X, &CDialogCameraSetting::OnEnChangeEditCentX)
	ON_EN_CHANGE(IDC_EDIT_CENT_Y, &CDialogCameraSetting::OnEnChangeEditCentY)
	ON_EN_CHANGE(IDC_EDIT_CENT_Z, &CDialogCameraSetting::OnEnChangeEditCentZ)
	ON_EN_CHANGE(IDC_EDIT_CAM_X, &CDialogCameraSetting::OnEnChangeEditCamX)
	ON_EN_CHANGE(IDC_EDIT_CAM_Y, &CDialogCameraSetting::OnEnChangeEditCamY)
	ON_EN_CHANGE(IDC_EDIT_CAM_Z, &CDialogCameraSetting::OnEnChangeEditCamZ)
END_MESSAGE_MAP()


// CDialogCameraSetting 消息处理程序


void CDialogCameraSetting::SetPivotPoint(float x, float y, float z)
{
	CString str;
	str.Format(_T("%f"), x);
	SetDlgItemText(IDC_EDIT_CENT_X, str);
	str.Format(_T("%f"), y);
	SetDlgItemText(IDC_EDIT_CENT_Y, str);
	str.Format(_T("%f"), z);
	SetDlgItemText(IDC_EDIT_CENT_Z, str);
}

void CDialogCameraSetting::SetCameraCenter(float x, float y, float z)
{
	CString str;
	str.Format(_T("%f"), x);
	SetDlgItemText(IDC_EDIT_CAM_X, str);
	str.Format(_T("%f"), y);
	SetDlgItemText(IDC_EDIT_CAM_Y, str);
	str.Format(_T("%f"), z);
	SetDlgItemText(IDC_EDIT_CAM_Z, str);
}

BOOL CDialogCameraSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CSliderCtrl* pSliderX = reinterpret_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_RX));
	CSliderCtrl* pSliderY = reinterpret_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_RY));
	CSliderCtrl* pSliderZ = reinterpret_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_RZ));

	pSliderX->SetPos(50);
	pSliderY->SetPos(50);
	pSliderZ->SetPos(50);

	pSliderX->SetRange(-1800, 1800);
	pSliderY->SetRange(-1800, 1800);
	pSliderZ->SetRange(-1800, 1800);

	pSliderX->SetLineSize(10);
	pSliderY->SetLineSize(10);
	pSliderZ->SetLineSize(10);

	pSliderX->SetPageSize(100);
	pSliderY->SetPageSize(100);
	pSliderZ->SetPageSize(100);

	SetDlgItemText(IDC_EDIT_RX, _T("0.0"));
	SetDlgItemText(IDC_EDIT_RY, _T("0.0"));
	SetDlgItemText(IDC_EDIT_RZ, _T("0.0"));

	SetDlgItemText(IDC_EDIT_CENT_X, _T("0.0"));
	SetDlgItemText(IDC_EDIT_CENT_Y, _T("0.0"));
	SetDlgItemText(IDC_EDIT_CENT_Z, _T("0.0"));

	SetDlgItemText(IDC_EDIT_CAM_X, _T("0.0"));
	SetDlgItemText(IDC_EDIT_CAM_Y, _T("0.0"));
	SetDlgItemText(IDC_EDIT_CAM_Z, _T("0.0"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDialogCameraSetting::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	CSliderCtrl* pSliderX = reinterpret_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_RX));
	CSliderCtrl* pSliderY = reinterpret_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_RY));
	CSliderCtrl* pSliderZ = reinterpret_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_RZ));

	CString str;
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
	str.Format(_T("%.1f"), (float)pSlider->GetPos() / 10.0f);

	if ((CSliderCtrl*)pScrollBar == pSliderX)
		SetDlgItemText(IDC_EDIT_RX, str);

	if ((CSliderCtrl*)pScrollBar == pSliderY)
		SetDlgItemText(IDC_EDIT_RY, str);

	if ((CSliderCtrl*)pScrollBar == pSliderZ)
		SetDlgItemText(IDC_EDIT_RZ, str);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDialogCameraSetting::OnEnChangeEditRx()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CEdit* pEdit = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_RX));
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_RX));
	CString str;
	pEdit->GetWindowText(str);
	double deg = atof(CT2A(str));
	pSlider->SetPos((int)deg * 10);
	SetViewModel();
}


void CDialogCameraSetting::OnEnChangeEditRy()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CEdit* pEdit = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_RY));
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_RY));
	CString str;
	pEdit->GetWindowText(str);
	double deg = atof(CT2A(str));
	pSlider->SetPos((int)deg * 10);
	SetViewModel();
}


void CDialogCameraSetting::OnEnChangeEditRz()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CEdit* pEdit = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_RZ));
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_RZ));
	CString str;
	pEdit->GetWindowText(str);
	double deg = atof(CT2A(str));
	pSlider->SetPos((int)deg * 10);
	SetViewModel();
}

void CDialogCameraSetting::SetViewModel()
{
	CSliderCtrl* pSliderX = reinterpret_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_RX));
	CSliderCtrl* pSliderY = reinterpret_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_RY));
	CSliderCtrl* pSliderZ = reinterpret_cast<CSliderCtrl*>(GetDlgItem(IDC_SLIDER_RZ));

	CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(GetParent());
	COpenGL* pView = reinterpret_cast<COpenGL*>(pMainFrame->GetActiveView());
	float x = (float)pSliderX->GetPos() / 10.0;
	float y = (float)pSliderY->GetPos() / 10.0;
	float z = (float)pSliderZ->GetPos() / 10.0;

	float phi_rad = pi<float>() * x / 180.0f;
	float theta_rad = pi<float>() * y / 180.0f;
	float psi_rad = pi<float>() * z / 180.0f;
	vec3 t3D = vec3(0.0f, 0.0f, 0.0f);
	mat4 mat = eulerAngleXYZ(phi_rad, theta_rad, psi_rad);
	//pView->setBaseViewMat(mat);
	pView->RenderScene();
}

void CDialogCameraSetting::SetPivotPoint()
{
	CString strX, strY, strZ;
	GetDlgItemText(IDC_EDIT_CENT_X, strX);
	GetDlgItemText(IDC_EDIT_CENT_Y, strY);
	GetDlgItemText(IDC_EDIT_CENT_Z, strZ);
	vec3 P(atof(CT2A(strX)), atof(CT2A(strY)), atof(CT2A(strZ)));
	CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(GetParent());
	COpenGL* pView = reinterpret_cast<COpenGL*>(pMainFrame->GetActiveView());
	pView->setPivotPoint(P);
}

void CDialogCameraSetting::SetCameraPos()
{
	CString strX, strY, strZ;
	GetDlgItemText(IDC_EDIT_CAM_X, strX);
	GetDlgItemText(IDC_EDIT_CAM_Y, strY);
	GetDlgItemText(IDC_EDIT_CAM_Z, strZ);
	vec3 P(atof(CT2A(strX)), atof(CT2A(strY)), atof(CT2A(strZ)));
	CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(GetParent());
	COpenGL* pView = reinterpret_cast<COpenGL*>(pMainFrame->GetActiveView());
	pView->setCameraPos(P);
}


void CDialogCameraSetting::OnEnChangeEditCentX()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	SetPivotPoint();
}


void CDialogCameraSetting::OnEnChangeEditCentY()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	SetPivotPoint();
}


void CDialogCameraSetting::OnEnChangeEditCentZ()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	SetPivotPoint();
}


void CDialogCameraSetting::OnEnChangeEditCamX()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	SetCameraPos();
}


void CDialogCameraSetting::OnEnChangeEditCamY()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	SetCameraPos();
}


void CDialogCameraSetting::OnEnChangeEditCamZ()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	SetCameraPos();
}
