#pragma once
#include <vector>
#include <string>

#include "Frame.h"

using namespace utils;

namespace algorithms
{
	struct ParameterIface
	{
		size_t activeDevice = 0;

		virtual ~ParameterIface() {};
	};

	class Algorithm
	{
	public:
		Algorithm();
		virtual ~Algorithm();

		virtual Frame getFrame();
		virtual void setFrame(const Frame &frame);
		virtual void setParameter(ParameterIface *parameter);
		virtual void generateNoise(float percent);
		virtual float compute();

		virtual std::vector<std::string> getDevices();

	protected:
		virtual void computeImpl() {};
		Frame _frame;
		ParameterIface *_parameter;
	};
}
