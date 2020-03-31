#pragma once
#include <fstream>

#include "algorithms.h"
#include "parameters.h"
#include "enums.h"

class Benchmark
{
public:
	Benchmark(Parameters& par, size_t nIter = 5);
	~Benchmark();
	void runAlgorithm(Algorithm& alg);
	Parameters params;
private:
	void benchMedianFilter(std::vector<Frame>& frames);
	void benchGaussFilter(std::vector<Frame>& frames);
	void benchSharpness(std::vector<Frame>& frames);
	void benchSobelFilter(std::vector<Frame>& frames);
	void benchMorphology(std::vector<Frame>& frames);
	size_t nIterations;
	std::ofstream logFile;
	std::vector<Frame> frames;
};

