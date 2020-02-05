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

void ImageViewer::show(const utils::Frame &frame)
{
	//double width = points[0][0].size() / 2;
	glClearColor(0.5f, 0.5f, 1.f, 0.5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-width * 2.0, width*2.0, -width * 2.0, width*2.0, -width * 2.0, width*2.0);

	glTranslated(0.f, 0.f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();/*
	glRotated(*angleX, 1, 0, 0);
	glRotated(*angleY, 0, 1, 0);
	glScaled(*zoom, *zoom, *zoom);*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);//
	glBegin(GL_QUADS);

	glEnd();
	/*glBegin(GL_LINES);
	glColor3d(1.0, 0, 0);
	glVertex3d(1,0,1)
	glEnd();*/
	glFinish();
	SwapBuffers(wglGetCurrentDC());
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
	glClearColor(0.0, 0.0, 0.0, 1.0);
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