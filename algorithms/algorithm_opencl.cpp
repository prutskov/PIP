#include "stdafx.h"
#include "algorithm_opencl.h"

namespace algorithms
{

	AlgorithmOCL::AlgorithmOCL()
	{
		_platforms.clear();
		_devices.clear();

		/*Get platfroms*/
		cl::Platform::get(&_platforms);

		/*Get all devices*/
		for (cl::Platform plat : _platforms)
		{
			std::vector<cl::Device> device;
			plat.getDevices(CL_DEVICE_TYPE_ALL, &device);
			_devices.insert(_devices.end(), device.begin(), device.end());
		}
	}

	void AlgorithmOCL::setParameter(ParameterIface * parameter)
	{
		_parameter = parameter;
		buildProgram();
	}
	
	std::vector<std::string> AlgorithmOCL::getDevices()
	{
		std::vector<std::string> deviceNames(_devices.size());

		for (size_t i = 0; i < _devices.size(); i++)
		{
			deviceNames[i] = _devices[i].getInfo<CL_DEVICE_NAME>();
		}

		return deviceNames;
	}

}
