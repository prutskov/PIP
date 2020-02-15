#include "stdafx.h"
#include "algorithm_median_filter_opencl.h"
#include <fstream>

namespace algorithms
{
	namespace median_filter
	{
		namespace opencl
		{
			Algorithm::Algorithm()
			{
				_platforms.clear();
				_devices.clear();
				 
				/*Get platfroms*/
				cl::Platform::get(&_platforms);

				/*Get all devices*/
				for (cl::Platform plat : _platforms)
				{
					std::vector<cl::Device> device;
					plat.getDevices(CL_DEVICE_TYPE_ALL, &device);
					_devices.insert(_devices.end(), device.begin(), device.end());
				}
			}

			Algorithm::~Algorithm() {}

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

				std::ifstream sourceFile("kernels/nativeBitonic.cl");
				std::string sourceCode(std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
				cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length() + 1));
				_program = cl::Program(_context, source);

				/*try
				{*/
					_program.build(_context.getInfo<CL_CONTEXT_DEVICES>());
				/*}
				catch (const cl::Error &err)
				{
					_log->add(std::string(err.what()) + ": " + _program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(_context.getInfo<CL_CONTEXT_DEVICES>()[0]));
				}*/
			}

			float Algorithm::compute()
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
				return 0.0F;
			}


			void Algorithm::compute3x3()
			{
				utils::Frame result = _frame.clone();
				const int nRows = result.nRows;
				const int nCols = result.nCols;

				cl::CommandQueue comqueque(_context, _context.getInfo<CL_CONTEXT_DEVICES>()[0]);

				cl::Buffer imageRIn = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					(_frame.nRows*_frame.nCols) * sizeof(float), _frame.dataRPtr.get());

				cl::Buffer imageGIn = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					(_frame.nRows*_frame.nCols) * sizeof(float), _frame.dataGPtr.get());

				cl::Buffer imageBIn = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					(_frame.nRows*_frame.nCols) * sizeof(float), _frame.dataBPtr.get());

				cl::Buffer imageROut = cl::Buffer(_context,
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					(result.nRows*result.nCols) * sizeof(float), result.dataRPtr.get());

				cl::Buffer imageGOut = cl::Buffer(_context,
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					(result.nRows*result.nCols) * sizeof(float), result.dataGPtr.get());

				cl::Buffer imageBOut = cl::Buffer(_context,
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					(result.nRows*result.nCols) * sizeof(float), result.dataGPtr.get());

				cl::Kernel kernel(_program, "nativeFilter3x3");

				kernel.setArg(0, result.nRows);
				kernel.setArg(1, result.nCols);
				kernel.setArg(2, imageRIn);
				kernel.setArg(3, imageGIn);
				kernel.setArg(4, imageBIn);
				kernel.setArg(5, imageROut);
				kernel.setArg(6, imageGOut);
				kernel.setArg(7, imageBOut);

				comqueque.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(result.nRows, result.nCols), cl::NDRange(4, 4));
				comqueque.finish();

				comqueque.enqueueReadBuffer(imageROut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), result.dataRPtr.get());
				comqueque.enqueueReadBuffer(imageGOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), result.dataGPtr.get());
				comqueque.enqueueReadBuffer(imageBOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), result.dataBPtr.get());

				_frame = result;
			}

			void Algorithm::compute5x5()
			{
				utils::Frame result = _frame.clone();
				const int nRows = result.nRows;
				const int nCols = result.nCols;

				cl::CommandQueue comqueque(_context, _context.getInfo<CL_CONTEXT_DEVICES>()[0]);

				cl::Buffer imageRIn = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					(_frame.nRows*_frame.nCols) * sizeof(float), _frame.dataRPtr.get());

				cl::Buffer imageGIn = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					(_frame.nRows*_frame.nCols) * sizeof(float), _frame.dataGPtr.get());

				cl::Buffer imageBIn = cl::Buffer(_context,
					CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					(_frame.nRows*_frame.nCols) * sizeof(float), _frame.dataBPtr.get());

				cl::Buffer imageROut = cl::Buffer(_context,
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					(result.nRows*result.nCols) * sizeof(float), result.dataRPtr.get());

				cl::Buffer imageGOut = cl::Buffer(_context,
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					(result.nRows*result.nCols) * sizeof(float), result.dataGPtr.get());

				cl::Buffer imageBOut = cl::Buffer(_context,
					CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
					(result.nRows*result.nCols) * sizeof(float), result.dataGPtr.get());

				cl::Kernel kernel(_program, "nativeFilter5x5");

				kernel.setArg(0, result.nRows);
				kernel.setArg(1, result.nCols);
				kernel.setArg(2, imageRIn);
				kernel.setArg(3, imageGIn);
				kernel.setArg(4, imageBIn);
				kernel.setArg(5, imageROut);
				kernel.setArg(6, imageGOut);
				kernel.setArg(7, imageBOut);

				comqueque.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(result.nRows, result.nCols), cl::NDRange(4, 4));
				comqueque.finish();

				comqueque.enqueueReadBuffer(imageROut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), result.dataRPtr.get());
				comqueque.enqueueReadBuffer(imageGOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), result.dataGPtr.get());
				comqueque.enqueueReadBuffer(imageBOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), result.dataBPtr.get());

				_frame = result;
			}
		}

	}
}
