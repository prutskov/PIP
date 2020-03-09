#include "stdafx.h"

#include <chrono>

#include "algorithm_sobel_filter_openmp.h"

namespace algorithms
{
	namespace sobel_filter
	{
		namespace openmp
		{
			Algorithm::Algorithm() {}

			Algorithm::~Algorithm() {}

			void Algorithm::generateSobelMatrix()
			{
				gXKernel = {-1, 0, 1,
							-2, 0, 2,
							-1, 0, 1};

				gYKernel = {1, 2, 1,
							0, 0, 0,
							-1, -2, -1};
			}

			void Algorithm::setParameter(ParameterIface *parameter)
			{
				_parameter = parameter;
				generateSobelMatrix();
			}

			float Algorithm::compute()
			{				
				auto start = std::chrono::high_resolution_clock::now();
				computeImpl();
				auto end = std::chrono::high_resolution_clock::now();
				float duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0F;

				return duration;
			}


			void Algorithm::pixelCompute(int x, int y, const Frame& frame, Frame& result, int indexRes)
			{
				const size_t maskSize = 9;

				float sumGxR = 0.f;
				float sumGxG = 0.f;
				float sumGxB = 0.f;

				float sumGyR = 0.f;
				float sumGyG = 0.f;
				float sumGyB = 0.f;

				const size_t indexes[maskSize] = { (y - 1)*frame.nCols + x - 1, (y - 1)*frame.nCols + x, (y - 1)*frame.nCols + x + 1,
														 y*frame.nCols + x - 1, y*frame.nCols + x, y*frame.nCols + x + 1,
											(y + 1)*frame.nCols + x - 1, (y + 1)*frame.nCols + x, (y + 1)*frame.nCols + x + 1 };

				for (int i = 0; i < maskSize; ++i)
				{
					const float valueR = frame.dataRPtr[indexes[i]];
					const float valueG = frame.dataGPtr[indexes[i]];
					const float valueB = frame.dataBPtr[indexes[i]];

					sumGxR += valueR * gXKernel[i];
					sumGxG += valueG * gXKernel[i];
					sumGxB += valueB * gXKernel[i];

					sumGyR += valueR * gYKernel[i];
					sumGyG += valueG * gYKernel[i];
					sumGyB += valueB * gYKernel[i];
				}

				result.dataRPtr[indexRes] = sqrt(sumGxR*sumGxR + sumGyR*sumGyR);
				result.dataGPtr[indexRes] = sqrt(sumGxG*sumGxG + sumGyG*sumGyG);
				result.dataBPtr[indexRes] = sqrt(sumGxB*sumGxB + sumGyB*sumGyB);
			}

			
			void Algorithm::computeImpl()
			{
				Frame result = _frame.clone();
				const int nRows = static_cast<int>(result.nRows);
				const int nCols = static_cast<int>(result.nCols);

#pragma omp parallel for
				for (int i = 1; i < nRows - 1; i++)
				{
					for (int j = 1; j < nCols - 1; j++)
					{
						pixelCompute(j, i, _frame, result, i*nCols + j);
					}
				}

				_frame = result;
			}
		}

	}
}
