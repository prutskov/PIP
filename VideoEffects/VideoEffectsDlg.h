
// VideoEffectsDlg.h: файл заголовка
//

#pragma once
#include "algorithms.h"
#include "utils.h"
#include "CVManager.h"
#include "ImageViewer.h"
#include "enums.h"
#include "parameters.h"


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
	void getAvailableDevices();

public:
	void loadImage();
	std::string getVideoPath();
	void videoFlow(cv::VideoCapture & video);
	void imageFlow();
	cv::VideoCapture _videoCapture;
	void setAlgParameters(std::shared_ptr<algorithms::Algorithm>& alg);

private:
	CVManager* cvManager;
	ImageViewer _imgViewer;
	CRect _imgSurface;

	BOOL _accType;
	BOOL _algType;
	CComboBox _deviceNames;

	HANDLE hThreadCompute;
	DWORD pdwThreadCalculate;
	bool _offThread;
	bool _isImage;
	bool _isNoiseAdd;
	size_t _percentNoise;
	Parameters params;
public:
	afx_msg void OnBnClickedOpenVideo();
	afx_msg void OnBnClickedOpenCamera();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic _ctrlImgSize;
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedBtnParameters();
	afx_msg void OnBnClickedBench();
	CStatic _FPS;
};

DWORD WINAPI computeThread(PVOID param);

