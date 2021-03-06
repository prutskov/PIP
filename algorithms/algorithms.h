#pragma once
#include "algorithm.h"
#include "algorithm_opencl.h"
#include "median_filter/parameter_median_filter.h"
#include "median_filter/openmp/algorithm_median_filter_openmp.h"
#include "median_filter/opencl/algorithm_median_filter_opencl.h"
#include "median_filter/tbb/algorithm_median_filter_tbb.h"

#include "gaussian_filter/parameter_gaussian_filter.h"
#include "gaussian_filter/openmp/algorithm_gaussian_filter_openmp.h"
#include "gaussian_filter/opencl/algorithm_gaussian_filter_opencl.h"
#include "gaussian_filter/tbb/algorithm_gaussian_filter_tbb.h"

#include "sharpness/parameter_sharpness.h"
#include "sharpness/openmp/algorithm_sharpness_openmp.h"
#include "sharpness/opencl/algorithm_sharpness_opencl.h"
#include "sharpness/tbb/algorithm_sharpness_tbb.h"

#include "sobel_filter/parameter_sobel_filter.h"
#include "sobel_filter/openmp/algorithm_sobel_filter_openmp.h"
#include "sobel_filter/opencl/algorithm_sobel_filter_opencl.h"
#include "sobel_filter/tbb/algorithm_sobel_filter_tbb.h"

#include "erosion/parameter_erosion.h"
#include "erosion/opencl/algorithm_erosion_opencl.h"
#include "erosion/openmp/algorithm_erosion_openmp.h"
#include "erosion/tbb/algorithm_erosion_tbb.h"
