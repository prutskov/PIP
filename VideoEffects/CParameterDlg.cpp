// CParameterDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "VideoEffects.h"
#include "CParameterDlg.h"
#include "afxdialogex.h"
#include "median_filter/parameter_median_filter.h"


// Диалоговое окно CParameterDlg

IMPLEMENT_DYNAMIC(CParameterDlg, CDialogEx)

CParameterDlg::CParameterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARAMETERS, pParent)
	, _medianFilterMask(FALSE)
	, _gaussMaskSize(3)
	, _gaussSigma(1)
	, _k(1)
	, _morphType(FALSE)
	, _morphRows(4)
	, _morphCols(6)
{

}

CParameterDlg::~CParameterDlg()
{
}

void CParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_MF_3x3, _medianFilterMask);
	DDX_Text(pDX, IDC_GAUSS_SIZE, _gaussMaskSize);
	DDX_Text(pDX, IDC_GAUSS_SIGMA, _gaussSigma);
	DDX_Text(pDX, IDC_SHARPNESS_K, _k);
	DDX_Radio(pDX, IDC_EROSION, _morphType);
	DDX_Text(pDX, IDC_ROWS, _morphRows);
	DDX_Text(pDX, IDC_ROWS2, _morphCols);
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

	/**Gaussian filter parameters*/
	_gaussMaskSize = params.gaussFilterMask;
	_gaussSigma = params.sigma;

	/**Sharpness parameters*/
	_k = params.k;

	/**Morphology (erosion/dilatation) parameters*/
	_morphRows = params.morphRowsMask;
	_morphCols = params.morphColsMask;
	_morphType = params.morphType == algorithms::erosion::MorphType::erosion ? 0 : 1;
}

void CParameterDlg::OnBnClickedBtnApply()
{
	UpdateData(TRUE);

	/**Median filter parameters*/
	params.medianFilterMask = _medianFilterMask == 0 ? algorithms::median_filter::MASK3X3 : algorithms::median_filter::MASK5X5;

	/**Gaussian filter parameters*/
	params.gaussFilterMask = _gaussMaskSize;
	params.sigma = _gaussSigma;

	/**Sharpness parameters*/
	params.k = _k;

	/**Morphology (erosion/dilatation) parameters*/
	params.morphRowsMask = _morphRows;
	params.morphColsMask = _morphCols;
	params.morphType = _morphType == 0 ? algorithms::erosion::MorphType::erosion : algorithms::erosion::MorphType::dilation;
	CDialog::OnOK();
}


