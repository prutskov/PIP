#pragma once
#include "../openmp/algorithm_erosion_openmp.h"
#include "../parameter_erosion.h"

namespace algorithms
{
	namespace erosion
	{
		namespace tbb
		{
			class Algorithm : public algorithms::erosion::openmp::Algorithm
			{
			protected:
				virtual void computeImpl() override;
			};
		}
	}
}


