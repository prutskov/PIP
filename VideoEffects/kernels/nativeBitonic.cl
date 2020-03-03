inline void sortPair(float *a, float *b)
{
	float tmp = *a;
	*a = fmin(tmp, *b);
	*b = fmax(tmp, *b);
}

inline void bitonicSort3x3(float* a, int size)
{

	sortPair(a + 1, a + 2); sortPair(a + 4, a + 5); sortPair(a + 7, a + 8);
	sortPair(a + 0, a + 1); sortPair(a + 3, a + 4); sortPair(a + 6, a + 7);
	sortPair(a + 1, a + 2); sortPair(a + 4, a + 5); sortPair(a + 7, a + 8);
	sortPair(a + 0, a + 3); sortPair(a + 5, a + 8); sortPair(a + 4, a + 7);
	sortPair(a + 3, a + 6); sortPair(a + 1, a + 4); sortPair(a + 2, a + 5);
	sortPair(a + 4, a + 7); sortPair(a + 4, a + 2); sortPair(a + 6, a + 4);
	sortPair(a + 4, a + 2);
}

inline void bitonicSort5x5(float* a, int size)
{
	sortPair(a + 0, a + 1); sortPair(a + 3, a + 4); sortPair(a + 2, a + 4); sortPair(a + 2, a + 3); sortPair(a + 6, a + 7);
	sortPair(a + 5, a + 7); sortPair(a + 5, a + 6); sortPair(a + 9, a + 10); sortPair(a + 8, a + 10); sortPair(a + 8, a + 9);
	sortPair(a + 12, a + 13); sortPair(a + 11, a + 13); sortPair(a + 11, a + 12); sortPair(a + 15, a + 16); sortPair(a + 14, a + 16);
	sortPair(a + 14, a + 15); sortPair(a + 18, a + 19); sortPair(a + 17, a + 19); sortPair(a + 17, a + 18); sortPair(a + 21, a + 22);
	sortPair(a + 20, a + 22); sortPair(a + 20, a + 21); sortPair(a + 23, a + 24); sortPair(a + 2, a + 5); sortPair(a + 3, a + 6);
	sortPair(a + 0, a + 6); sortPair(a + 0, a + 3); sortPair(a + 4, a + 7); sortPair(a + 1, a + 7); sortPair(a + 1, a + 4);
	sortPair(a + 11, a + 14); sortPair(a + 8, a + 14); sortPair(a + 8, a + 11); sortPair(a + 12, a + 15); sortPair(a + 9, a + 15);
	sortPair(a + 9, a + 12); sortPair(a + 13, a + 16); sortPair(a + 10, a + 16); sortPair(a + 10, a + 13); sortPair(a + 20, a + 23);
	sortPair(a + 17, a + 23); sortPair(a + 17, a + 20); sortPair(a + 21, a + 24); sortPair(a + 18, a + 24); sortPair(a + 18, a + 21);
	sortPair(a + 19, a + 22); sortPair(a + 9, a + 18); sortPair(a + 0, a + 18); sortPair(a + 8, a + 17); sortPair(a + 0, a + 9);
	sortPair(a + 10, a + 19); sortPair(a + 1, a + 19); sortPair(a + 1, a + 10); sortPair(a + 11, a + 20); sortPair(a + 2, a + 20);
	sortPair(a + 12, a + 21); sortPair(a + 2, a + 11); sortPair(a + 3, a + 21); sortPair(a + 3, a + 12); sortPair(a + 13, a + 22);
	sortPair(a + 4, a + 22); sortPair(a + 4, a + 13); sortPair(a + 14, a + 23); sortPair(a + 5, a + 23); sortPair(a + 5, a + 14);
	sortPair(a + 15, a + 24); sortPair(a + 6, a + 24); sortPair(a + 6, a + 15); sortPair(a + 7, a + 16); sortPair(a + 7, a + 19);
	sortPair(a + 13, a + 21); sortPair(a + 15, a + 23); sortPair(a + 7, a + 13); sortPair(a + 7, a + 15); sortPair(a + 1, a + 9);
	sortPair(a + 3, a + 11); sortPair(a + 5, a + 17); sortPair(a + 11, a + 17); sortPair(a + 9, a + 17); sortPair(a + 4, a + 10);
	sortPair(a + 6, a + 12); sortPair(a + 7, a + 14); sortPair(a + 4, a + 6); sortPair(a + 4, a + 7); sortPair(a + 12, a + 14);
	sortPair(a + 10, a + 14); sortPair(a + 6, a + 7); sortPair(a + 10, a + 12); sortPair(a + 6, a + 10); sortPair(a + 6, a + 17);
	sortPair(a + 12, a + 17); sortPair(a + 7, a + 17); sortPair(a + 7, a + 10); sortPair(a + 12, a + 18); sortPair(a + 7, a + 12);
	sortPair(a + 10, a + 18); sortPair(a + 12, a + 20);  sortPair(a + 10, a + 20); sortPair(a + 10, a + 12);
}


