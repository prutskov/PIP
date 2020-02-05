#pragma once
#include <gl/GLU.h>
#include <gl/GL.h>
#include <memory>
#include "glut.h"
#include "frame.h"

class ImageViewer
{
public:
	ImageViewer();
	~ImageViewer();

	void show();
	void setFrame(utils::Frame *frame);
	void initializeOGL(CRect & rt, CDC* pdc);

private:
	CRect rect;
	CDC* pDC;
	utils::Frame* _framePtr;
	BOOL bSetupPixelFormat();
};

