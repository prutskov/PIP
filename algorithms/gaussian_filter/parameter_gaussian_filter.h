#pragma once
#include "../Algorithm.h"

namespace algorithms
{
	namespace gaussian_filter
	{
		struct Parameter : algorithms::ParameterIface
		{
			Parameter(size_t mask = 3, float s = 1.f, size_t device = 0)
			{
				maskSize = mask;
				sigma = s;
				activeDevice = device;
			}

			size_t maskSize = 3;
			float sigma = 1.f;
		};
	}
}