#include "stdafx.h"

#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"

#include "algorithm_gaussian_filter_tbb.h"

namespace algorithms
{
	namespace gaussian_filter
	{
		namespace tbb
		{
			void Algorithm::computeImpl()
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
