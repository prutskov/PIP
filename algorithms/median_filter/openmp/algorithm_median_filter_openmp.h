#pragma once
#include "../../algorithm.h"
#include "../parameter_median_filter.h"

namespace algorithms
{
	namespace median_filter
	{
		namespace openmp
		{
			class Algorithm : public algorithms::Algorithm
			{
			protected:
				void computeImpl() override;
				virtual void compute3x3();
				virtual void compute5x5();
				virtual void quickSort(float* data, int size);
				void median3x3(int x, int y, const Frame& frame, Frame& result, int indexRes);
				void median5x5(int x, int y, const Frame& frame, Frame& result, int indexRes);
			};
		}
	}
}


