#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

void main()
{
	// ��ȡ����ͼ��
	cv::Mat image = cv::imread("castle.jpg");
	cv::imshow("ԭͼ", image);

	// ƽ���˲�
	cv::Mat result;
	//cv::blur(image, result,
	//	cv::Size(5, 5));	// �˲����ߴ�
	cv::boxFilter(image, result, CV_8U, cv::Size(5, 5));
	cv::imshow("ƽ����ͼ��", result);

	// ��˹ƽ���˲�
	cv::GaussianBlur(image, result, cv::Size(5, 5), 0);
	cv::imshow("��˹ƽ����ͼ��", result);

	cv::waitKey();
}