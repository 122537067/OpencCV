#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// ��ӳ��ͼ�񡢴���������Ч��
void wave(const cv::Mat &image, cv::Mat &result) {
	// ӳ�����(�������ͼ����ÿ��������Դͼ���е�ԭʼλ��)
	cv::Mat srcX(image.rows, image.cols, CV_32F);
	cv::Mat srcY(image.rows, image.cols, CV_32F);

	// ����ӳ�����
	for (int i = 0; i < image.rows; i++) {
		float* pSrcX = srcX.ptr<float>(i);
		float* pSrcY = srcY.ptr<float>(i);
		for (int j = 0; j < image.cols; j++) {
			//(i,j)���ص���λ��
			pSrcX[j] = j; // ������ͬһ��
			pSrcY[j] = i + 5 * sin(j / 10.0);
		}
	}

	// Ӧ��ӳ�����
	cv::remap(image, result, srcX, srcY, cv::INTER_LINEAR);
}

// ˮƽ��ת
void flip(const cv::Mat &image, cv::Mat &result) {
	// ӳ�����(�������ͼ����ÿ��������Դͼ���е�ԭʼλ��)
	cv::Mat srcX(image.rows, image.cols, CV_32F);
	cv::Mat srcY(image.rows, image.cols, CV_32F);

	// ����ӳ�����
	for (int i = 0; i < image.rows; i++) {
		float* pSrcX = srcX.ptr<float>(i);
		float* pSrcY = srcY.ptr<float>(i);
		for (int j = 0; j < image.cols; j++) {
			//(i,j)���ص���λ��
			pSrcX[j] = image.cols - j - 1;
			pSrcY[j] = i;
		}
	}

	// Ӧ��ӳ�����
	cv::remap(image, result, srcX, srcY, cv::INTER_LINEAR);
}

void main() {
	cv::Mat image = cv::imread("castle.jpg");
	cv::imshow("ԭͼ", image);

	cv::Mat result;
	wave(image, result);
	cv::imshow("����Ч��", result);

	flip(image, result);
	cv::imshow("ˮƽ��ת", result);

	cv::waitKey(0);
}