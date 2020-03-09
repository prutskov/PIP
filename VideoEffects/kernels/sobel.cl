
__kernel void sobel(const int nRows, const int nCols, __global const float* gxKernel, __global const float* gyKernel,
	__global const float* imageIn, __global float* imageOut)
{
	int rowIdx = get_global_id(0);
	int colIdx = get_global_id(1);
	const int maskSize = 9;
	if (((rowIdx > 0) && (rowIdx < (nRows - 1))) && ((colIdx > 0) && (colIdx < (nCols - 1))))
	{
		int indexes[9] = { (rowIdx - 1)*nCols + colIdx - 1,
						   (rowIdx - 1)*nCols + colIdx,
						   (rowIdx - 1)*nCols + colIdx + 1,
								 rowIdx*nCols + colIdx - 1,
								 rowIdx*nCols + colIdx,
								 rowIdx*nCols + colIdx + 1,
						   (rowIdx + 1)*nCols + colIdx - 1,
						   (rowIdx + 1)*nCols + colIdx,
						   (rowIdx + 1)*nCols + colIdx + 1 };

		float sumGx = 0.f;
		float sumGy = 0.f;
		for (int i = 0; i < maskSize; ++i)
		{
			float val = imageIn[indexes[i]];
			sumGx += dot(val, gxKernel[i]);
			sumGy += dot(val, gyKernel[i]);
		}

		imageOut[rowIdx*nCols + colIdx] = sqrt(sumGx*sumGx + sumGy * sumGy);
	}
}
