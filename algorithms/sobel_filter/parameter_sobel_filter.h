#pragma once
#include "../Algorithm.h"

namespace algorithms
{
	namespace sobel_filter
	{
		struct Parameter : algorithms::ParameterIface
		{
			Parameter(size_t device = 0)
			{
				activeDevice = device;
			}
		};
	}
}