
// VideoEffectsDlg.cpp: файл реализации
//

#include <CL/cl.hpp>
#include "stdafx.h"
#include "VideoEffects.h"
#include "VideoEffectsDlg.h"
#include "CParameterDlg.h"
#include "CBenchmark.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CVideoEffectsDlg
CRITICAL_SECTION cs;


CVideoEffectsDlg::CVideoEffectsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIDEOEFFECTS_DIALOG, pParent)
	, _accType(FALSE)
	, _offThread(false)
	, _algType(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoEffectsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_CL, _accType);
	DDX_Control(pDX, IDC_COMBO_DEV, _deviceNames);
	DDX_Control(pDX, IDC_IMG_SIZE, _ctrlImgSize);
	DDX_Radio(pDX, IDC_RADIO_NONE, _algType);
}

BEGIN_MESSAGE_MAP(CVideoEffectsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_APPLY, &CVideoEffectsDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDC_OPEN_IMAGE, &CVideoEffectsDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_OPEN_VIDEO, &CVideoEffectsDlg::OnBnClickedOpenVideo)
	ON_BN_CLICKED(IDC_OPEN_CAMERA, &CVideoEffectsDlg::OnBnClickedOpenCamera)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP, &CVideoEffectsDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BTN_PARAMETERS, &CVideoEffectsDlg::OnBnClickedBtnParameters)
	ON_BN_CLICKED(IDC_BENCH, &CVideoEffectsDlg::OnBnClickedBench)
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

	InitializeCriticalSection(&cs);

	CDC* dc = GetDlgItem(IDC_VIEW_PORT)->GetDC();
	GetDlgItem(IDC_VIEW_PORT)->GetClientRect(&_imgSurface);
	_imgViewer.initializeOGL(_imgSurface, dc);

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
		EnterCriticalSection(&cs);
		_imgViewer.show();
		LeaveCriticalSection(&cs);
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

	if (_isImage)
	{
		if (cvManager->isNullImage())
		{
			MessageBox(L"Please, load image.", L"Warning", MB_ICONINFORMATION);
			return;
		}
		imageFlow();
	}
	else
	{
		if (!_videoCapture.isOpened())
		{
			MessageBox(L"Video is not open!", L"Warning", MB_ICONWARNING);
			return;
		}
		_offThread = false;
		SetTimer(1, 30, NULL);
		hThreadCompute = CreateThread(
			NULL,		// дескриптор защиты
			0,			// начальный размер стека ( Если это значение нулевое, новый поток использует по умолчанию размер стека исполняемой программы)
			(LPTHREAD_START_ROUTINE)computeThread,	 // функция потока
			this,		// параметр потока 
			0,			//oпции создания(здесь можно отложить запуск выполнения потока. Для запуска потока сразу же, передаём 0.)
			&pdwThreadCalculate);// идентификатор потока (указатель на переменную, куда будет сохранён идентификатор потока)
	}
}


