
// VideoEffectsDlg.h: файл заголовка
//

#pragma once
#include "algorithms.h"
#include "utils.h"
#include "CVManager.h"
#include "ImageViewer.h"


// Диалоговое окно CVideoEffectsDlg
class CVideoEffectsDlg : public CDialogEx
{
// Создание
public:
	CVideoEffectsDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEOEFFECTS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedOpen();

public:
	void loadImage();

private:
	CVManager* cvManager;
	ImageViewer _imgViewer;
};
