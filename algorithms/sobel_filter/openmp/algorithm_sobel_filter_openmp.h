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
				void setParameter(ParameterIface *parameter) override;

			protected:
				virtual void computeImpl() override;
				void generateSobelMatrix();
				void pixelCompute(int x, int y, const Frame& frame, Frame& result, int indexRes);

			private:
				std::vector<float> gXKernel;
				std::vector<float> gYKernel;
			};
		}
	}
}


