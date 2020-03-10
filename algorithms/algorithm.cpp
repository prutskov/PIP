#include "stdafx.h"
#include <chrono>

#include "algorithm.h"

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

	void Algorithm::setParameter(ParameterIface *parameter)
	{
		_parameter = parameter;
	}

	void Algorithm::generateNoise(float percent)
	{
		const int nRows = static_cast<int>(_frame.nRows);
		const int nCols = static_cast<int>(_frame.nCols);

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
		auto start = std::chrono::high_resolution_clock::now();
		computeImpl();
		auto end = std::chrono::high_resolution_clock::now();
		float duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0F;

		return duration;
	}

	std::vector<std::string> Algorithm::getDevices()
	{
		std::vector<std::string> devices = { "default" };
		return devices;
	}

}
