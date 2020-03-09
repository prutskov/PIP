#pragma once
#include "../Algorithm.h"

namespace algorithms
{
	namespace sharpness
	{
		struct Parameter : algorithms::ParameterIface
		{
			Parameter(float m_k = 2.f, size_t device = 0)
			{
				k = m_k;
				activeDevice = device;
			}
			float k = 2.f;
		};
	}
}