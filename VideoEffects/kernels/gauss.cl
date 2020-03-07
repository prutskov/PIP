
__kernel void horizDirectionKernel(const int nRows, const int nCols, const int shift, __global const float* gaussKernel,
	__global const float* imageIn, __global float* imageOut)
{
	int rowIdx = get_global_id(0);
	int colIdx = get_global_id(1);

	if ((colIdx > (shift - 1)) && (colIdx < (nCols - shift)))
	{
		float sum = 0.f;
		for (int i = -shift, idx = 0; i <= shift; ++i, ++idx)
		{
			sum += dot(imageIn[rowIdx*nCols + i + colIdx], gaussKernel[idx]);
		}

		imageOut[rowIdx*nCols + colIdx] = sum;
	}
}

__kernel void verticDirectionKernel(const int nRows, const int nCols, const int shift, __global const float* gaussKernel,
	__global const float* imageIn, __global float* imageOut)
{
	int rowIdx = get_global_id(0);
	int colIdx = get_global_id(1);

	if ((rowIdx > (shift - 1)) && (rowIdx < (nRows - shift)))
	{
		float sum = 0.f;
		for (int i = -shift, idx = 0; i <= shift; ++i, ++idx)
		{
			sum += dot(imageIn[(rowIdx + i)*nCols + colIdx], gaussKernel[idx]);
		}

		imageOut[rowIdx*nCols + colIdx] = sum;
	}
}
