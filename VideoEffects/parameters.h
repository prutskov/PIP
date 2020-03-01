#pragma once
#include "median_filter/parameter_median_filter.h"

struct Parameters
{
	/**Median filter parameters*/
	algorithms::median_filter::Mask medianFilterMask = algorithms::median_filter::Mask::MASK3X3;
};