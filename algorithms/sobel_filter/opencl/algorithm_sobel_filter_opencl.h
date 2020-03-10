#pragma once
#include "../../algorithm_opencl.h"
#include "../parameter_sobel_filter.h"

namespace algorithms
{
	namespace sobel_filter
	{
		namespace opencl
		{
			class Algorithm : public algorithms::AlgorithmOCL
			{
			public:
				void setParameter(ParameterIface *parameter) override;
			protected:
				void computeImpl() override;
				void buildProgram() override;
			private:
				void generateSobelMatrix();

				std::vector<float> gXKernel;
				std::vector<float> gYKernel;
			};
		}
	}
}


