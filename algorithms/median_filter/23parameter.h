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
			Mask mask = Mask::MASK3X3;
			size_t activeDevice = 0;
		};
	}
}