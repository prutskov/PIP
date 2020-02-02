#pragma once
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "utils.h"

using namespace utils;

class CVManager
{
public:
	CVManager() {}
	~CVManager() {}
	void loadImage(std::string path, int flag = cv::IMREAD_GRAYSCALE);
	void imageShow(int mode);
	void imageShow(std::string name, Frame image, int mode);
	bool isNullImage();
	Frame convertToPtr(cv::Mat data);
	cv::Mat convertToMat(Frame data);
	Frame getImage();

private:
	cv::Mat _loadedImage;
	Frame _imagePtr;
};


