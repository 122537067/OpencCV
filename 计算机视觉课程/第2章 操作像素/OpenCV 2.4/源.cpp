#include <opencv2\highgui\highgui.hpp>

// 削减图像image的颜色数量，每个通道的颜色值数量削减到原来的1/div
void colorReduce(cv::Mat image, int div = 64) {
	// 在初始位置获得迭代器
	cv::MatIterator_<cv::Vec3b> it =
		image.begin<cv::Vec3b>();
	// 获得结束位置
	cv::MatIterator_<cv::Vec3b> itend =
		image.end<cv::Vec3b>();

	// 遍历所有像素
	for (; it != itend; ++it) { 
		// 处理每个像素-------------------------
		(*it)[0] = (*it)[0] / div * div + div / 2;
		(*it)[1] = (*it)[1] / div * div + div / 2;
		(*it)[2] = (*it)[2] / div * div + div / 2;
		// 像素处理结束-------------------------
	}
}

void main()
{
	// 读取图像
	cv::Mat image = cv::imread("castle.jpg");

	// 处理图像
	colorReduce(image, 64);

	// 显示图像
	cv::imshow("削减颜色数量", image);

	// 等待按键
	cv::waitKey(0);
}