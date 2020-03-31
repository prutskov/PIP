#pragma once
#include "parameters.h"
#include "benchmark.h"
#include "enums.h"


// Диалоговое окно CBenchmark

class CBenchmark : public CDialogEx
{
	DECLARE_DYNAMIC(CBenchmark)

public:
	CBenchmark(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CBenchmark();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BENCHMARK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	size_t nIter;
	Parameters params;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRunBenchmark();
	BOOL _isMedian;
	BOOL _isGaussian;
	BOOL _isSharpness;
	BOOL _isSobel;
	BOOL _isMorphology;

	std::vector<Algorithm> benchAlgs;

	HANDLE hThreadBench;
	DWORD pdwThreadBench;
};

DWORD WINAPI benchThread(PVOID param);