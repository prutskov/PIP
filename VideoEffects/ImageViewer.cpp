#include "stdafx.h"
#include "ImageViewer.h"


ImageViewer::ImageViewer() : _colorData(nullptr)
{
}


ImageViewer::~ImageViewer()
{
	wglMakeCurrent(pDC->GetSafeHdc(), 0);
	wglDeleteContext(wglGetCurrentContext());
}

void ImageViewer::show()
{

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glClear(GL_COLOR_BUFFER_BIT);

	if (_colorData != nullptr)
	{
	const int width = _framePtr.nCols;
	const int height = _framePtr.nRows;
		glRasterPos2i(0, 0);
		//glClearColor(0.5, 0.5, 0.0, 1.0);
		//Фстановить положение начала вывода битового массива
		//Этобразить пикселы на экране
		//glDrawPixels(width, height, GL_LUMINANCE, GL_SHORT, _colorData);
		glDrawPixels(width, height, GL_RGB, GL_FLOAT, _colorData.get());
	}
	glFlush();
	SwapBuffers(wglGetCurrentDC());
}

void ImageViewer::setFrame(utils::Frame frame)
{
	_framePtr = frame;
	float* rPtr = _framePtr.dataRPtr.get();
	float* gPtr = _framePtr.dataGPtr.get();
	float* bPtr = _framePtr.dataBPtr.get();

	const int width = _framePtr.nCols;
	const int height = _framePtr.nRows;

	_colorData = std::shared_ptr<float[]>(new float[width*height * 3], std::default_delete<float[]>());

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < 3 * width; col += 3)
		{
			_colorData[3 * width*row + col + 0] = rPtr[row*width + col / 3] / 255.f;
			_colorData[3 * width*row + col + 1] = gPtr[row*width + col / 3] / 255.f;
			_colorData[3 * width*row + col + 2] = bPtr[row*width + col / 3] / 255.f;
		}
	}
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