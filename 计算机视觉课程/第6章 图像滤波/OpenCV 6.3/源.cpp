#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

void main()
{
	// 读取输入图像
	cv::Mat image = cv::imread("castleWithNoises.jpg");
	cv::imshow("盐噪声图像", image);

	cv::Mat result;
	cv::blur(image, result, cv::Size(5, 5));
	cv::imshow("均值滤波后图像", result);

	cv::GaussianBlur(image, result, cv::Size(7, 7), 0);
	cv::imshow("高斯滤波后图像", result);

	// 中值滤波
	cv::medianBlur(image, result,
		3);  // 滤波器尺寸，必须>1且为奇数
	cv::medianBlur(result, result,
		3);  // 滤波器尺寸，必须>1且为奇数
	cv::imshow("中值滤波后图像", result);

	cv::waitKey();
}