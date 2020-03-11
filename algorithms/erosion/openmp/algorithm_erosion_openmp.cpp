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
					float minR = 255.f;
					float minG = 255.f;
					float minB = 255.f;
					for (int row = y; row < y + rowsSE; ++row)
					{
						for (int col = x; col < x + colsSE; ++col)
						{
							minR = std::min(minR, frame.dataRPtr[row*frame.nCols + col]);
							minG = std::min(minG, frame.dataGPtr[row*frame.nCols + col]);
							minB = std::min(minB, frame.dataBPtr[row*frame.nCols + col]);
						}
					}
					result.dataRPtr[indexRes] = minR;
					result.dataGPtr[indexRes] = minG;
					result.dataBPtr[indexRes] = minB;
				}
				else
				{
					float maxR = 0.f;
					float maxG = 0.f;
					float maxB = 0.f;
					for (int row = y; row < y + rowsSE; ++row)
					{
						for (int col = x; col < x + colsSE; ++col)
						{
							maxR = std::max(maxR, frame.dataRPtr[row*frame.nCols + col]);
							maxG = std::max(maxG, frame.dataGPtr[row*frame.nCols + col]);
							maxB = std::max(maxB, frame.dataBPtr[row*frame.nCols + col]);
						}
					}
					result.dataRPtr[indexRes] = maxR;
					result.dataGPtr[indexRes] = maxG;
					result.dataBPtr[indexRes] = maxB;
				}

			}
					   
			void Algorithm::computeImpl()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);

				const int nRows = static_cast<int>(_frame.nRows);
				const int nCols = static_cast<int>(_frame.nCols);

				const int nRowsRes = static_cast<int>(nRows - par->rows + 1);
				const int nColsRes = static_cast<int>(nCols - par->cols + 1);

				Frame result(nRowsRes, nColsRes,
					std::shared_ptr<float[]>(new float[nRowsRes*nColsRes], std::default_delete<float[]>()),
					std::shared_ptr<float[]>(new float[nRowsRes*nColsRes], std::default_delete<float[]>()),
					std::shared_ptr<float[]>(new float[nRowsRes*nColsRes], std::default_delete<float[]>()));

#pragma omp parallel for
				for (int i = 0; i < nRowsRes; i++)
				{
					for (int j = 0; j < nColsRes; j++)
					{
						pixelCompute(j, i, _frame, result, par->type, (int)par->rows, (int)par->cols, i*nColsRes + j);
					}
				}

				_frame = result;
			}
		}

	}
}
