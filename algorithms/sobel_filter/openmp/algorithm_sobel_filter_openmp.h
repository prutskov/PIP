#pragma once
#include "../../Algorithm.h"
#include "../parameter_sobel_filter.h"
#include <vector>

namespace algorithms
{
	namespace sobel_filter
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

			protected:
				void generateSobelMatrix();
				void pixelCompute(int x, int y, const Frame& frame, Frame& result, int indexRes);
				virtual void computeImpl();

				std::vector<float> gXKernel;
				std::vector<float> gYKernel;
			};
		}
	}
}


