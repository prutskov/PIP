#pragma once
#include "../../algorithm.h"
#include "../parameter_erosion.h"
#include <vector>

namespace algorithms
{
	namespace erosion
	{
		namespace openmp
		{
			class Algorithm : public algorithms::Algorithm
			{
			protected:
				virtual void computeImpl() override;
				void pixelCompute(int x, int y, const Frame& frame, Frame& result,
					MorphType morphType, int rowsSE, int colsSE, int indexRes);

			};
		}
	}
}


