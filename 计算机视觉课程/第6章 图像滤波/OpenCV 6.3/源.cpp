#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

void main()
{
	// ��ȡ����ͼ��
	cv::Mat image = cv::imread("castleWithNoises.jpg");
	cv::imshow("������ͼ��", image);

	cv::Mat result;
	cv::blur(image, result, cv::Size(5, 5));
	cv::imshow("��ֵ�˲���ͼ��", result);

	cv::GaussianBlur(image, result, cv::Size(7, 7), 0);
	cv::imshow("��˹�˲���ͼ��", result);

	// ��ֵ�˲�
	cv::medianBlur(image, result,
		3);  // �˲����ߴ磬����>1��Ϊ����
	cv::medianBlur(result, result,
		3);  // �˲����ߴ磬����>1��Ϊ����
	cv::imshow("��ֵ�˲���ͼ��", result);

	cv::waitKey();
}