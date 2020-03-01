#pragma once
#include "../../Algorithm.h"
#include "../parameter_gaussian_filter.h"
#include <vector>

namespace algorithms
{
	namespace gaussian_filter
	{
		namespace tbb
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

				std::vector<float> gaussKernel;
			};
		}
	}
}


