#pragma once
#include "../../Algorithm.h"
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
				Algorithm();
				float compute() override;
				void setParameter(ParameterIface *parameter) override;
				virtual ~Algorithm();
			private:
				void pixelCompute(int x, int y, const Frame& frame, Frame& result, int indexRes);
				void generateSharpnessnKernel();
				void computeImpl();

				std::vector<float> sharpnessKernel;
			};
		}
	}
}


