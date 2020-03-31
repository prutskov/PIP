#include "stdafx.h"
#include <fstream>

#include "algorithm_median_filter_opencl.h"

namespace algorithms
{
	namespace median_filter
	{
		namespace opencl
		{
			void Algorithm::buildProgram()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);
				std::vector<cl::Device> device = { _devices[par->activeDevice] };
				cl::Context context(device);
				_context = context;

				std::ifstream sourceFile("kernels/nativeBitonic.cl");
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
				if (par->mask == Mask::MASK3X3)
				{
					compute3x3();
				}
				else
				{
					compute5x5();
				}
			}

			void Algorithm::compute3x3()
			{
				utils::Frame result = _frame.clone();
				const int nRows = static_cast<int>(result.nRows);
				const int nCols = static_cast<int>(result.nCols);

				cl::CommandQueue comqueque(_context, _context.getInfo<CL_CONTEXT_DEVICES>()[0], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);

				cl::Buffer imageRIn = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					(nRows*nCols) * sizeof(float), _frame.dataRPtr.get());

				cl::Buffer imageGIn = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					(nRows*nCols) * sizeof(float), _frame.dataGPtr.get());

				cl::Buffer imageBIn = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					(nRows*nCols) * sizeof(float), _frame.dataBPtr.get());

				cl::Buffer imageROut = cl::Buffer(_context,
					CL_MEM_READ_WRITE, nRows*nCols * sizeof(float));

				cl::Buffer imageGOut = cl::Buffer(_context,
					CL_MEM_READ_WRITE, nRows*nCols * sizeof(float));

				cl::Buffer imageBOut = cl::Buffer(_context,
					CL_MEM_READ_WRITE, nRows*nCols * sizeof(float));

				cl::Kernel kernelR(_program, "nativeFilter3x3");
				cl::Kernel kernelG(_program, "nativeFilter3x3");
				cl::Kernel kernelB(_program, "nativeFilter3x3");

				kernelR.setArg(0, nRows);
				kernelR.setArg(1, nCols);
				kernelR.setArg(2, imageRIn);
				kernelR.setArg(3, imageROut);

				kernelG.setArg(0, nRows);
				kernelG.setArg(1, nCols);
				kernelG.setArg(2, imageGIn);
				kernelG.setArg(3, imageGOut);

				kernelB.setArg(0, nRows);
				kernelB.setArg(1, nCols);
				kernelB.setArg(2, imageBIn);
				kernelB.setArg(3, imageBOut);

				if ((nRows % 4 == 0) && (nCols % 4 == 0))
				{
					comqueque.enqueueNDRangeKernel(kernelR, cl::NullRange, cl::NDRange(nRows, nCols), cl::NDRange(4, 4));
					comqueque.enqueueNDRangeKernel(kernelG, cl::NullRange, cl::NDRange(nRows, nCols), cl::NDRange(4, 4));
					comqueque.enqueueNDRangeKernel(kernelB, cl::NullRange, cl::NDRange(nRows, nCols), cl::NDRange(4, 4));
				}
				else
				{
					comqueque.enqueueNDRangeKernel(kernelR, cl::NullRange, cl::NDRange(nRows, nCols));
					comqueque.enqueueNDRangeKernel(kernelG, cl::NullRange, cl::NDRange(nRows, nCols));
					comqueque.enqueueNDRangeKernel(kernelB, cl::NullRange, cl::NDRange(nRows, nCols));
				}
				comqueque.finish();

				comqueque.enqueueReadBuffer(imageROut, CL_TRUE, 0, nRows*nCols * sizeof(float), _frame.dataRPtr.get());
				comqueque.enqueueReadBuffer(imageGOut, CL_TRUE, 0, nRows*nCols * sizeof(float), _frame.dataGPtr.get());
				comqueque.enqueueReadBuffer(imageBOut, CL_TRUE, 0, nRows*nCols * sizeof(float), _frame.dataBPtr.get());
			}

			void Algorithm::compute5x5()
			{
				utils::Frame result = _frame.clone();
				const int nRows = static_cast<int>(result.nRows);
				const int nCols = static_cast<int>(result.nCols);

				cl::CommandQueue comqueque(_context, _context.getInfo<CL_CONTEXT_DEVICES>()[0], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);

				cl::Buffer imageRIn = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					(nRows*nCols) * sizeof(float), _frame.dataRPtr.get());

				cl::Buffer imageGIn = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					(nRows*nCols) * sizeof(float), _frame.dataGPtr.get());

				cl::Buffer imageBIn = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					(nRows*nCols) * sizeof(float), _frame.dataBPtr.get());

				cl::Buffer imageROut = cl::Buffer(_context,
					CL_MEM_READ_WRITE, nRows*nCols * sizeof(float));

				cl::Buffer imageGOut = cl::Buffer(_context,
					CL_MEM_READ_WRITE, nRows*nCols * sizeof(float));

				cl::Buffer imageBOut = cl::Buffer(_context,
					CL_MEM_READ_WRITE, nRows*nCols * sizeof(float));

				cl::Kernel kernelR(_program, "nativeFilter5x5");
				cl::Kernel kernelG(_program, "nativeFilter5x5");
				cl::Kernel kernelB(_program, "nativeFilter5x5");

				kernelR.setArg(0, nRows);
				kernelR.setArg(1, nCols);
				kernelR.setArg(2, imageRIn);
				kernelR.setArg(3, imageROut);

				kernelG.setArg(0, nRows);
				kernelG.setArg(1, nCols);
				kernelG.setArg(2, imageGIn);
				kernelG.setArg(3, imageGOut);

				kernelB.setArg(0, nRows);
				kernelB.setArg(1, nCols);
				kernelB.setArg(2, imageBIn);
				kernelB.setArg(3, imageBOut);

				if ((nRows % 4 == 0) && (nCols % 4 == 0))
				{
					comqueque.enqueueNDRangeKernel(kernelR, cl::NullRange, cl::NDRange(nRows, nCols), cl::NDRange(4, 4));
					comqueque.enqueueNDRangeKernel(kernelG, cl::NullRange, cl::NDRange(nRows, nCols), cl::NDRange(4, 4));
					comqueque.enqueueNDRangeKernel(kernelB, cl::NullRange, cl::NDRange(nRows, nCols), cl::NDRange(4, 4));
				}
				else
				{
					comqueque.enqueueNDRangeKernel(kernelR, cl::NullRange, cl::NDRange(nRows, nCols));
					comqueque.enqueueNDRangeKernel(kernelG, cl::NullRange, cl::NDRange(nRows, nCols));
					comqueque.enqueueNDRangeKernel(kernelB, cl::NullRange, cl::NDRange(nRows, nCols));
				}
				comqueque.finish();

				comqueque.enqueueReadBuffer(imageROut, CL_TRUE, 0, nRows*nCols * sizeof(float), _frame.dataRPtr.get());
				comqueque.enqueueReadBuffer(imageGOut, CL_TRUE, 0, nRows*nCols * sizeof(float), _frame.dataGPtr.get());
				comqueque.enqueueReadBuffer(imageBOut, CL_TRUE, 0, nRows*nCols * sizeof(float), _frame.dataBPtr.get());
			}
		}

	}
}
