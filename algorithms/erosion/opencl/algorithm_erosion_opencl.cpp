#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <numeric>
#include <fstream>

#include "algorithm_erosion_opencl.h"

namespace algorithms
{
	namespace erosion
	{
		namespace opencl
		{
			void Algorithm::setParameter(ParameterIface *parameter)
			{
				_parameter = parameter;
				buildProgram();
			}

			void Algorithm::buildProgram()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);
				std::vector<cl::Device> device = { _devices[par->activeDevice] };
				cl::Context context(device);
				_context = context;

				std::ifstream sourceFile("kernels/morphology.cl");
				std::string sourceCode(std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
				cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length() + 1));
				_program = cl::Program(_context, source);

				try
				{
					_program.build(_context.getInfo<CL_CONTEXT_DEVICES>());
				}
				catch (const cl::Error &err)
				{
					std::ofstream fileLog;
					fileLog.open("_build_log.txt");
					fileLog << std::string(err.what()) + ": " + _program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(_context.getInfo<CL_CONTEXT_DEVICES>()[0]);
					fileLog.close();
				}
			}

			void Algorithm::computeImpl()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);

				const int nRows = static_cast<int>(_frame.nRows);
				const int nCols = static_cast<int>(_frame.nCols);

				const int nRowsRes = static_cast<int>(nRows - par->rows + 1);
				const int nColsRes = static_cast<int>(nCols - par->cols + 1);

				Frame result(nRows, nCols,
					std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()),
					std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()),
					std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()));
				
				cl::CommandQueue comqueque(_context, _context.getInfo<CL_CONTEXT_DEVICES>()[0], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);
				
				cl::Buffer imageRIn = cl::Buffer(_context,
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					(nRows*nCols) * sizeof(float), _frame.dataRPtr.get());

				cl::Buffer imageGIn = cl::Buffer(_context,
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					(nRows*nCols) * sizeof(float), _frame.dataGPtr.get());

				cl::Buffer imageBIn = cl::Buffer(_context,
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					(nRows*nCols) * sizeof(float), _frame.dataBPtr.get());

				cl::Buffer imageROut = cl::Buffer(_context,
					CL_MEM_READ_WRITE, nRows*nCols * sizeof(float));

				cl::Buffer imageGOut = cl::Buffer(_context,
					CL_MEM_READ_WRITE, nRows*nCols * sizeof(float));

				cl::Buffer imageBOut = cl::Buffer(_context,
					CL_MEM_READ_WRITE, nRows*nCols * sizeof(float));

				auto setArgs = [&](cl::Kernel& kernel, cl::Buffer& imageIn, cl::Buffer& imageOut)
				{
					kernel.setArg(0, nCols);
					kernel.setArg(1, static_cast<int>(par->rows));
					kernel.setArg(2, static_cast<int>(par->cols));
					kernel.setArg(3, imageIn);
					kernel.setArg(4, imageOut);
					return;
				};

				if (par->type == MorphType::erosion)
				{
					cl::Kernel kernelErosionR(_program, "erosion");
					cl::Kernel kernelErosionG(_program, "erosion");
					cl::Kernel kernelErosionB(_program, "erosion");

					setArgs(kernelErosionR, imageRIn, imageROut);
					setArgs(kernelErosionG, imageGIn, imageGOut);
					setArgs(kernelErosionB, imageBIn, imageBOut);

					comqueque.enqueueNDRangeKernel(kernelErosionR, cl::NullRange, cl::NDRange(nRowsRes, nColsRes));
					comqueque.enqueueNDRangeKernel(kernelErosionG, cl::NullRange, cl::NDRange(nRowsRes, nColsRes));
					comqueque.enqueueNDRangeKernel(kernelErosionB, cl::NullRange, cl::NDRange(nRowsRes, nColsRes));
				}
				else
				{
					cl::Kernel kernelDilationR(_program, "dilation");
					cl::Kernel kernelDilationG(_program, "dilation");
					cl::Kernel kernelDilationB(_program, "dilation");

					setArgs(kernelDilationR, imageRIn, imageROut);
					setArgs(kernelDilationG, imageGIn, imageGOut);
					setArgs(kernelDilationB, imageBIn, imageBOut);

					comqueque.enqueueNDRangeKernel(kernelDilationR, cl::NullRange, cl::NDRange(nRowsRes, nColsRes));
					comqueque.enqueueNDRangeKernel(kernelDilationG, cl::NullRange, cl::NDRange(nRowsRes, nColsRes));
					comqueque.enqueueNDRangeKernel(kernelDilationB, cl::NullRange, cl::NDRange(nRowsRes, nColsRes));
				}
				comqueque.finish();
				
				
				comqueque.enqueueReadBuffer(imageROut, CL_TRUE, 0, nRows*nCols * sizeof(float), result.dataRPtr.get());
				comqueque.enqueueReadBuffer(imageGOut, CL_TRUE, 0, nRows*nCols * sizeof(float), result.dataGPtr.get());
				comqueque.enqueueReadBuffer(imageBOut, CL_TRUE, 0, nRows*nCols * sizeof(float), result.dataBPtr.get());

				_frame = result;
			}
		}

	}
}
