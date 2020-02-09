#include "stdafx.h"
#include "ImageViewer.h"
#include <math.h>

ULONG_PTR token1;;
ImageViewer::ImageViewer()
{
	Status s;
	GdiplusStartupInput input;
	s = GdiplusStartup(&token1, &input, NULL);
	if (s != Ok)
	{
		MessageBox(L"ERROR!!!", L"Îøèáêà", MB_ICONERROR);
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
		const size_t width = _frame.nCols;
		const size_t height = _frame.nRows;

		Bitmap bmpBuffer(width, height);
		Rect rectTmp(0, 0, width, height);
		BitmapData bmpData;

		bmpBuffer.LockBits(&rectTmp, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite,
			bmpBuffer.GetPixelFormat(), &bmpData);
		BYTE* curPixel = (BYTE*)bmpData.Scan0;
		int stride = bmpData.Stride;

		/*for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Color color;
				color = Color::MakeARGB(255, *(_frame.dataRPtr.get() + i * width + j),
											*(_frame.dataGPtr.get() + i * width + j),
												*(_frame.dataBPtr.get() + i * width + j));
				bmpBuffer.SetPixel(j, i, color);
			}
		}*/

#pragma omp parallel for
		for (int i = 0; i < height; i++)
		{
			int col = 0;
			for (int j = 0; j < stride; j+=4)
			{
				curPixel[j + i * stride] = *(_frame.dataBPtr.get() + i * width + j / 4);
				curPixel[j + i * stride + 1] = *(_frame.dataGPtr.get() + i * width + j / 4);
				curPixel[j + i * stride + 2] = *(_frame.dataRPtr.get() + i * width + j / 4);
				curPixel[j + i * stride + 3] = 255; 

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
