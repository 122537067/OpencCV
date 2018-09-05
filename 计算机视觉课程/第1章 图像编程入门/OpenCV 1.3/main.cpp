#include <opencv2/core/core.hpp>  // 可省
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

// 鼠标事件回调函数
void onMouse(int event, int x, int y, int flags, void* param) {
	cv::Mat *im = reinterpret_cast<cv::Mat*>(param);

	switch (event) { // 什么鼠标事件？
	case CV_EVENT_LBUTTONDOWN:	// 鼠标左键按下事件
								// 输出像素值(x, y)
		std::cout << "坐标 (" << x << "," << y << ") 处像素值为: "
			<< static_cast<int>(im->at <uchar>(cv::Point(x, y))) << std::endl;
		break;
	}
}

void main()
{
	cv::Mat image;	// 创建一个空图像(尺寸为0×0)

	// 验证该图像的大小
	std::cout << "图像大小为 " << image.rows << " x "
		<< image.cols << std::endl;

	//image = cv::imread("puppy.jpg");	// 读取输入图像

	// 读入一个图像文件并转换为灰度图像
	//image = cv::imread("puppy.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	// 读取图像，并转换为三通道彩色图像
	//image = cv::imread("puppy.jpg", CV_LOAD_IMAGE_COLOR);  // 默认情况

	// 读入图像，使用文件本身格式
	image = cv::imread("puppy.jpg", CV_LOAD_IMAGE_UNCHANGED);

	if (image.empty()) {	// 图像为空？
		std::cout << "读取图像失败!" << std::endl;
		exit(EXIT_FAILURE); // 退出程序
	}

	std::cout << "该图像有 " << image.channels() << " 通道" << std::endl;

	// 创建窗口(可选)
	cv::namedWindow("原始图像");

	// 显示图像
	cv::imshow("原始图像", image);

	cv::Mat result;	// 创建另一个空的图像
	// 水平翻转图像
	cv::flip(image,	// 输入图像 
		result,		// 输出图像
		1);			// 正数表示水平翻转，0表示垂直翻转，负数表示水平和垂直同时翻转

	// 就地处理
	//cv::flip(image, image, 1);

	// 设置鼠标回调函数
	cv::setMouseCallback("原始图像", onMouse, reinterpret_cast<void*>(&image));

	// 在另一个窗口显示结果
	cv::namedWindow("输出图像"); // 输出窗口
	cv::imshow("输出图像", result);

	cv::circle(image,  // 目标图像
		cv::Point(386, 325),  // 中心点坐标
		300,  // 半径
		0,  // 颜色(这里用黑色)
		3); // 线的粗细

	cv::putText(image,			// 目标图像
		"I am not a dog!",		// 文本
		cv::Point(600, 592),	// 文本位置
		cv::FONT_HERSHEY_PLAIN,	// 字体类型
		2.0,					// 字体大小
		255,					// 字体颜色(这里用白色，对于彩色图像则是蓝色)
		2);						// 文本粗细
	cv::imshow("原始图像", image);

	// 按键后退出
	cv::waitKey(0); // 0表示永远地等待按键，正数表示等待指定的毫秒数

	cv::imwrite("output.bmp", result);	// 将处理后图像保存起来
}