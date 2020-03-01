#pragma once
#include "../../Algorithm.h"
#include "../parameter_gaussian_filter.h"
#include <vector>

namespace algorithms
{
	namespace gaussian_filter
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
				void generateGaussianKernel();
				void horizDirectionCompute(int x, int y, const Frame& frame, Frame& result, int indexRes);
				void verticDirectionCompute(int x, int y, const Frame& frame, Frame& result, int indexRes);
				void directionsCompute();
				void compute3x3();

				std::vector<float> gaussKernel;
			};
		}
	}
}


