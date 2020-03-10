#pragma once
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <vector>

#include "Frame.h"
#include "algorithm.h"

using namespace utils;

namespace algorithms
{
	class AlgorithmOCL : public Algorithm
	{
	public:
		AlgorithmOCL();

		virtual void setParameter(ParameterIface *parameter) override;
		virtual std::vector<std::string> getDevices() override;

	protected:
		virtual void buildProgram() = 0;

		std::vector<cl::Platform> _platforms;
		std::vector<cl::Device> _devices;
		cl::Program _program;
		cl::Context _context;
	};
}
