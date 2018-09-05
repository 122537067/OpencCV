#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

void main()
{
	// 读取图像
	cv::Mat image = cv::imread("road.jpg");
	if (image.empty()) {
		std::cout << "图像读取失败!" << std::endl;
		exit(EXIT_FAILURE);
	}
	cv::imshow("原始图像", image);

	// 转灰度图
	cv::Mat gray;
	cv::cvtColor(image, gray, CV_BGR2GRAY);
	cv::imshow("灰度图", gray);

	// 应用Canny算法
	double low = 350;		// 低阈值
	double high = 1250;		// 高阈值

	cv::Mat contours;
	cv::Canny(gray,// 灰度图像
		contours,	// 输出轮廓图
		low,		// 低阈值
		high);		// 高阈值
	cv::imshow("Canny轮廓", contours);

	cv::waitKey();
}