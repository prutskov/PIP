#pragma once
#include "../../algorithm_opencl.h"
#include "../parameter_erosion.h"

namespace algorithms
{
	namespace erosion
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
			};
		}
	}
}


