#pragma once
#include "../../algorithm_opencl.h"
#include "../parameter_gaussian_filter.h"

namespace algorithms
{
	namespace gaussian_filter
	{
		namespace opencl
		{
			class Algorithm : public algorithms::AlgorithmOCL
			{
			public:
				void setParameter(ParameterIface *parameter) override;

			protected:
				void buildProgram() override;
				void computeImpl() override;

			private:
				void generateGaussianKernel();
				
				std::vector<float> gaussKernel;
			};
		}
	}
}


