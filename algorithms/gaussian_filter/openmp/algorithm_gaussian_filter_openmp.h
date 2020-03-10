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
				void setParameter(ParameterIface *parameter) override;
			protected:
				void computeImpl() override;
				void horizDirectionCompute(int x, int y, const Frame& frame, Frame& result, int indexRes);
				void verticDirectionCompute(int x, int y, const Frame& frame, Frame& result, int indexRes);

				std::vector<float> gaussKernel;
			private:
				void generateGaussianKernel();

			};
		}
	}
}


