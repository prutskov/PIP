#pragma once
#include "../../algorithm.h"
#include "../parameter_sharpness.h"
#include <vector>

namespace algorithms
{
	namespace sharpness
	{
		namespace openmp
		{
			class Algorithm : public algorithms::Algorithm
			{
			public:
				void setParameter(ParameterIface *parameter) override;

			protected:
				void computeImpl() override;
				void pixelCompute(int x, int y, const Frame& frame, Frame& result, int indexRes);
				std::vector<float> sharpnessKernel;

			private:
				void generateSharpnessnKernel();
			};
		}
	}
}


