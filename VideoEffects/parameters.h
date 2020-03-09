#pragma once
#include "median_filter/parameter_median_filter.h"

struct Parameters
{
	/**Common parameters*/
	size_t activeDevice = 0;

	/**Median filter parameters*/
	algorithms::median_filter::Mask medianFilterMask = algorithms::median_filter::Mask::MASK3X3;

	/**Gaussian filter parameters*/
	size_t gaussFilterMask = 7;
	float sigma = 5.f;

	/**Sharpness parameters*/
	float k = 2.f;
};