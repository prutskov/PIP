#include "stdafx.h"

#include <algorithm>

#include "algorithm_sharpness_openmp.h"

namespace algorithms
{
	namespace sharpness
	{
		namespace openmp
		{
			void Algorithm::generateSharpnessnKernel()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);
				const float k = par->k;
				const size_t kernelSize = 3*3;

				sharpnessKernel.resize(kernelSize);
				
				std::for_each(sharpnessKernel.begin(), sharpnessKernel.end(), [k](float& val)
				{
					val = - k / 8.f;
				});

				sharpnessKernel[kernelSize / 2] = 1 + k;
			}

			void Algorithm::setParameter(ParameterIface *parameter)
			{
				_parameter = parameter;
				generateSharpnessnKernel();
			}
					   
			void Algorithm::pixelCompute(int x, int y, const Frame& frame, Frame& result, int indexRes)
			{
				const size_t maskSize = 9;//sharpnessKernel.size();

				const int offset = static_cast<int>(sqrt(sharpnessKernel.size()) / 2);

				float sumR = 0.f;
				float sumG = 0.f;
				float sumB = 0.f;

				const size_t indexes[maskSize] = { (y - 1)*frame.nCols + x - 1, (y - 1)*frame.nCols + x, (y - 1)*frame.nCols + x + 1,
														 y*frame.nCols + x - 1, y*frame.nCols + x, y*frame.nCols + x + 1,
											(y + 1)*frame.nCols + x - 1, (y + 1)*frame.nCols + x, (y + 1)*frame.nCols + x + 1 };

				for (int i = 0; i < maskSize; ++i)
				{
					sumR += frame.dataRPtr[indexes[i]] * sharpnessKernel[i];
					sumG += frame.dataGPtr[indexes[i]] * sharpnessKernel[i];
					sumB += frame.dataBPtr[indexes[i]] * sharpnessKernel[i];
				}

				result.dataRPtr[indexRes] = sumR;
				result.dataGPtr[indexRes] = sumG;
				result.dataBPtr[indexRes] = sumB;
			}

			void Algorithm::computeImpl()
			{
				Frame result = _frame.clone();
				const int nRows = static_cast<int>(result.nRows);
				const int nCols = static_cast<int>(result.nCols);
				const int offset = static_cast<int>(sqrt(sharpnessKernel.size()) / 2); // offsets 9 / 3 /2 = 1

#pragma omp parallel for
				for (int i = offset; i < nRows - offset; i++)
				{
					for (int j = offset; j < nCols - offset; j++)
					{
						pixelCompute(j, i, _frame, result, i*nCols + j);
					}
				}

				_frame = result;
			}
		}

	}
}
