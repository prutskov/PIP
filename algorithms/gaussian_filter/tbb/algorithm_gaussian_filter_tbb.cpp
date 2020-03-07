#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <numeric>
#include <chrono>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"

#include "algorithm_gaussian_filter_tbb.h"

namespace algorithms
{
	namespace gaussian_filter
	{
		namespace tbb
		{
			Algorithm::Algorithm() {}

			Algorithm::~Algorithm() {}

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

			float Algorithm::compute()
			{
				auto start = std::chrono::high_resolution_clock::now();
				directionsCompute();
				auto end = std::chrono::high_resolution_clock::now();
				float duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0F;

				return duration;
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
					sumR += frame.dataRPtr[(y + i)*frame.nCols + x]*gaussKernel[idx];
					sumG += frame.dataGPtr[(y + i)*frame.nCols + x]*gaussKernel[idx];
					sumB += frame.dataBPtr[(y + i)*frame.nCols + x]*gaussKernel[idx];
				}
				
				result.dataRPtr[indexRes] = sumR;
				result.dataGPtr[indexRes] = sumG;
				result.dataBPtr[indexRes] = sumB;
			}

			void Algorithm::directionsCompute()
			{
				Frame partialResult = _frame.clone();
				const int nRows = static_cast<int>(partialResult.nRows);
				const int nCols = static_cast<int>(partialResult.nCols);
				const int offset = static_cast<int>(gaussKernel.size() / 2);

				::tbb::parallel_for(::tbb::blocked_range2d<int>(0, nRows, offset, nCols - offset),
					[&](::tbb::blocked_range2d<int> r)
				{
					for (int i = r.rows().begin(); i != r.rows().end(); ++i)
					{
						for (int j = r.cols().begin(); j != r.cols().end(); ++j)
						{
							horizDirectionCompute(j, i, _frame, partialResult, i*nCols + j);
						}
					}
				});

				::tbb::parallel_for(::tbb::blocked_range2d<int>(offset, nRows - offset, 0, nCols),
					[&](::tbb::blocked_range2d<int> r)
				{
					for (int i = r.rows().begin(); i != r.rows().end(); ++i)
					{
						for (int j = r.cols().begin(); j != r.cols().end(); ++j)
						{
							verticDirectionCompute(j, i, partialResult, _frame, i*nCols + j);
						}
					}
				});
			}

		}
	}
}
