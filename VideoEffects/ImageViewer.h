#pragma once
#include "afxwin.h"
#include <gdiplus.h>
#include "frame.h"

using namespace Gdiplus;

class ImageViewer : public CStatic
{
public:
	ImageViewer();
	~ImageViewer();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void setFrame(utils::Frame frame);

private:
	utils::Frame _frame;
};

