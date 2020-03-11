#pragma once
#include "median_filter/parameter_median_filter.h"
#include "erosion/parameter_erosion.h"

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

	/**Morphology (erosion/dilatation) parameters*/
	size_t morphRowsMask = 4;
	size_t morphColsMask = 6;
	algorithms::erosion::MorphType morphType = algorithms::erosion::MorphType::erosion;
};