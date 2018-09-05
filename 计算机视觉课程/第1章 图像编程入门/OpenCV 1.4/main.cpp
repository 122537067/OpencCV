#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>

// 测试函数，它创建一幅图像
cv::Mat function() {
	// 创建图像
	cv::Mat ima(500, 500, CV_8U, 50);
	// 返回图像
	return ima;
}

int main() {
	// 定义图像窗口
	/*cv::namedWindow("Image 1");
	cv::namedWindow("Image 2");
	cv::namedWindow("Image 3");
	cv::namedWindow("Image 4");
	cv::namedWindow("Image 5");
	cv::namedWindow("Image");*/

	// 创建一个240行×320列的新图像
	cv::Mat image1(240, 320, CV_8U, 100);
	cv::imshow("Image", image1);	// 显示图像
	cv::waitKey(0);		// 等待按键
	
	// 重新分配一幅新的图像(仅在大小或类型不同时)
	image1.create(200, 200, CV_8U);
	image1 = 200; // 设置所有像素值为200

	cv::imshow("Image", image1);	// 显示图像
	cv::waitKey(0);		// 等待按键

	// 创建一幅红色的图像
	// 通道次序为BGR
	cv::Mat image2(240, 320, CV_8UC3, cv::Scalar(0, 0, 255));

	// 或者：
	//cv::Mat image2(cv::Size(320, 240), CV_8UC3);
	//image2 = cv::Scalar(0, 0, 255);

	cv::imshow("Image", image2);	// 显示图像
	cv::waitKey(0);		// 等待按键

	// 读入一幅图像
	cv::Mat image3 = cv::imread("puppy.jpg");

	// 所有这些图像都指向同一个数据块
	cv::Mat image4(image3);
	image1 = image3;

	// 这些图像是原图像的副本图像
	image3.copyTo(image2);
	cv::Mat image5 = image3.clone();

	// 转换图像用来测试
	cv::flip(image3, image3, 1);

	// 检查哪些图像在处理过程中受到了影响
	cv::imshow("Image 1", image1);
	cv::imshow("Image 2", image2);
	cv::imshow("Image 3", image3);
	cv::imshow("Image 4", image4);
	cv::imshow("Image 5", image5);
	cv::waitKey(0);		// 等待按键

	// 从函数中获取一幅灰度图像
	cv::Mat gray = function();
	cv::imshow("Image", gray); // 显示图像
	cv::waitKey(0);	// 等待按键

	// 作为灰度图像读入
	image1 = cv::imread("puppy.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	image1.convertTo(image2, CV_32F, 1/255.0, 0.0);
	std::cout << image2 << std::endl;
	cv::imshow("Image", image2); // 显示图像
	cv::waitKey(0);	// 等待按键

	return 0;
}