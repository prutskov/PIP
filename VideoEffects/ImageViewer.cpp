#include "stdafx.h"
#include "ImageViewer.h"

ULONG_PTR token1;

ImageViewer::ImageViewer()
{
	Status s;
	GdiplusStartupInput input;
	s = GdiplusStartup(&token1, &input, NULL);
	if (s != Ok)
	{
		MessageBox(L"ERROR!!!", L"Error GDI+ sturtup", MB_ICONERROR);
	}
}


ImageViewer::~ImageViewer()
{
	GdiplusShutdown(token1);
}


void ImageViewer::DrawItem(LPDRAWITEMSTRUCT RECT)
{
	Graphics gr(RECT->hDC);
	if (!_frame.isNull())
	{
		const int width = static_cast<int>(_frame.nCols);
		const int height = static_cast<int>(_frame.nRows);

		Bitmap bmpBuffer(width, height);
		Rect rectTmp(0, 0, width, height);
		BitmapData bmpData;

		bmpBuffer.LockBits(&rectTmp, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite,
			bmpBuffer.GetPixelFormat(), &bmpData);
		BYTE* curPixel = (BYTE*)bmpData.Scan0;
		int stride = bmpData.Stride;

#pragma omp parallel for
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < stride; j+=4)
			{
				curPixel[j + i * stride] = static_cast<BYTE>(*(_frame.dataBPtr.get() + i * width + j / 4));
				curPixel[j + i * stride + 1] = static_cast<BYTE>(*(_frame.dataGPtr.get() + i * width + j / 4));
				curPixel[j + i * stride + 2] = static_cast<BYTE>(*(_frame.dataRPtr.get() + i * width + j / 4));
				curPixel[j + i * stride + 3] = BYTE(255);
			}
		}

		bmpBuffer.UnlockBits(&bmpData);
		Rect rect(0, 0, RECT->rcItem.right, RECT->rcItem.bottom);
		gr.DrawImage(&bmpBuffer, rect);
	}
}

void ImageViewer::setFrame(utils::Frame frame)
{
	_frame = frame;
}
