#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

void main()
{
	// ��ȡ����ͼ��
	cv::Mat image = cv::imread("binary.bmp");
	//image = cv::Scalar(255, 255, 255) - image;
	cv::imshow("ԭͼ", image);
	cv::waitKey();

	// ��ʴͼ��
	cv::Mat eroded;	// Ŀ��ͼ��
	// �ṹԪ�ش��վ����ʾʹ��Ĭ��3��3�����νṹԪ��
	cv::erode(image, eroded, cv::Mat());
	cv::imshow("��ʴ���", eroded);

	// ����ͼ��
	cv::Mat dilated;	// Ŀ��ͼ��
	// �ṹԪ�ش��վ����ʾʹ��Ĭ��3��3�����νṹԪ��
	cv::dilate(image, dilated, cv::Mat());
	cv::imshow("���ͽ��", dilated);
	cv::waitKey();

	// ʹ���Զ����7��7�ṹԪ��(Ԫ��ֵȫΪ1)
	cv::Mat element(7, 7, CV_8U, cv::Scalar(1));
	cv::erode(image, eroded, element);
	cv::imshow("7��7�ṹԪ�ظ�ʴ���", eroded);
	cv::waitKey();

	// ��ʴͼ������
	cv::erode(image, eroded, cv::Mat(), cv::Point(-1, -1), 3);
	cv::imshow("��ʴͼ�����ν��", eroded);
	cv::waitKey();

	// ������
	cv::Mat element5(5, 5, CV_8U, cv::Scalar(1));
	cv::Mat closed;
	cv::morphologyEx(image, closed, cv::MORPH_CLOSE, element5);
	cv::imshow("��������", closed);

	// ������
	cv::Mat opened;
	cv::morphologyEx(image, opened, cv::MORPH_OPEN, element5);
	cv::imshow("��������", opened);

	cv::waitKey();
}