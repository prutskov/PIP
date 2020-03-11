
__kernel void erosion(const int nCols, const int nColsRes, const int nRowsSE, int nColsSE,
	__global const float* imageIn, __global float* imageOut)
{
	int rowIdx = get_global_id(0);
	int colIdx = get_global_id(1);

	float minVal = 255.f;
	const int rowLimit = rowIdx + nRowsSE;
	const int colLimit = colIdx + nColsSE;
	for (int row = rowIdx; row < rowLimit; row++)
	{
		for (int col = colIdx; col < colLimit; col++)
		{
			minVal = fmin(minVal, imageIn[row*nCols + col]);
		}
	}

	imageOut[rowIdx*nColsRes + colIdx] = minVal;
}
__kernel void dilation(const int nCols, const int nColsRes, const int nRowsSE, int nColsSE,
	__global const float* imageIn, __global float* imageOut)
{
	int rowIdx = get_global_id(0);
	int colIdx = get_global_id(1);

	float maxVal = 0.f;
	const int rowLimit = rowIdx + nRowsSE;
	const int colLimit = colIdx + nColsSE;
	for (int row = rowIdx; row < rowLimit; row++)
	{
		for (int col = colIdx; col < colLimit; col++)
		{
			maxVal = fmax(maxVal, imageIn[row*nCols + col]);
		}
	}

	imageOut[rowIdx*nColsRes + colIdx] = maxVal;
}
