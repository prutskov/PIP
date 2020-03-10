#include "stdafx.h"
#include <algorithm>

#include "algorithm_median_filter_openmp.h"

namespace algorithms
{
	namespace median_filter
	{
		namespace openmp
		{
			void Algorithm::computeImpl()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);
				if (par->mask == Mask::MASK3X3)
				{
					compute3x3();
				}
				else
				{
					compute5x5();
				}
			}

			void Algorithm::quickSort(float* data, int size)
			{
				std::sort(data, data + size);
			}

			void Algorithm::compute3x3()
			{
				Frame result = _frame.clone();
				const int nRows = static_cast<int>(result.nRows);
				const int nCols = static_cast<int>(result.nCols);

#pragma omp parallel for
				for (int i = 1; i < nRows - 1; i++)
				{
					for (int j = 1; j < nCols - 1; j++)
					{
						median3x3(j, i, _frame, result, i*nCols + j);
					}
				}

				_frame = result;
			}

			void Algorithm::median3x3(int x, int y, const Frame& frame, Frame& result, int indexRes)
			{
				const size_t maskSize = 9;
				/*Indexes from original frame for mask*/
				size_t indexes[maskSize] = { (y - 1)*frame.nCols + x - 1, (y - 1)*frame.nCols + x, (y - 1)*frame.nCols + x + 1,
								  y*frame.nCols + x - 1, y*frame.nCols + x, y*frame.nCols + x + 1,
								  (y + 1)*frame.nCols + x - 1, (y + 1)*frame.nCols + x, (y + 1)*frame.nCols + x + 1 };

				/*Get submatrix from filter-mask*/
				float matrixForSortingR[maskSize];
				float matrixForSortingG[maskSize];
				float matrixForSortingB[maskSize];

				for (int i = 0; i < maskSize; i++)
				{
					matrixForSortingR[i] = frame.dataRPtr[indexes[i]];
					matrixForSortingG[i] = frame.dataGPtr[indexes[i]];
					matrixForSortingB[i] = frame.dataBPtr[indexes[i]];
				}

				/*Sorting array*/
				quickSort(matrixForSortingR, maskSize);
				quickSort(matrixForSortingG, maskSize);
				quickSort(matrixForSortingB, maskSize);

				result.dataRPtr[indexRes] = matrixForSortingR[4];
				result.dataGPtr[indexRes] = matrixForSortingG[4];
				result.dataBPtr[indexRes] = matrixForSortingB[4];
			}

			void Algorithm::median5x5(int x, int y, const Frame& frame, Frame& result, int indexRes)
			{
				const size_t maskSize = 25;
				/*Indexes from original frame for mask*/
				size_t indexes[maskSize] = { (y - 2)*frame.nCols + x - 2,
									(y - 2)*frame.nCols + x - 1,
									(y - 2)*frame.nCols + x,
									(y - 2)*frame.nCols + x + 1,
									(y - 2)*frame.nCols + x + 2,
									(y - 1)*frame.nCols + x - 2,
									(y - 1)*frame.nCols + x - 1,
									(y - 1)*frame.nCols + x,
									(y - 1)*frame.nCols + x + 1,
									(y - 1)*frame.nCols + x + 2,
										  y*frame.nCols + x - 2,
										  y*frame.nCols + x - 1,
										  y*frame.nCols + x,
										  y*frame.nCols + x + 1,
										  y*frame.nCols + x + 2,
									(y + 1)*frame.nCols + x - 2,
									(y + 1)*frame.nCols + x - 1,
									(y + 1)*frame.nCols + x,
									(y + 1)*frame.nCols + x + 1,
									(y + 1)*frame.nCols + x + 2,
									(y + 2)*frame.nCols + x - 2,
									(y + 2)*frame.nCols + x - 1,
									(y + 2)*frame.nCols + x,
									(y + 2)*frame.nCols + x + 1,
									(y + 2)*frame.nCols + x + 2 };

				/*Get submatrix from filter-mask*/
				float matrixForSortingR[maskSize];
				float matrixForSortingG[maskSize];
				float matrixForSortingB[maskSize];

				for (int i = 0; i < maskSize; i++)
				{
					matrixForSortingR[i] = frame.dataRPtr[indexes[i]];
					matrixForSortingG[i] = frame.dataGPtr[indexes[i]];
					matrixForSortingB[i] = frame.dataBPtr[indexes[i]];
				}

				/*Sorting array*/
				quickSort(matrixForSortingR, maskSize);
				quickSort(matrixForSortingG, maskSize);
				quickSort(matrixForSortingB, maskSize);

				result.dataRPtr[indexRes] = matrixForSortingR[12];
				result.dataGPtr[indexRes] = matrixForSortingG[12];
				result.dataBPtr[indexRes] = matrixForSortingB[12];
			}

			void Algorithm::compute5x5()
			{
				Frame result = _frame.clone();
				const int nRows = static_cast<int>(result.nRows);
				const int nCols = static_cast<int>(result.nCols);

#pragma omp parallel for
				for (int i = 2; i < nRows - 2; i++)
				{
					for (int j = 2; j < nCols - 2; j++)
					{
						median5x5(j, i, _frame, result, i*nCols + j);
					}
				}

				_frame = result;
			}
		}

	}
}
