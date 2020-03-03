#include "stdafx.h"
#include "CVManager.h"
#include <memory>

void CVManager::loadImage(std::string path, int flag)
{
	_loadedImage = cv::imread(path, flag);
	//_loadedImage.convertTo(matOpened, CV_32FC1);
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
			frame.dataBPtr[i*nCols + j] = data.at<cv::Vec3b>(i, j)[0];
			frame.dataGPtr[i*nCols + j] = data.at<cv::Vec3b>(i, j)[1];
			frame.dataRPtr[i*nCols + j] = data.at<cv::Vec3b>(i, j)[2];
		}
	}

	return frame;
}

cv::Mat CVManager::convertToMat(Frame data)
{
	const int nRows = static_cast<int>(data.nRows);
	const int nCols = static_cast<int>(data.nCols);

	cv::Mat mat(nRows, nCols, CV_32FC3);

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			mat.at<cv::Vec3f>(i, j)[0] = data.dataBPtr[i*nCols + j] / 255.f;
			mat.at<cv::Vec3f>(i, j)[1] = data.dataGPtr[i*nCols + j] / 255.f;
			mat.at<cv::Vec3f>(i, j)[2] = data.dataRPtr[i*nCols + j] / 255.f;
		}
	}

	return mat.clone();
}

Frame CVManager::getImage()
{
	_imagePtr = convertToPtr(_loadedImage);
	return _imagePtr;
}

Frame CVManager::generateFrame(size_t nRows, size_t nCols)
{
	Frame frame(nRows, nCols,
		std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()),
		std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()),
		std::shared_ptr<float[]>(new float[nRows*nCols], std::default_delete<float[]>()));

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			frame.dataBPtr[i*nCols + j] = static_cast<float>(rand() % 256);
			frame.dataGPtr[i*nCols + j] = static_cast<float>(rand() % 256);
			frame.dataRPtr[i*nCols + j] = static_cast<float>(rand() % 256);
		}
	}

	return frame;
}

