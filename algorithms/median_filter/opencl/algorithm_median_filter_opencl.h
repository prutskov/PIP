#pragma once
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include "../../Algorithm.h"
#include "../Parameter.h"

namespace algorithms
{
	namespace median_filter
	{
		namespace opencl
		{
			class Algorithm : public algorithms::Algorithm
			{
			public:
				Algorithm();
				float compute() override;
				void setParameter(ParameterIface *parameter) override;
				virtual ~Algorithm();
			private:
				void buildProgram();
				void compute3x3();
				void compute5x5();

				std::vector<cl::Platform> _platforms;
				std::vector<cl::Device> _devices;
				cl::Program _program;
				cl::Context _context;
			};
		}
	}
}


