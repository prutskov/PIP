#pragma once
#include "../../algorithm_opencl.h"
#include "../parameter_median_filter.h"

namespace algorithms
{
	namespace median_filter
	{
		namespace opencl
		{
			class Algorithm : public algorithms::AlgorithmOCL
			{
			protected:
				void buildProgram() override;
				void computeImpl() override;
			private:
				void compute3x3();
				void compute5x5();
			};
		}
	}
}


