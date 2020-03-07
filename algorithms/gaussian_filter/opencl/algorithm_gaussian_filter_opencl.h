#pragma once
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include "../../Algorithm.h"
#include "../parameter_gaussian_filter.h"

namespace algorithms
{
	namespace gaussian_filter
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
				void generateGaussianKernel();
				void computeImpl();
				std::vector<std::string> getDevices() override;

				std::vector<cl::Platform> _platforms;
				std::vector<cl::Device> _devices;
				cl::Program _program;
				cl::Context _context;

				std::vector<float> gaussKernel;
			};
		}
	}
}

