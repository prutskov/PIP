#include "stdafx.h"

#include <chrono>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"

#include "algorithm_sobel_filter_tbb.h"

namespace algorithms
{
	namespace sobel_filter
	{
		namespace tbb
		{
			void Algorithm::computeImpl()
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
							this->pixelCompute(j, i, _frame, result, i*nCols + j);
						}
					}
				});
				_frame = result;
			}
		}

	}
}
