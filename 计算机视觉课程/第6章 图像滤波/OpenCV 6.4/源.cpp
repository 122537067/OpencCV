#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

void main()
{
	// 读取并显示原图
	cv::Mat image = cv::imread("castle.jpg");
	cv::imshow("原图", image);

	// 转灰度图
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	cv::imshow("灰度图", gray);

	// 水平方向Sobel滤波
	cv::Mat sobelX;
	cv::Sobel(gray,		// 输入图像
		sobelX,			// 输出图像
		CV_8U,			// 输出图像像素深度
		1, 0,			// x和y方向导数阶数
		3,				// 正方形内核尺寸，必须为奇数
		0.4, 128);		// 缩放比例和偏移量(默认值为1和0)
	cv::imshow("水平方向Sobel滤波结果(3×3)", sobelX);
	cv::waitKey();

	// 垂直方向Sobel滤波
	cv::Mat sobelY;
	cv::Sobel(gray,		// 输入图像
		sobelY,			// 输出图像
		CV_8U,			// 输出图像像素深度
		0, 1,			// x和y方向导数阶数
		3,				// 正方形内核尺寸，必须为奇数
		0.4, 128);		// 缩放比例和偏移量(默认值为1和0)
	cv::imshow("垂直方向Sobel滤波结果(3×3)", sobelY);
	cv::waitKey();

	// 计算Sobel滤波器的模
	cv::Sobel(gray, sobelX, CV_16S, 1, 0);
	cv::Sobel(gray, sobelY, CV_16S, 0, 1);
	cv::Mat sobel;
	// 计算L1模
	sobel = abs(sobelX) + abs(sobelY);
	// 找到Sobel中像素最大值
	double sobmin, sobmax;
	cv::minMaxLoc(sobel, &sobmin, &sobmax);
	// 转换成8位图像
	// sobelImage = -255. / sobmax * sobel + 255
	cv::Mat sobelL1Image;
	/*if (sobel.type() == CV_16S)
		double alpha = -255. / sobmax;*/
	sobel.convertTo(sobelL1Image, CV_8U, -255. / sobmax, 255);
	cv::imshow("Sobel滤波器的L1模", sobelL1Image);
	cv::waitKey();

	/* 计算Sobel算子的模和方向 */
	// 计算Sobel算子，必须用浮点数类型
	cv::Sobel(gray, sobelX, CV_32F, 1, 0);
	cv::Sobel(gray, sobelY, CV_32F, 0, 1);
	// 计算梯度的L2模和方向
	cv::Mat norm, dir;
	cv::cartToPolar(sobelX, sobelY, norm, dir); // 笛卡尔坐标转极坐标
	// 找到Sobel最大值
	//double sobmin, sobmax;
	cv::minMaxLoc(norm, &sobmin, &sobmax);
	// 转换成8位图像
	// sobelImage = -alpha * sobel + 255
	cv::Mat sobelL2Image;
	norm.convertTo(sobelL2Image, CV_8U, -255. / sobmax, 255);
	cv::imshow("Sobel滤波器的L2模", sobelL2Image);
	cv::waitKey();

	// 阈值化
	cv::Mat sobelThresholded;
	double threshold = 230;
	cv::threshold(sobelL1Image, sobelThresholded, threshold, 255, cv::THRESH_BINARY);
	cv::imshow("阈值化结果", sobelThresholded);
	cv::waitKey();

	// 更大尺寸的Sobel滤波
	cv::Sobel(gray,		// 输入图像
		sobelX,			// 输出图像
		CV_8U,			// 输出图像像素深度
		1, 0,			// x和y方向导数阶数
		7,				// 正方形内核尺寸，必须为奇数
		0.002, 128);	// 缩放比例和偏移量(默认值为1和0)
	cv::imshow("水平方向Sobel滤波结果(7×7)", sobelX);

	cv::waitKey();
}