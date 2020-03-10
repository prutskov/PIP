#pragma once
#include "../openmp/algorithm_sobel_filter_openmp.h"
#include "../parameter_sobel_filter.h"

namespace algorithms
{
	namespace sobel_filter
	{
		namespace tbb
		{
			class Algorithm : public algorithms::sobel_filter::openmp::Algorithm
			{
			protected:
				virtual void computeImpl() override;
			};
		}
	}
}


