#pragma once
#include "../openmp/algorithm_sharpness_openmp.h"
#include "../parameter_sharpness.h"

namespace algorithms
{
	namespace sharpness
	{
		namespace tbb
		{
			class Algorithm : public algorithms::sharpness::openmp::Algorithm
			{
			protected:
				void computeImpl() override;
			};
		}
	}
}


