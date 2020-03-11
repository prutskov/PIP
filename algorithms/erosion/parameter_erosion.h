#pragma once
#include "../algorithm.h"

namespace algorithms
{
	namespace erosion
	{
		enum MorphType
		{
			erosion,
			dilation
		};
		struct Parameter : algorithms::ParameterIface
		{
			Parameter(size_t n_rows = 4, size_t n_cols = 6, MorphType mType = MorphType::erosion, size_t device = 0)
			{
				rows = n_rows;
				cols = n_cols;
				type = mType;
				activeDevice = device;
			}

			size_t rows = 4;
			size_t cols = 6;
			MorphType type = erosion;
		};
	}
}