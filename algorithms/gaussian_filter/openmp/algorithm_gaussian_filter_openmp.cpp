#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <numeric>

#include "algorithm_gaussian_filter_openmp.h"

namespace algorithms
{
	namespace gaussian_filter
	{
		namespace openmp
		{
			void Algorithm::generateGaussianKernel()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);
				const size_t maskSize = par->maskSize;
				const float sigma = par->sigma;

				gaussKernel.resize(maskSize);

				const float sqrSigma = 2.f * sigma * sigma;
				float sum = 0.f;

				int shift = static_cast<int>(maskSize / 2);
				for (int i = -shift; i <= shift; ++i)
				{
					const float r = static_cast<float>(i*i);
					gaussKernel[i + maskSize / 2] = static_cast<float>(exp(-r / sqrSigma) / sqrSigma / M_PI);
					sum += gaussKernel[i + maskSize / 2];
				}

				std::for_each(gaussKernel.begin(), gaussKernel.end(), [sum](float& val)
				{
					val /= sum;
				});

			}

			void Algorithm::setParameter(ParameterIface *parameter)
			{
				_parameter = parameter;
				generateGaussianKernel();
			}
					   
			void Algorithm::horizDirectionCompute(int x, int y, const Frame& frame, Frame& result, int indexRes)
			{
				const size_t maskSize = gaussKernel.size();

				int shift = static_cast<int>(maskSize / 2);

				float sumR = 0.f;
				float sumG = 0.f;
				float sumB = 0.f;

				for (int i = -shift, idx = 0; i <= shift; ++i, ++idx)
				{
					sumR += frame.dataRPtr[y*frame.nCols + i + x] * gaussKernel[idx];
					sumG += frame.dataGPtr[y*frame.nCols + i + x] * gaussKernel[idx];
					sumB += frame.dataBPtr[y*frame.nCols + i + x] * gaussKernel[idx];
				}

				result.dataRPtr[indexRes] = sumR;
				result.dataGPtr[indexRes] = sumG;
				result.dataBPtr[indexRes] = sumB;
			}

			void Algorithm::verticDirectionCompute(int x, int y, const Frame& frame, Frame& result, int indexRes)
			{
				const size_t maskSize = gaussKernel.size();

				int shift = static_cast<int>(maskSize / 2);

				float sumR = 0.f;
				float sumG = 0.f;
				float sumB = 0.f;


				for (int i = -shift, idx = 0; i <= shift; ++i, ++idx)
				{
					sumR += frame.dataRPtr[(y + i)*frame.nCols + x] * gaussKernel[idx];
					sumG += frame.dataGPtr[(y + i)*frame.nCols + x] * gaussKernel[idx];
					sumB += frame.dataBPtr[(y + i)*frame.nCols + x] * gaussKernel[idx];
				}

				result.dataRPtr[indexRes] = sumR;
				result.dataGPtr[indexRes] = sumG;
				result.dataBPtr[indexRes] = sumB;
			}

			void Algorithm::computeImpl()
			{
				Frame partialResult = _frame.clone();
				const int nRows = static_cast<int>(partialResult.nRows);
				const int nCols = static_cast<int>(partialResult.nCols);
				const int offset = static_cast<int>(gaussKernel.size() / 2);

#pragma omp parallel for
				for (int i = 0; i < nRows; i++)
				{
					for (int j = offset; j < nCols - offset; j++)
					{
						horizDirectionCompute(j, i, _frame, partialResult, i*nCols + j);
					}
				}

#pragma omp parallel for
				for (int i = offset; i < nRows - offset; i++)
				{
					for (int j = 0; j < nCols; j++)
					{
						verticDirectionCompute(j, i, partialResult, _frame, i*nCols + j);
					}
				}
			}
		}

	}
}
