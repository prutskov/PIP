
// VideoEffectsDlg.cpp: файл реализации
//

#include <CL/cl.hpp>
#include "stdafx.h"
#include "VideoEffects.h"
#include "VideoEffectsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CVideoEffectsDlg



CVideoEffectsDlg::CVideoEffectsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIDEOEFFECTS_DIALOG, pParent)
	, _accType(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoEffectsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEW_PORT, _imgViewer);
	DDX_Radio(pDX, IDC_RADIO_CL, _accType);
	DDX_Control(pDX, IDC_COMBO_DEV, _deviceNames);
}

BEGIN_MESSAGE_MAP(CVideoEffectsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_APPLY, &CVideoEffectsDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDC_OPEN_IMAGE, &CVideoEffectsDlg::OnBnClickedOpen)
END_MESSAGE_MAP()


// Обработчики сообщений CVideoEffectsDlg

BOOL CVideoEffectsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	srand(static_cast<uint>(time(NULL)));


	cvManager = new CVManager();
	getAvailableDevices();

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CVideoEffectsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CVideoEffectsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVideoEffectsDlg::OnBnClickedApply()
{
	UpdateData(TRUE);

	if (cvManager->isNullImage())
	{
		MessageBox(L"Please, load image.", L"Warning", MB_ICONINFORMATION);
		return;
	}

	std::shared_ptr<algorithms::Algorithm> algorithm;
	algorithm = std::shared_ptr<algorithms::median_filter::Algorithm>(new algorithms::median_filter::Algorithm());
	algorithms::ParameterIface *parameter = new algorithms::median_filter::Parameter();

	algorithm->setParameter(parameter);
	algorithm->setFrame(cvManager->getImage());


	algorithm->generateNoise(30 / 100.0F);

	_imgViewer.setFrame(algorithm->getFrame());
	_imgViewer.RedrawWindow();
	float duration = algorithm->compute();

	_imgViewer.setFrame(algorithm->getFrame());
	_imgViewer.RedrawWindow();

}


void CVideoEffectsDlg::OnBnClickedOpen()
{
	loadImage();
	Frame image = cvManager->getImage();
	_imgViewer.setFrame(image);
	_imgViewer.RedrawWindow();
}

void CVideoEffectsDlg::getAvailableDevices()
{
	/*Get platfroms*/
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	std::vector<cl::Device> devices;

	/*Get all devices*/
	for (cl::Platform plat : platforms)
	{
		std::vector<cl::Device> device;
		plat.getDevices(CL_DEVICE_TYPE_ALL, &device);
		devices.insert(devices.end(), device.begin(), device.end());
	}

	std::vector<std::string> deviceNames(devices.size());

	for (size_t i = 0; i < devices.size(); i++)
	{
		deviceNames[i] = devices[i].getInfo<CL_DEVICE_NAME>();
	}

	for (int i = 0; i < deviceNames.size(); i++)
	{
		CString str(deviceNames[i].c_str());
		_deviceNames.AddString(str);
	}

	_deviceNames.SetCurSel(0);
}

void CVideoEffectsDlg::loadImage()
{
	CFileDialog fd(true, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY |
		OFN_LONGNAMES | OFN_PATHMUSTEXIST, _T("All Files (*.*)|*.*| Bitmap files (*.bmp)|*.bmp| JPEG files (*.jpg)|*.jpg| |"), NULL, 0, TRUE);

	if (fd.DoModal() != IDOK)
	{
		MessageBox(L"File is not open!", L"Warning", MB_ICONWARNING);
	}
	else
	{
		CString pathBMP = fd.GetPathName();
		CT2CA pathBuf(pathBMP);
		std::string str(pathBuf);
		cvManager->loadImage(str, cv::IMREAD_COLOR);
	}
}

