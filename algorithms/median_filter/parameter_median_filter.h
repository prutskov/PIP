#pragma once
#include "../Algorithm.h"

namespace algorithms
{
	namespace median_filter
	{
		enum Mask
		{
			MASK3X3,
			MASK5X5
		};

		struct Parameter : algorithms::ParameterIface
		{
			Parameter(Mask m = Mask::MASK3X3, size_t device = 0)
			{
				mask = m;
				activeDevice = device;
			}
			Mask mask = Mask::MASK3X3;
		};
	}
}