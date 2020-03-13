#include "stdafx.h"
#include <memory>

#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"

#include "algorithm_erosion_tbb.h"

namespace algorithms
{
	namespace erosion
	{
		namespace tbb
		{					   
			void Algorithm::computeImpl()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);

				const int nRows = static_cast<int>(_frame.nRows);
				const int nCols = static_cast<int>(_frame.nCols);

				const int nRowsRes = static_cast<int>(nRows - par->rows + 1);
				const int nColsRes = static_cast<int>(nCols - par->cols + 1);

				Frame result(nRows, nCols,
					std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()),
					std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()),
					std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()));
				
				::tbb::parallel_for(::tbb::blocked_range2d<int>(0, nRowsRes, 0, nColsRes),
					[&](::tbb::blocked_range2d<int> r)
				{
					for (int i = r.rows().begin(); i != r.rows().end(); ++i)
					{
						for (int j = r.cols().begin(); j != r.cols().end(); ++j)
						{
							pixelCompute(j, i, _frame, result, par->type, (int)par->rows, (int)par->cols, i*nCols + j);
						}
					}
				});

				_frame = result;
			}
		}

	}
}
