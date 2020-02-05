#pragma once
#include <gl/GLU.h>
#include <gl/GL.h>
#include "glut.h"
#include "frame.h"


class ImageViewer
{
public:
	ImageViewer();
	~ImageViewer();

	double *angleX, *angleY, *zoom;
	void show(const utils::Frame &frame);

	void initializeOGL(CRect & rt, CDC* pdc);
	CRect rect;
	CDC* pDC;

	BOOL bSetupPixelFormat();
};

