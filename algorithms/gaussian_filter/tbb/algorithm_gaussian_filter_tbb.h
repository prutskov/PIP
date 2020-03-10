#pragma once
#include "../openmp/algorithm_gaussian_filter_openmp.h"
#include "../parameter_gaussian_filter.h"

namespace algorithms
{
	namespace gaussian_filter
	{
		namespace tbb
		{
			class Algorithm : public algorithms::gaussian_filter::openmp::Algorithm
			{
			protected:
				void computeImpl() override;
			};
		}
	}
}


