// CBenchmark.cpp: файл реализации
//

#include "stdafx.h"
#include "VideoEffects.h"
#include "CBenchmark.h"
#include "afxdialogex.h"


// Диалоговое окно CBenchmark

IMPLEMENT_DYNAMIC(CBenchmark, CDialogEx)

CBenchmark::CBenchmark(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BENCHMARK, pParent)
	, nIter(5)
	, _isMedian(TRUE)
	, _isGaussian(TRUE)
	, _isSharpness(TRUE)
	, _isSobel(TRUE)
	, _isMorphology(TRUE)
{

}

CBenchmark::~CBenchmark()
{
}

void CBenchmark::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_N_ITER, nIter);
	DDX_Check(pDX, IDC_CHECK1, _isMedian);
	DDX_Check(pDX, IDC_CHECK2, _isGaussian);
	DDX_Check(pDX, IDC_CHECK3, _isSharpness);
	DDX_Check(pDX, IDC_CHECK4, _isSobel);
	DDX_Check(pDX, IDC_CHECK5, _isMorphology);
	DDX_Control(pDX, IDC_PROGRESS1, progressCtrl);
}


BEGIN_MESSAGE_MAP(CBenchmark, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_RUN_BENCHMARK, &CBenchmark::OnBnClickedBtnRunBenchmark)
END_MESSAGE_MAP()


// Обработчики сообщений CBenchmark


BOOL CBenchmark::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	progressCtrl.SetRange(0, 100);
	progressCtrl.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CBenchmark::OnBnClickedBtnRunBenchmark()
{
	UpdateData(TRUE);
	progressCtrl.SetPos(0);

	lockIntarface(true);
	benchAlgs.clear();
	if (_isMedian) benchAlgs.push_back(Algorithm::median);
	if (_isGaussian) benchAlgs.push_back(Algorithm::gauss);
	if (_isSharpness) benchAlgs.push_back(Algorithm::sharpness);
	if (_isSobel) benchAlgs.push_back(Algorithm::sobel);
	if (_isMorphology) benchAlgs.push_back(Algorithm::erosion);

	hThreadBench = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)benchThread,
		this,
		0,
		&pdwThreadBench);
}

void CBenchmark::lockIntarface(bool isLock)
{
	auto btnRun = GetDlgItem(IDC_BTN_RUN_BENCHMARK);
	btnRun->EnableWindow(!isLock);
}

DWORD WINAPI benchThread(PVOID param)
{
	CBenchmark *dlg = (CBenchmark*)param;
	Benchmark bench(dlg->params, dlg->nIter);

	int nAlgs = static_cast<int>(dlg->benchAlgs.size());

	for (int i = 0; i < nAlgs; ++i)
	{
		bench.runAlgorithm(dlg->benchAlgs[i]);
		int position = (i + 1) * 100;
		dlg->progressCtrl.SetPos(position/nAlgs);
	}
	dlg->lockIntarface(false);
	return 0;
}
