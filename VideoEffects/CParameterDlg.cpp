// CParameterDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "VideoEffects.h"
#include "CParameterDlg.h"
#include "afxdialogex.h"
#include "median_filter/parameter.h"


// Диалоговое окно CParameterDlg

IMPLEMENT_DYNAMIC(CParameterDlg, CDialogEx)

CParameterDlg::CParameterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARAMETERS, pParent)
	, _medianFilterMask(FALSE)
{

}

CParameterDlg::~CParameterDlg()
{
}

void CParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_MF_3x3, _medianFilterMask);
}


BEGIN_MESSAGE_MAP(CParameterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CParameterDlg::OnBnClickedBtnApply)
END_MESSAGE_MAP()


// Обработчики сообщений CParameterDlg

BOOL CParameterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	setParameters();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}

void CParameterDlg::setParameters()
{
	/**Median filter parameters*/
	_medianFilterMask = params.medianFilterMask == algorithms::median_filter::MASK3X3 ? 0 : 1;
}

void CParameterDlg::OnBnClickedBtnApply()
{
	UpdateData(TRUE);

	/**Median filter parameters*/
	params.medianFilterMask = _medianFilterMask == 0 ? algorithms::median_filter::MASK3X3 : algorithms::median_filter::MASK5X5;
	CDialog::OnOK();
}


