#pragma once
#include "parameters.h"

// Диалоговое окно CParameterDlg

class CParameterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParameterDlg)

public:
	CParameterDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CParameterDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAMETERS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	Parameters params;
	BOOL _medianFilterMask;
	afx_msg void OnBnClickedBtnApply();
	virtual BOOL OnInitDialog();
private:
	void setParameters();
};
