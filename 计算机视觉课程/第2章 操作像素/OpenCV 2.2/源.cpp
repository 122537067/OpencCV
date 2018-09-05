#include <opencv2\highgui\highgui.hpp>

// 为图像image添加盐噪声，n为盐噪声的数量
void salt(cv::Mat image, int n)
{
	int i, j; 
	for (int k = 0; k < n; k++) {
		// rand()是随机数生成器
		i = rand() % image.cols;
		j = rand() % image.rows;

		if (image.type() == CV_8UC1)  // 灰度图像(CV_8UC1和CV_8U值相等)
			image.at<uchar>(j, i) = 255;  // 将像素值设为白色
		else if (image.type() == CV_8UC3) { // 彩色图像
			// 将像素值设为白色
			//image.at<cv::Vec3b>(j, i)  [0] = 255;	 // 蓝色通道像素值
			//image.at<cv::Vec3b>(j, i)[1] = 255;	 // 绿色通道像素值
			//image.at<cv::Vec3b>(j, i)[2] = 255;	 // 红色通道像素值
			image.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255);
		}
	}
}

// 针对彩色图像的简化版本
void salt(cv::Mat_<cv::Vec3b> image, int n)
{
	int i, j;
	for (int k = 0; k < n; k++) {
		// rand()是随机数生成器
		i = rand() % image.cols;  // 随机列号
		j = rand() % image.rows;  // 随机行号
		
		// 将像素值设为白色
		//image(j, i)[0] = 255;	 // 蓝色通道像素值
		//image(j, i)[1] = 255;	 // 绿色通道像素值
		//image(j, i)[2] = 255;	 // 红色通道像素值
		image(j, i) = cv::Vec3b(255, 255, 255);
	}
}

void main()
{
	// 打开图像
	cv::Mat image = cv::imread("castle.jpg");
	//cv::Mat_<cv::Vec3b> image = cv::imread("castle.jpg");

	// 调用函数以添加盐噪声
	salt(image, 30000);

	// 显示图像
	cv::imshow("添加盐噪声", image);
	//cv::imwrite("castleWithNoises.jpg", image);

	// 等待按键
	cv::waitKey(0);
}