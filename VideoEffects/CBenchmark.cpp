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
{

}

CBenchmark::~CBenchmark()
{
}

void CBenchmark::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_N_ITER, nIter);
}


BEGIN_MESSAGE_MAP(CBenchmark, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_RUN_BENCHMARK, &CBenchmark::OnBnClickedBtnRunBenchmark)
END_MESSAGE_MAP()


// Обработчики сообщений CBenchmark


BOOL CBenchmark::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CBenchmark::OnBnClickedBtnRunBenchmark()
{
	UpdateData(TRUE);
	Benchmark bench(params, 5);
	bench.runBenchmark();
}
