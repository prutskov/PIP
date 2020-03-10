#pragma once
#include "../../algorithm_opencl.h"
#include "../parameter_sharpness.h"

namespace algorithms
{
	namespace sharpness
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
				void generateSharpnessnKernel();

				std::vector<float> sharpnessKernel;
			};
		}
	}
}


