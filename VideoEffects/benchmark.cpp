#include "stdafx.h"
#include <memory>

#include "benchmark.h"
#include "CVManager.h"


Benchmark::Benchmark(Parameters& par, size_t nIter) : params(par), nIterations(nIter)
{
	logFile.open("_benchmark_log.txt", std::ios::out);
}


Benchmark::~Benchmark()
{
	logFile.close();
}

void Benchmark::runBenchmark()
{
	logFile << "################ Results of performace testing ################\n";
	std::vector<Frame> frames;

	frames.push_back(CVManager::generateFrame(480, 640));
	frames.push_back(CVManager::generateFrame(720, 1280));
	frames.push_back(CVManager::generateFrame(1080, 1920));
	//frames.push_back(CVManager::generateFrame(2160, 4096));

	benchMedianFilter(frames);
}

void Benchmark::benchMedianFilter(std::vector<Frame>& frames)
{
	logFile << "\nMedian filter\n";

	std::shared_ptr<algorithms::Algorithm> alg;
	algorithms::ParameterIface *parameters;

	std::string mask = params.medianFilterMask == algorithms::median_filter::MASK3X3 ? "3x3" : "5x5";
	logFile << "Parameters: " << "\tmask " << mask << std::endl;
	std::string deviceName;

	logFile << "|    Image size    |    OpenMP    |   Intel TBB  |    OpenCL    |\n";
	for(const Frame& frame : frames)
	{
		float timeOMP = 0.f;
		float timeTBB = 0.f;
		float timeOCL = 0.f;

		{ // openmp test
			alg = std::shared_ptr<algorithms::median_filter::openmp::Algorithm>(new algorithms::median_filter::openmp::Algorithm());
			parameters = new algorithms::median_filter::Parameter(params.medianFilterMask);

			alg->setParameter(parameters);
			alg->setFrame(frame);

			for (size_t i = 0; i < nIterations; ++i)
			{
				timeOMP += alg->compute();
			}

			timeOMP /= static_cast<float>(nIterations);
		}

		{ // tbb test
			alg = std::shared_ptr<algorithms::median_filter::tbb::Algorithm>(new algorithms::median_filter::tbb::Algorithm());
			parameters = new algorithms::median_filter::Parameter(params.medianFilterMask);

			alg->setParameter(parameters);
			alg->setFrame(frame);

			for (size_t i = 0; i < nIterations; ++i)
			{
				timeTBB += alg->compute();
			}

			timeTBB /= static_cast<float>(nIterations);
		}

		{ // opencl test
			alg = std::shared_ptr<algorithms::median_filter::opencl::Algorithm>(new algorithms::median_filter::opencl::Algorithm());
			parameters = new algorithms::median_filter::Parameter(params.medianFilterMask, params.activeDevice);

			alg->setParameter(parameters);
			alg->setFrame(frame);

			for (size_t i = 0; i < nIterations; ++i)
			{
				timeOCL += alg->compute();
			}

			timeOCL /= static_cast<float>(nIterations);

			deviceName = alg->getDevices()[params.activeDevice];
		}


		std::string resolution = std::to_string(frame.nRows) + "x" + std::to_string(frame.nCols);
		logFile << "|";
		logFile << std::setw(18);
		logFile << resolution << "|";
		logFile << std::setw(14) << timeOMP << "|";
		logFile << std::setw(14) << timeTBB << "|";
		logFile << std::setw(14) << timeOCL << "|";
		logFile << std::endl;
	}

	logFile << "Used OpenCL device: " << deviceName << std::endl;


}
