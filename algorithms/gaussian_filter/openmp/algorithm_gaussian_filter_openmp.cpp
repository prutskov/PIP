#define _USE_MATH_DEFINES

#include "stdafx.h"
#include "algorithm_gaussian_filter_openmp.h"
#include <algorithm>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace algorithms
{
	namespace gaussian_filter
	{
		namespace openmp
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

				for (int i = -maskSize / 2; i < maskSize / 2; ++i)
				{
					const float r = static_cast<float>(i*i);
					gaussKernel[i + maskSize / 2] = exp(-r / sqrSigma) / sqrSigma / M_PI;
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
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);

				return 0.0F;
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
				const size_t maskSize = gaussKernel.size();

				std::vector<size_t> indexes;

				/**Get mask of indexes*/
				for (int i = -maskSize / 2; i < maskSize / 2; ++i)
				{
					indexes.emplace_back(y*frame.nCols + i);
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
				
				result.dataRPtr[indexRes] = std::accumulate(vecR.begin(), vecR.end(), 0);
				result.dataGPtr[indexRes] = std::accumulate(vecG.begin(), vecG.end(), 0);
				result.dataBPtr[indexRes] = std::accumulate(vecB.begin(), vecB.end(), 0);
			}

			void Algorithm::firstDirectionCompute(Frame & frame)
			{
				Frame partialResult = _frame.clone();
				const int nRows = static_cast<int>(partialResult.nRows);
				const int nCols = static_cast<int>(partialResult.nCols);
				const int offset = gaussKernel.size() / 2;

#pragma omp parallel for
				for (int i = 0; i < nRows; i++)
				{
					for (int j = offset; j < nCols - offset; j++)
					{
						median3x3(j, i, _frame, partialResult, i*nCols + j);
					}
				}
			}
		}

	}
}
