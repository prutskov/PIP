#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include "algorithm_gaussian_filter_tbb.h"
#include <algorithm>
#include <numeric>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"

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
				directionsCompute();
				return 0.0F;
			}


			void Algorithm::horizDirectionCompute(int x, int y, const Frame& frame, Frame& result, int indexRes)
			{
				const size_t maskSize = gaussKernel.size();

				std::vector<size_t> indexes;

				int shift = static_cast<int>(maskSize / 2);

				for (int i = -shift; i <= shift; ++i)
				{
					indexes.push_back(y*frame.nCols + i + x);
				}

				std::vector<float> vecR;
				std::vector<float> vecG;
				std::vector<float> vecB;

				/**Convolution*/
				for (int i = 0; i < maskSize; i++)
				{
					vecR.emplace_back(frame.dataRPtr[indexes[i]] * gaussKernel[i]);
					vecG.emplace_back(frame.dataGPtr[indexes[i]] * gaussKernel[i]);
					vecB.emplace_back(frame.dataBPtr[indexes[i]] * gaussKernel[i]);
				}

				result.dataRPtr[indexRes] = std::accumulate(vecR.begin(), vecR.end(), 0.f);
				result.dataGPtr[indexRes] = std::accumulate(vecG.begin(), vecG.end(), 0.f);
				result.dataBPtr[indexRes] = std::accumulate(vecB.begin(), vecB.end(), 0.f);
			}

			void Algorithm::verticDirectionCompute(int x, int y, const Frame& frame, Frame& result, int indexRes)
			{
				const size_t maskSize = gaussKernel.size();

				std::vector<size_t> indexes;

				int shift = static_cast<int>(maskSize / 2);

				for (int i = -shift; i <= shift; ++i)
				{
					indexes.push_back((y + i)*frame.nCols + x);
				}

				std::vector<float> vecR;
				std::vector<float> vecG;
				std::vector<float> vecB;

				/**Convolution*/
				for (int i = 0; i < maskSize; i++)
				{
					vecR.emplace_back(frame.dataRPtr[indexes[i]] * gaussKernel[i]);
					vecG.emplace_back(frame.dataGPtr[indexes[i]] * gaussKernel[i]);
					vecB.emplace_back(frame.dataBPtr[indexes[i]] * gaussKernel[i]);
				}

				result.dataRPtr[indexRes] = std::accumulate(vecR.begin(), vecR.end(), 0.f);
				result.dataGPtr[indexRes] = std::accumulate(vecG.begin(), vecG.end(), 0.f);
				result.dataBPtr[indexRes] = std::accumulate(vecB.begin(), vecB.end(), 0.f);
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
