#include "stdafx.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"
#include "tbb/parallel_sort.h"

#include "algorithm_median_filter_tbb.h"

namespace algorithms
{
	namespace median_filter
	{
		namespace tbb
		{
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
