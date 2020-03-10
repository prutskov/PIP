#include "stdafx.h"

#include <fstream>
#include <algorithm>

#include "algorithm_sharpness_opencl.h"

namespace algorithms
{
	namespace sharpness
	{
		namespace opencl
		{
			void Algorithm::setParameter(ParameterIface *parameter)
			{
				_parameter = parameter;
				buildProgram();
				generateSharpnessnKernel();
			}

			void Algorithm::buildProgram()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);
				std::vector<cl::Device> device = { _devices[par->activeDevice] };
				cl::Context context(device);
				_context = context;

				std::ifstream sourceFile("kernels/sharpness.cl");
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

			void Algorithm::generateSharpnessnKernel()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);
				const float k = par->k;
				const size_t kernelSize = 3 * 3;

				sharpnessKernel.resize(kernelSize);

				std::for_each(sharpnessKernel.begin(), sharpnessKernel.end(), [k](float& val)
				{
					val = k / 8.f;
				});

				sharpnessKernel[kernelSize / 2] = 1 + k;

			}

			void Algorithm::computeImpl()
			{
				utils::Frame partialResult = _frame.clone();
				const int nRows = static_cast<int>(partialResult.nRows);
				const int nCols = static_cast<int>(partialResult.nCols);
				const int shift = 1;

				cl::CommandQueue comqueque(_context, _context.getInfo<CL_CONTEXT_DEVICES>()[0], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);

				cl::Buffer sharpnessBuf = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					sharpnessKernel.size() * sizeof(float), sharpnessKernel.data());

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
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					(partialResult.nRows*partialResult.nCols) * sizeof(float), partialResult.dataRPtr.get());

				cl::Buffer imageGOut = cl::Buffer(_context,
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					(partialResult.nRows*partialResult.nCols) * sizeof(float), partialResult.dataGPtr.get());

				cl::Buffer imageBOut = cl::Buffer(_context,
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					(partialResult.nRows*partialResult.nCols) * sizeof(float), partialResult.dataGPtr.get());

				cl::Kernel kernelSharpnessR(_program, "sharpness");
				cl::Kernel kernelSharpnessG(_program, "sharpness");
				cl::Kernel kernelSharpnessB(_program, "sharpness");
				auto setArgs = [&](cl::Kernel& kernel, cl::Buffer& imageIn, cl::Buffer& imageOut)
				{
					kernel.setArg(0, nRows);
					kernel.setArg(1, nCols);
					kernel.setArg(2, sharpnessBuf);
					kernel.setArg(3, imageIn);
					kernel.setArg(4, imageOut);
					return;
				};

				setArgs(kernelSharpnessR, imageRIn, imageROut);
				setArgs(kernelSharpnessG, imageGIn, imageGOut);
				setArgs(kernelSharpnessB, imageBIn, imageBOut);


				comqueque.enqueueNDRangeKernel(kernelSharpnessR, cl::NullRange, cl::NDRange(nRows, nCols), cl::NDRange(4, 4));
				comqueque.enqueueNDRangeKernel(kernelSharpnessG, cl::NullRange, cl::NDRange(nRows, nCols), cl::NDRange(4, 4));
				comqueque.enqueueNDRangeKernel(kernelSharpnessB, cl::NullRange, cl::NDRange(nRows, nCols), cl::NDRange(4, 4));
				comqueque.finish();

				comqueque.enqueueReadBuffer(imageROut, CL_TRUE, 0, nRows*nCols * sizeof(float), _frame.dataRPtr.get());
				comqueque.enqueueReadBuffer(imageGOut, CL_TRUE, 0, nRows*nCols * sizeof(float), _frame.dataGPtr.get());
				comqueque.enqueueReadBuffer(imageBOut, CL_TRUE, 0, nRows*nCols * sizeof(float), _frame.dataBPtr.get());
			}
		}

	}
}
