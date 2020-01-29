#include "stdafx.h"
#include "Frame.h"

namespace utils
{

	Frame::Frame() : nRows(0), nCols(0)
	{
		dataRPtr = std::shared_ptr<float[]>(nullptr);
		dataGPtr = std::shared_ptr<float[]>(nullptr);
		dataBPtr = std::shared_ptr<float[]>(nullptr);
	}

	Frame::Frame(size_t rows, size_t cols, std::shared_ptr<float[]> dataR,
		std::shared_ptr<float[]> dataG, std::shared_ptr<float[]> dataB) : nRows(rows), nCols(cols)
	{
		dataRPtr = dataR;
		dataGPtr = dataG;
		dataBPtr = dataB;
	}

	Frame Frame::clone()
	{
		Frame frameCloned(nRows, nCols,
			std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()),
			std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()),
			std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()));

		frameCloned.nCols = nCols;
		frameCloned.nRows = nRows;
		std::memcpy(frameCloned.dataBPtr.get(), dataBPtr.get(), nRows*nCols * sizeof(float));
		std::memcpy(frameCloned.dataGPtr.get(), dataGPtr.get(), nRows*nCols * sizeof(float));
		std::memcpy(frameCloned.dataRPtr.get(), dataRPtr.get(), nRows*nCols * sizeof(float));

		return frameCloned;
	}

}	//	namespace utils