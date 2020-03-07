#pragma once
#include <fstream>

#include "algorithms.h"
#include "parameters.h"

class Benchmark
{
public:
	Benchmark(Parameters& par, size_t nIter = 5);
	~Benchmark();
	void runBenchmark();
	Parameters params;
private:
	void benchMedianFilter(std::vector<Frame>& frames);
	void benchGaussFilter(std::vector<Frame>& frames);
	size_t nIterations;
	std::ofstream logFile;
};
