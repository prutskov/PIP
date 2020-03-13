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
	void setFrame(utils::Frame frame, utils::Frame frameOrig);
	void initializeOGL(CRect & rt, CDC* pdc);

private:
	CRect _rect;
	CDC* _pDC;
	utils::Frame _framePtr;
	utils::Frame _frameOriginalPtr;
	std::shared_ptr<float[]> _colorData;
	BOOL bSetupPixelFormat();
};

