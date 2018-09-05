#pragma once
#include "ColorDetector.h"

class ColorDetectController
{
private:
	// 包含算法的类
	ColorDetector *cdetect;

	cv::Mat image;		// 被处理的图像
	cv::Mat result;		// 结果图像

public:

	ColorDetectController()
	{
		// 建立算法类对象
		cdetect = new ColorDetector();
	}

	~ColorDetectController()
	{
		delete cdetect;	// 释放动态分配的类实例的内存
	}

	// 设置颜色差距的阈值
	void setColorDistanceThreshold(int distance) {
		cdetect->setColorDistanceThreshold(distance);
	}

	// 取得颜色差距的阈值
	int getColorDistanceThreshold() const {
		return cdetect->getColorDistanceThreshold();
	}

	// 设置被检测的颜色
	void setTargetColor(unsigned char red,
		unsigned char green, unsigned char blue) {
		cdetect->setTargetColor(blue, green, red);
	}

	// 取得被检测的颜色
	void getTargetColor(unsigned char &red,
		unsigned char &green, unsigned char &blue) const {
		cv::Vec3b color = cdetect->getTargetColor();

		red = color[2];
		green = color[1];
		blue = color[0];
	}

	// 设置输入图像，从文件中读取它
	bool setInputImage(std::string filename) {
		image = cv::imread(filename);
		return !image.empty();
	}

	// 返回当前的输入图像
	const cv::Mat getInputImage() const {
		return image;
	}

	// 执行图像处理
	void process() {
		result = cdetect->process(image);
	}

	// 返回最后处理的结果图像
	const cv::Mat getLastResult() const {
		return result;
	}
};

