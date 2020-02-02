#include "stdafx.h"
#include "stdafx.h"
#include "Algorithm.h"

namespace algorithms
{

	Algorithm::Algorithm()
	{
	}


	Algorithm::~Algorithm()
	{
	}

	Frame Algorithm::getFrame()
	{
		return _frame;
	}

	void Algorithm::setFrame(const Frame & frame)
	{
		_frame = frame;
	}

	void Algorithm::setParameter(ParameterIface parameter)
	{
		_parameter = parameter;
	}

	void Algorithm::generateNoise(float percent)
	{
		const int nRows = _frame.nRows;
		const int nCols = _frame.nCols;

		const int nNoisedPixels = static_cast<int>(nRows*nCols*percent);
		int k = 0;
		while (k <= nNoisedPixels)
		{
			const int i = rand() % nRows;
			const int j = rand() % nCols;

			_frame.dataRPtr[i*nCols + j] = static_cast<float>(rand() % 2 * 255);
			_frame.dataBPtr[i*nCols + j] = static_cast<float>(rand() % 2 * 255);
			_frame.dataGPtr[i*nCols + j] = static_cast<float>(rand() % 2 * 255);
			k++;
		}
	}

	float Algorithm::compute()
	{
		return 0.0f;
	}

	std::vector<std::string> Algorithm::getDevices()
	{
		std::vector<std::string> devices = { "default" };
		return devices;
	}

}
