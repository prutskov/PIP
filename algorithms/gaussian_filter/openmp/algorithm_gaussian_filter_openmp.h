#pragma once
#include "../../Algorithm.h"
#include "../Parameter.h"

namespace algorithms
{
	namespace median_filter
	{
		namespace openmp
		{
			class Algorithm : public algorithms::Algorithm
			{
			public:
				Algorithm();
				float compute() override;
				virtual ~Algorithm();
			private:
				void quickSort(float* data, int size);
				void median3x3(int x, int y, const Frame& frame, Frame& result, int indexRes);
				void median5x5(int x, int y, const Frame& frame, Frame& result, int indexRes);
				void compute3x3();
				void compute5x5();
			};
		}
	}
}