void CVideoEffectsDlg::OnBnClickedOpen()
{
	_isImage = true;
	_offThread = true;

	loadImage();
	Frame image = cvManager->getImage();

	const size_t width = image.nCols;
	const size_t height = image.nRows;

	CString str("Resolution: ");
	CString  arg1, arg2;
	arg1.Format(_T("%lu"), width);
	arg2.Format(_T("%lu"), height);
	str += arg1 + "x" + arg2;
	_ctrlImgSize.SetWindowTextW(str);

	_imgViewer.setFrame(image);
	_imgViewer.show();
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

	for (size_t i = 0; i < deviceNames.size(); i++)
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

std::string CVideoEffectsDlg::getVideoPath()
{
	CFileDialog fd(true, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY |
		OFN_LONGNAMES | OFN_PATHMUSTEXIST, _T("All Files (*.*)|*.*| AVI files (*.avi)|*.avi| MP4 files (*.mp4)|*.mp4| |"), NULL, 0, TRUE);

	std::string outPath = "";
	if (fd.DoModal() != IDOK)
	{
		MessageBox(L"File is not open!", L"Warning", MB_ICONWARNING);
	}
	else
	{
		CString path = fd.GetPathName();
		CT2CA pathBuf(path);
		std::string str(pathBuf);
		outPath = str;
	}
	return outPath;
}

void CVideoEffectsDlg::videoFlow(cv::VideoCapture & video)
{
	if (_algType != 0)
	{
		std::shared_ptr<algorithms::Algorithm> algorithm;
		setAlgParameters(algorithm);

		for (;;)
		{
			cv::Mat frame;
			video >> frame; // get a new frame from camera
			algorithm->setFrame(cvManager->convertToPtr(frame.clone()));
			//algorithm->generateNoise( 30/ 100.0F);
			algorithm->compute();
			EnterCriticalSection(&cs);
			_imgViewer.setFrame(algorithm->getFrame());
			LeaveCriticalSection(&cs);
			if (_offThread) break;
		}
	}
	else
	{
		for (;;)
		{
			cv::Mat frame;
			video >> frame; // get a new frame from camera
			EnterCriticalSection(&cs);
			_imgViewer.setFrame(cvManager->convertToPtr(frame.clone()));
			LeaveCriticalSection(&cs);
			if (_offThread) break;
		}
	}

}

void CVideoEffectsDlg::imageFlow()
{
	if (_algType != 0)
	{
		std::shared_ptr<algorithms::Algorithm> algorithm;
		setAlgParameters(algorithm);
		algorithm->setFrame(cvManager->getImage());

		//algorithm->generateNoise(30 / 100.0F);

		_imgViewer.setFrame(algorithm->getFrame());
		_imgViewer.show();
		algorithm->compute();

		_imgViewer.setFrame(algorithm->getFrame());
	}
	else
	{
		_imgViewer.setFrame(cvManager->getImage());
	}
	_imgViewer.show();
}

void CVideoEffectsDlg::setAlgParameters(std::shared_ptr<algorithms::Algorithm> & alg)
{
	algorithms::ParameterIface *parameters;
	const Accelerator accType = static_cast<Accelerator>(_accType);
	const Algorithm algType = static_cast<Algorithm>(_algType);

	if (accType == Accelerator::opencl)
	{
		switch (algType)
		{
		case Algorithm::median:
			alg = std::shared_ptr<algorithms::median_filter::opencl::Algorithm>(new algorithms::median_filter::opencl::Algorithm());
			parameters = new algorithms::median_filter::Parameter(params.medianFilterMask, _deviceNames.GetCurSel());
			break;
		case Algorithm::gauss:
			alg = std::shared_ptr<algorithms::gaussian_filter::opencl::Algorithm>(new algorithms::gaussian_filter::opencl::Algorithm());
			parameters = new algorithms::gaussian_filter::Parameter(params.gaussFilterMask, params.sigma, _deviceNames.GetCurSel());
			break;
		case Algorithm::sharpness:
			alg = std::shared_ptr<algorithms::sharpness::opencl::Algorithm>(new algorithms::sharpness::opencl::Algorithm());
			parameters = new algorithms::sharpness::Parameter(params.k, _deviceNames.GetCurSel());
			break;
		default:
			break;
		}

	}
	else if (accType == Accelerator::openmp)
	{
		switch (algType)
		{
		case Algorithm::median:
			alg = std::shared_ptr<algorithms::median_filter::openmp::Algorithm>(new algorithms::median_filter::openmp::Algorithm());
			parameters = new algorithms::median_filter::Parameter(params.medianFilterMask);
			break;
		case Algorithm::gauss:
			alg = std::shared_ptr<algorithms::gaussian_filter::openmp::Algorithm>(new algorithms::gaussian_filter::openmp::Algorithm());
			parameters = new algorithms::gaussian_filter::Parameter(params.gaussFilterMask, params.sigma);
			break;
		case Algorithm::sharpness:
			alg = std::shared_ptr<algorithms::sharpness::openmp::Algorithm>(new algorithms::sharpness::openmp::Algorithm());
			parameters = new algorithms::sharpness::Parameter(params.k);
			break;
		default:
			break;
		}
	}
	else if (accType == Accelerator::tbb)
	{
		switch (algType)
		{
		case Algorithm::median:
			alg = std::shared_ptr<algorithms::median_filter::tbb::Algorithm>(new algorithms::median_filter::tbb::Algorithm());
			parameters = new algorithms::median_filter::Parameter(params.medianFilterMask);
			break;
		case Algorithm::gauss:
			alg = std::shared_ptr<algorithms::gaussian_filter::tbb::Algorithm>(new algorithms::gaussian_filter::tbb::Algorithm());
			parameters = new algorithms::gaussian_filter::Parameter(params.gaussFilterMask, params.sigma);
			break;
		case Algorithm::sharpness:
			alg = std::shared_ptr<algorithms::sharpness::tbb::Algorithm>(new algorithms::sharpness::tbb::Algorithm());
			parameters = new algorithms::sharpness::Parameter(params.k);
			break;
		default:
			break;
		}
	}
	else
	{
		MessageBox(L"Accelerator was not found.", L"Warning", MB_ICONWARNING);
		return;
	}
	alg->setParameter(parameters);
}


void CVideoEffectsDlg::OnBnClickedOpenVideo()
{
	UpdateData(TRUE);
	_isImage = false;
	_offThread = true;
	_videoCapture = cv::VideoCapture(getVideoPath());
	if (!_videoCapture.isOpened())
	{
		MessageBox(L"Video is not open!", L"Warning", MB_ICONWARNING);
		return;
	}

	{
		cv::Mat frameMat;
		_videoCapture >> frameMat;

		Frame image = cvManager->convertToPtr(frameMat.clone());

		const size_t width = image.nCols;
		const size_t height = image.nRows;

		CString str("Resolution: ");
		CString  arg1, arg2;
		arg1.Format(_T("%lu"), width);
		arg2.Format(_T("%lu"), height);
		str += arg1 + "x" + arg2;
		_ctrlImgSize.SetWindowTextW(str);
	}
}


void CVideoEffectsDlg::OnBnClickedOpenCamera()
{
	UpdateData(TRUE);
	_isImage = false;
	_offThread = true;
	_videoCapture = cv::VideoCapture(0);

	if (!_videoCapture.isOpened())
	{
		MessageBox(L"Camera is not open!", L"Warning", MB_ICONWARNING);
		return;
	}

	{
		cv::Mat frameMat;
		_videoCapture >> frameMat;

		Frame image = cvManager->convertToPtr(frameMat.clone());

		const size_t width = image.nCols;
		const size_t height = image.nRows;

		CString str("Resolution: ");
		CString  arg1, arg2;
		arg1.Format(_T("%lu"), width);
		arg2.Format(_T("%lu"), height);
		str += arg1 + "x" + arg2;
		_ctrlImgSize.SetWindowTextW(str);
	}
}


DWORD WINAPI computeThread(PVOID param)
{
	CVideoEffectsDlg *dlg = (CVideoEffectsDlg*)param;
	dlg->videoFlow(dlg->_videoCapture);
	return 0;
}

void CVideoEffectsDlg::OnTimer(UINT_PTR nIDEvent)
{
	EnterCriticalSection(&cs);
	_imgViewer.show();
	LeaveCriticalSection(&cs);

	CDialogEx::OnTimer(nIDEvent);
}


void CVideoEffectsDlg::OnBnClickedStop()
{
	_offThread = true;
	KillTimer(1);
}


void CVideoEffectsDlg::OnBnClickedBtnParameters()
{
	UpdateData(TRUE);
	CParameterDlg parametersDlg;
	parametersDlg.params = params;
	parametersDlg.DoModal();
	params = parametersDlg.params;
	params.activeDevice = _deviceNames.GetCurSel();
}


void CVideoEffectsDlg::OnBnClickedBench()
{
	UpdateData(TRUE);
	CBenchmark benchDlg;

	params.activeDevice = _deviceNames.GetCurSel();
	benchDlg.params = params;
	benchDlg.DoModal();
}
