#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// 重映射图像、创建波浪形效果
void wave(const cv::Mat &image, cv::Mat &result) {
	// 映射参数(给出结果图像中每个像素在源图像中的原始位置)
	cv::Mat srcX(image.rows, image.cols, CV_32F);
	cv::Mat srcY(image.rows, image.cols, CV_32F);

	// 创建映射参数
	for (int i = 0; i < image.rows; i++) {
		float* pSrcX = srcX.ptr<float>(i);
		float* pSrcY = srcY.ptr<float>(i);
		for (int j = 0; j < image.cols; j++) {
			//(i,j)像素的新位置
			pSrcX[j] = j; // 保持在同一列
			pSrcY[j] = i + 5 * sin(j / 10.0);
		}
	}

	// 应用映射参数
	cv::remap(image, result, srcX, srcY, cv::INTER_LINEAR);
}

// 水平翻转
void flip(const cv::Mat &image, cv::Mat &result) {
	// 映射参数(给出结果图像中每个像素在源图像中的原始位置)
	cv::Mat srcX(image.rows, image.cols, CV_32F);
	cv::Mat srcY(image.rows, image.cols, CV_32F);

	// 创建映射参数
	for (int i = 0; i < image.rows; i++) {
		float* pSrcX = srcX.ptr<float>(i);
		float* pSrcY = srcY.ptr<float>(i);
		for (int j = 0; j < image.cols; j++) {
			//(i,j)像素的新位置
			pSrcX[j] = image.cols - j - 1;
			pSrcY[j] = i;
		}
	}

	// 应用映射参数
	cv::remap(image, result, srcX, srcY, cv::INTER_LINEAR);
}

void main() {
	cv::Mat image = cv::imread("castle.jpg");
	cv::imshow("原图", image);

	cv::Mat result;
	wave(image, result);
	cv::imshow("波浪效果", result);

	flip(image, result);
	cv::imshow("水平翻转", result);

	cv::waitKey(0);
}