inline void getMedian3x3(const int x, const int y, const int nRows, const int nCols, const int indexRes,
	__global const float* imageIn, __global float* imageOut)
{
	/*Indexes from original frame for mask*/
	int indexes[9] = { (y - 1)*nCols + x - 1,
					   (y - 1)*nCols + x,
					   (y - 1)*nCols + x + 1,
							 y*nCols + x - 1,
							 y*nCols + x,
							 y*nCols + x + 1,
					   (y + 1)*nCols + x - 1,
					   (y + 1)*nCols + x,
					   (y + 1)*nCols + x + 1 };

	/*Get submatrix from filter-mask*/
	float matrixForSorting[9];
	for (int i = 0; i < 9; i++)
	{
		matrixForSorting[i] = imageIn[indexes[i]];
	}

	/*Sorting array*/
	bitonicSort3x3(matrixForSorting, 9);

	imageOut[indexRes] = matrixForSorting[4];
}

inline void getMedian5x5(const int x, const int y, const int nRows, const int nCols, const int indexRes,
	__global const float* imageIn, 	__global float* imageOut)
{
	/*Indexes from original frame for mask*/
	int indexes[25] = { (y - 2)*nCols + x - 2,
							  (y - 2)*nCols + x - 1,
							  (y - 2)*nCols + x,
							  (y - 2)*nCols + x + 1,
							  (y - 2)*nCols + x + 2,
							  (y - 1)*nCols + x - 2,
							  (y - 1)*nCols + x - 1,
							  (y - 1)*nCols + x,
							  (y - 1)*nCols + x + 1,
							  (y - 1)*nCols + x + 2,
									y*nCols + x - 2,
									y*nCols + x - 1,
									y*nCols + x,
									y*nCols + x + 1,
									y*nCols + x + 2,
							  (y + 1)*nCols + x - 2,
							  (y + 1)*nCols + x - 1,
							  (y + 1)*nCols + x,
							  (y + 1)*nCols + x + 1,
							  (y + 1)*nCols + x + 2,
							  (y + 2)*nCols + x - 2,
							  (y + 2)*nCols + x - 1,
							  (y + 2)*nCols + x,
							  (y + 2)*nCols + x + 1,
							  (y + 2)*nCols + x + 2 };

	/*Get submatrix from filter-mask*/
	float matrixForSorting[25];

	for (int i = 0; i < 25; i++)
	{
		matrixForSorting[i] = imageIn[indexes[i]];
	}

	/*Sorting array*/
	bitonicSort5x5(matrixForSorting, 25);

	imageOut[indexRes] = matrixForSorting[12];
}

__kernel void nativeFilter3x3(const int nRows, const int nCols,
	__global const float* imageIn, __global float* imageOut)
{
	int rowIdx = get_global_id(0);
	int colIdx = get_global_id(1);

	if (((rowIdx > 0) && (rowIdx < (nRows - 1))) && ((colIdx > 0) && (colIdx < (nCols - 1))))
	{
		getMedian3x3(colIdx, rowIdx, nRows, nCols, rowIdx*nCols + colIdx, imageIn, imageOut);
	}
}

__kernel void nativeFilter5x5(const int nRows, const int nCols,
	__global const float* imageIn, __global float* imageOut)
{
	int rowIdx = get_global_id(0);
	int colIdx = get_global_id(1);

	if (((rowIdx > 1) && (rowIdx < (nRows - 2))) && ((colIdx > 1) && (colIdx < (nCols - 2))))
	{
		getMedian5x5(colIdx, rowIdx, nRows, nCols, rowIdx*nCols + colIdx, imageIn, imageOut);
	}
}