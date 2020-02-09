#pragma once
#include <memory>

namespace utils
{
	class Frame
	{
	public:
		Frame();
		Frame(size_t rows, size_t cols, std::shared_ptr<float[]> dataR,
			std::shared_ptr<float[]> dataG, std::shared_ptr<float[]> dataB);

		Frame clone();
		bool isNull();

		std::shared_ptr<float[]> dataRPtr;
		std::shared_ptr<float[]> dataGPtr;
		std::shared_ptr<float[]> dataBPtr;
		std::size_t nRows;
		std::size_t nCols;
	};

}	// namespace utils

