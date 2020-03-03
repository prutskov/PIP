#include "stdafx.h"
#include <algorithm>
#include <chrono>

#include "algorithm_median_filter_tbb.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"
#include "tbb/parallel_sort.h"

namespace algorithms
{
	namespace median_filter
	{
		namespace tbb
		{
			Algorithm::Algorithm() {}

			Algorithm::~Algorithm() {}

			float Algorithm::compute()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);
				auto start = std::chrono::high_resolution_clock::now();
				if (par->mask == Mask::MASK3X3)
				{
					compute3x3();
				}
				else
				{
					compute5x5();
				}
				
				auto end = std::chrono::high_resolution_clock::now();
				float duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0F;

				return duration;
			}


			void Algorithm::quickSort(float* data, int size)
			{
				::tbb::parallel_sort(data, data + size);
			}

			void Algorithm::compute3x3()
			{
				Frame result = _frame.clone();
				const int nRows = static_cast<int>(result.nRows);
				const int nCols = static_cast<int>(result.nCols);

				::tbb::parallel_for(::tbb::blocked_range2d<int>(1, nRows - 1, 1, nCols - 1),
					[&](::tbb::blocked_range2d<int> r)
				{
					for (int i = r.rows().begin(); i != r.rows().end(); ++i)
					{
						for (int j = r.cols().begin(); j != r.cols().end(); ++j)
						{
							median3x3(j, i, _frame, result, i*nCols + j);
						}
					}
				});

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

				::tbb::parallel_for(::tbb::blocked_range2d<int>(2, nRows - 2, 2, nCols - 2),
					[&](::tbb::blocked_range2d<int> r)
				{
					for (int i = r.rows().begin(); i != r.rows().end(); ++i)
					{
						for (int j = r.cols().begin(); j != r.cols().end(); ++j)
						{
							median5x5(j, i, _frame, result, i*nCols + j);
						}
					}
				});

				_frame = result;
			}
		}

	}
}
