#include "stdafx.h"
#include "CVManager.h"

namespace utils
{
	void CVManager::loadImage(std::string path, int flag)
	{
		_loadedImage = cv::imread(path, flag);
	}

	void CVManager::imageShow(int mode)
	{
		std::string nameWindow = "Original image. Size: " + std::to_string(_loadedImage.rows) +
			"x" + std::to_string(_loadedImage.cols);
		cv::namedWindow(nameWindow, mode);
		cv::moveWindow(nameWindow, 0, 0);
		cv::imshow(nameWindow, _loadedImage);
	}

	void CVManager::imageShow(std::string name, Frame image, int mode)
	{
		std::string nameWindow = name + " Size: " + std::to_string(image.nRows) +
			"x" + std::to_string(image.nCols);
		cv::namedWindow(nameWindow, mode);
		cv::moveWindow(nameWindow, 0, 0);
		cv::imshow(nameWindow, convertToMat(image));
	}

	bool CVManager::isNullImage()
	{
		return _loadedImage.empty();
	}

	Frame CVManager::convertToPtr(cv::Mat data)
	{
		const std::size_t nRows = data.rows;
		const std::size_t nCols = data.cols;

		Frame frame(nRows, nCols,
			std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()),
			std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()),
			std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()));

		for (int i = 0; i < nRows; i++)
		{
			for (int j = 0; j < nCols; j++)
			{
				frame.dataBPtr[i*nCols + j] = data.at<cv::Vec3f>(i, j)[0];
				frame.dataGPtr[i*nCols + j] = data.at<cv::Vec3f>(i, j)[1];
				frame.dataRPtr[i*nCols + j] = data.at<cv::Vec3f>(i, j)[2];
			}
		}

		return frame;
	}

	cv::Mat CVManager::convertToMat(Frame data)
	{
		const int nRows = data.nRows;
		const int nCols = data.nCols;

		cv::Mat mat(nRows, nCols, CV_8UC3);

		for (int i = 0; i < nRows; i++)
		{
			for (int j = 0; j < nCols; j++)
			{
				mat.at<cv::Vec3f>(i, j)[0] = data.dataBPtr[i*nCols + j];
				mat.at<cv::Vec3f>(i, j)[1] = data.dataGPtr[i*nCols + j];
				mat.at<cv::Vec3f>(i, j)[2] = data.dataRPtr[i*nCols + j];
			}
		}

		return mat.clone();
	}

	Frame CVManager::getImage()
	{
		_imagePtr = convertToPtr(_loadedImage);
		return _imagePtr;
	}


}	//namespace utils