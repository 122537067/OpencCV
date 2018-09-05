#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

void main()
{
	// 读取输入图像
	cv::Mat image = cv::imread("castle.jpg");
	cv::imshow("原图", image);

	// 平滑滤波
	cv::Mat result;
	//cv::blur(image, result,
	//	cv::Size(5, 5));	// 滤波器尺寸
	cv::boxFilter(image, result, CV_8U, cv::Size(5, 5));
	cv::imshow("平滑后图像", result);

	// 高斯平滑滤波
	cv::GaussianBlur(image, result, cv::Size(5, 5), 0);
	cv::imshow("高斯平滑后图像", result);

	cv::waitKey();
}