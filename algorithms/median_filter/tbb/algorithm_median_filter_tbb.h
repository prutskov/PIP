#pragma once
#include "../openmp/algorithm_median_filter_openmp.h"
#include "../parameter_median_filter.h"

namespace algorithms
{
	namespace median_filter
	{
		namespace tbb
		{
			class Algorithm : public algorithms::median_filter::openmp::Algorithm
			{
			protected:
				void compute3x3() override;
				void compute5x5() override;
				void quickSort(float* data, int size) override;
			};
		}
	}
}


