#include "stdafx.h"

#include <algorithm>
#include <chrono>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"

#include "algorithm_sharpness_tbb.h"

namespace algorithms
{
	namespace sharpness
	{
		namespace tbb
		{
			void Algorithm::computeImpl()
			{
				Frame result = _frame.clone();
				const int nRows = static_cast<int>(result.nRows);
				const int nCols = static_cast<int>(result.nCols);
				const int offset = static_cast<int>(sqrt(sharpnessKernel.size()) / 2); // offsets 9 / 3 /2 = 1
				
				::tbb::parallel_for(::tbb::blocked_range2d<int>(offset, nRows - offset, offset, nCols - offset),
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
