#include "stdafx.h"
#include <fstream>
#include <chrono>

#include "algorithm_median_filter_opencl.h"

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

			float Algorithm::compute()
			{
				const Parameter *par = dynamic_cast<Parameter *>(_parameter);
				auto start = std::chrono::high_resolution_clock::now();
				if (par->mask == Mask::MASK3X3)
				{
					compute3x3();
				}
				else
				{
					compute5x5();
				}

				auto end = std::chrono::high_resolution_clock::now();
				float duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0F;

				return duration;
			}


			void Algorithm::compute3x3()
			{
				utils::Frame result = _frame.clone();
				const int nRows = static_cast<int>(result.nRows);
				const int nCols = static_cast<int>(result.nCols);

				cl::CommandQueue comqueque(_context, _context.getInfo<CL_CONTEXT_DEVICES>()[0], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);

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

				comqueque.enqueueNDRangeKernel(kernelR, cl::NullRange, cl::NDRange(result.nRows, result.nCols), cl::NDRange(4, 4));
				comqueque.enqueueNDRangeKernel(kernelG, cl::NullRange, cl::NDRange(result.nRows, result.nCols), cl::NDRange(4, 4));
				comqueque.enqueueNDRangeKernel(kernelB, cl::NullRange, cl::NDRange(result.nRows, result.nCols), cl::NDRange(4, 4));
				comqueque.finish();

				comqueque.enqueueReadBuffer(imageROut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), _frame.dataRPtr.get());
				comqueque.enqueueReadBuffer(imageGOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), _frame.dataGPtr.get());
				comqueque.enqueueReadBuffer(imageBOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), _frame.dataBPtr.get());
			}

			void Algorithm::compute5x5()
			{
				utils::Frame result = _frame.clone();
				const int nRows = static_cast<int>(result.nRows);
				const int nCols = static_cast<int>(result.nCols);

				cl::CommandQueue comqueque(_context, _context.getInfo<CL_CONTEXT_DEVICES>()[0], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);

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

				comqueque.enqueueNDRangeKernel(kernelR, cl::NullRange, cl::NDRange(result.nRows, result.nCols), cl::NDRange(4, 4));
				comqueque.enqueueNDRangeKernel(kernelG, cl::NullRange, cl::NDRange(result.nRows, result.nCols), cl::NDRange(4, 4));
				comqueque.enqueueNDRangeKernel(kernelB, cl::NullRange, cl::NDRange(result.nRows, result.nCols), cl::NDRange(4, 4));
				comqueque.finish();

				comqueque.enqueueReadBuffer(imageROut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), _frame.dataRPtr.get());
				comqueque.enqueueReadBuffer(imageGOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), _frame.dataGPtr.get());
				comqueque.enqueueReadBuffer(imageBOut, CL_TRUE, 0, result.nRows*result.nCols * sizeof(float), _frame.dataBPtr.get());
			}

			std::vector<std::string> Algorithm::getDevices()
			{
				std::vector<std::string> deviceNames(_devices.size());

				for (size_t i = 0; i < _devices.size(); i++)
				{
					deviceNames[i] = _devices[i].getInfo<CL_DEVICE_NAME>();
				}

				return deviceNames;
			}
		}

	}
}
