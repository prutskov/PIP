#include "stdafx.h"
#include <fstream>

#include "algorithm_sobel_filter_opencl.h"

namespace algorithms
{
	namespace sobel_filter
	{
		namespace opencl
		{
			void Algorithm::setParameter(ParameterIface *parameter)
			{
				_parameter = parameter;
				buildProgram();
				generateSobelMatrix();
			}

			void Algorithm::buildProgram()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);
				std::vector<cl::Device> device = { _devices[par->activeDevice] };
				cl::Context context(device);
				_context = context;

				std::ifstream sourceFile("kernels/sobel.cl");
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

			void Algorithm::generateSobelMatrix()
			{
				gXKernel = { -1, 0, 1,
							-2, 0, 2,
							-1, 0, 1 };

				gYKernel = { 1, 2, 1,
							0, 0, 0,
							-1, -2, -1 };
			}
					   
			void Algorithm::computeImpl()
			{
				const int nRows = static_cast<int>(_frame.nRows);
				const int nCols = static_cast<int>(_frame.nCols);
				const int shift = 1;

				cl::CommandQueue comqueque(_context, _context.getInfo<CL_CONTEXT_DEVICES>()[0], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);

				cl::Buffer gXBuf = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					gXKernel.size() * sizeof(float), gXKernel.data());

				cl::Buffer gYBuf = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					gYKernel.size() * sizeof(float), gYKernel.data());

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

				cl::Kernel kernelSobelR(_program, "sobel");
				cl::Kernel kernelSobelG(_program, "sobel");
				cl::Kernel kernelSobelB(_program, "sobel");
				auto setArgs = [&](cl::Kernel& kernel, cl::Buffer& imageIn, cl::Buffer& imageOut)
				{
					kernel.setArg(0, nRows);
					kernel.setArg(1, nCols);
					kernel.setArg(2, gXBuf);
					kernel.setArg(3, gYBuf);
					kernel.setArg(4, imageIn);
					kernel.setArg(5, imageOut);
					return;
				};

				setArgs(kernelSobelR, imageRIn, imageROut);
				setArgs(kernelSobelG, imageGIn, imageGOut);
				setArgs(kernelSobelB, imageBIn, imageBOut);


				comqueque.enqueueNDRangeKernel(kernelSobelR, cl::NullRange, cl::NDRange(nRows, nCols), cl::NDRange(4, 4));
				comqueque.enqueueNDRangeKernel(kernelSobelG, cl::NullRange, cl::NDRange(nRows, nCols), cl::NDRange(4, 4));
				comqueque.enqueueNDRangeKernel(kernelSobelB, cl::NullRange, cl::NDRange(nRows, nCols), cl::NDRange(4, 4));
				comqueque.finish();

				comqueque.enqueueReadBuffer(imageROut, CL_TRUE, 0, nRows*nCols * sizeof(float), _frame.dataRPtr.get());
				comqueque.enqueueReadBuffer(imageGOut, CL_TRUE, 0, nRows*nCols * sizeof(float), _frame.dataGPtr.get());
				comqueque.enqueueReadBuffer(imageBOut, CL_TRUE, 0, nRows*nCols * sizeof(float), _frame.dataBPtr.get());
			}
		}

	}
}
