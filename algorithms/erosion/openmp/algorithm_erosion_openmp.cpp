#include "stdafx.h"
#include <memory>
#include <algorithm>

#include "algorithm_erosion_openmp.h"

namespace algorithms
{
	namespace erosion
	{
		namespace openmp
		{
			void Algorithm::pixelCompute(int x, int y, const Frame& frame, Frame& result, MorphType morphType, int rowsSE, int colsSE, int indexRes)
			{
				if (morphType == MorphType::erosion)
				{
					float min = 255.f;
					for (int row = y; row < y + rowsSE; ++row)
					{
						for (int col = x; col < x + colsSE; ++col)
						{
							min = std::min(min, frame.dataRPtr[row*frame.nCols + col]);
						}
					}
					result.dataRPtr[indexRes] = min;
				}
				else
				{
					float max = 0.f;
					for (int row = y; row < y + rowsSE; ++row)
					{
						for (int col = x; col < x + colsSE; ++col)
						{
							max = std::max(max, frame.dataRPtr[row*frame.nCols + col]);
						}
					}
					result.dataRPtr[indexRes] = max;
				}

			}

			void Algorithm::frameToGrayScale()
			{
				const int nRows = static_cast<int>(_frame.nRows);
				const int nCols = static_cast<int>(_frame.nCols);
#pragma omp parallel for
				for (int row = 0; row < nRows; row++)
				{
					for (int col = 0; col < nCols; col++)
					{
						float val = 0.299*_frame.dataRPtr[row*nCols + col] + 0.587*_frame.dataGPtr[row*nCols + col] + 0.114*_frame.dataBPtr[row*nCols + col];

						_frame.dataRPtr[row*nCols + col] = val;
					}
				}

				_frame.dataGPtr = _frame.dataBPtr = _frame.dataRPtr;
			}


			void Algorithm::computeImpl()
			{
				frameToGrayScale();

				const Parameter *par = dynamic_cast<Parameter *>(_parameter);

				const int nRows = static_cast<int>(_frame.nRows);
				const int nCols = static_cast<int>(_frame.nCols);

				const int nRowsRes = static_cast<int>(nRows - par->rows + 1);
				const int nColsRes = static_cast<int>(nCols - par->cols + 1);

				Frame result(nRowsRes, nColsRes,
					std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()),
					std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()),
					std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()));

#pragma omp parallel for
				for (int i = 0; i < nRowsRes; i++)
				{
					for (int j = 0; j < nColsRes; j++)
					{
						pixelCompute(j, i, _frame, result, par->type, par->rows, par->cols, i*nColsRes + j);
					}
				}

				_frame = result;
				_frame.dataGPtr = _frame.dataBPtr = _frame.dataRPtr;
			}
		}

	}
}
