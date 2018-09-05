#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

void main()
{
	// ��ȡͼ��
	cv::Mat image = cv::imread("road.jpg");
	if (image.empty()) {
		std::cout << "ͼ���ȡʧ��!" << std::endl;
		exit(EXIT_FAILURE);
	}
	cv::imshow("ԭʼͼ��", image);

	// ת�Ҷ�ͼ
	cv::Mat gray;
	cv::cvtColor(image, gray, CV_BGR2GRAY);
	cv::imshow("�Ҷ�ͼ", gray);

	// Ӧ��Canny�㷨
	double low = 350;		// ����ֵ
	double high = 1250;		// ����ֵ

	cv::Mat contours;
	cv::Canny(gray,// �Ҷ�ͼ��
		contours,	// �������ͼ
		low,		// ����ֵ
		high);		// ����ֵ
	cv::imshow("Canny����", contours);

	cv::waitKey();
}