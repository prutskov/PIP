#include "stdafx.h"
#include "ImageViewer.h"


ImageViewer::ImageViewer()
{
}


ImageViewer::~ImageViewer()
{
	wglMakeCurrent(pDC->GetSafeHdc(), 0);
	wglDeleteContext(wglGetCurrentContext());
}

void ImageViewer::show()
{

	float* rPtr = _framePtr->dataRPtr.get();
	float* gPtr = _framePtr->dataGPtr.get();
	float* bPtr = _framePtr->dataBPtr.get();
	
	const int width  = _framePtr->nCols;
	const int height = _framePtr->nRows;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-width / 2., width / 2., -height / 2., height / 2.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.5, 0.5, 0.0, 1.0);
	//Установить положение начала вывода битового массива
	glRasterPos2i(-1, 1);
	glPixelZoom(1, -1);
	//Задать атрибуты вывода пикселов
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, width); //длина строки
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0); // сколько строк пропустить?
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0); // сколько пикселов пропустить в каждой строке?
	//Отобразить пикселы на экране
	glDrawPixels(width, height, GL_LUMINANCE, GL_SHORT, rPtr);
	/*glDrawPixels(width, height, GL_GREEN, GL_FLOAT, gPtr);
	glDrawPixels(width, height, GL_BLUE, GL_FLOAT, bPtr);*/
	glFlush();

	//glutSwapBuffers();
	SwapBuffers(wglGetCurrentDC());
}

void ImageViewer::setFrame(utils::Frame *frame)
{
	_framePtr = frame;
}

void ImageViewer::initializeOGL(CRect & rt, CDC* pdc)
{
	rect = rt;
	pDC = pdc;
	HGLRC hrc;
	if (!bSetupPixelFormat())
		return;

	hrc = wglCreateContext(pDC->GetSafeHdc());
	ASSERT(hrc != NULL);

	wglMakeCurrent(pDC->GetSafeHdc(), hrc);

	glViewport(0, 0, rect.right, rect.bottom);

	glCullFace(GL_FRONT);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.5, 0.0, 0.0, 1.0);
}

BOOL ImageViewer::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |			// support OpenGL
		PFD_DOUBLEBUFFER,				// double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;
	if ((pixelformat = ChoosePixelFormat(pDC->GetSafeHdc(), &pfd)) == 0)
	{
		return FALSE;
	}

	if (SetPixelFormat(pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}