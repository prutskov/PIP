#include "stdafx.h"
#include "ImageViewer.h"


ImageViewer::ImageViewer() : _colorData(nullptr)
{
}


ImageViewer::~ImageViewer()
{
	wglMakeCurrent(_pDC->GetSafeHdc(), 0);
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
		const int width = static_cast<int>(_framePtr.nCols);
		const int height = static_cast<int>(_framePtr.nRows);

		const int widthRect = _rect.right;
		const int heightRect = _rect.bottom;

		float xW = static_cast<float>(widthRect) / width;
		float xH = static_cast<float>(heightRect) / height;

		if (xH < xW)
		{
			glPixelZoom(xH, xH);
			glRasterPos2f(-1, -1);
		}
		else
		{
			glPixelZoom(xW, xW);
			glRasterPos2f(-1, -1);
		}
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

	const int width = static_cast<int>(_framePtr.nCols);
	const int height = static_cast<int>(_framePtr.nRows);

	_colorData = std::shared_ptr<float[]>(new float[width*height * 3], std::default_delete<float[]>());

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < 3 * width; col += 3)
		{
			_colorData[3 * width*row + col + 0] = rPtr[(height - row)*width - col / 3] / 255.f;
			_colorData[3 * width*row + col + 1] = gPtr[(height - row)*width - col / 3] / 255.f;
			_colorData[3 * width*row + col + 2] = bPtr[(height - row)*width - col / 3] / 255.f;
		}
	}
}

void ImageViewer::initializeOGL(CRect & rt, CDC* pdc)
{
	_rect = rt;
	_pDC = pdc;
	HGLRC hrc;
	if (!bSetupPixelFormat())
		return;

	hrc = wglCreateContext(_pDC->GetSafeHdc());
	ASSERT(hrc != NULL);

	wglMakeCurrent(_pDC->GetSafeHdc(), hrc);

	glViewport(0, 0, _rect.right, _rect.bottom);
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
	if ((pixelformat = ChoosePixelFormat(_pDC->GetSafeHdc(), &pfd)) == 0)
	{
		return FALSE;
	}

	if (SetPixelFormat(_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}
