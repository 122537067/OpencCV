#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

void main()
{
	// ��ȡ����ʾԭͼ
	cv::Mat image = cv::imread("castle.jpg");
	cv::imshow("ԭͼ", image);

	// ת�Ҷ�ͼ
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	cv::imshow("�Ҷ�ͼ", gray);

	// ˮƽ����Sobel�˲�
	cv::Mat sobelX;
	cv::Sobel(gray,		// ����ͼ��
		sobelX,			// ���ͼ��
		CV_8U,			// ���ͼ���������
		1, 0,			// x��y����������
		3,				// �������ں˳ߴ磬����Ϊ����
		0.4, 128);		// ���ű�����ƫ����(Ĭ��ֵΪ1��0)
	cv::imshow("ˮƽ����Sobel�˲����(3��3)", sobelX);
	cv::waitKey();

	// ��ֱ����Sobel�˲�
	cv::Mat sobelY;
	cv::Sobel(gray,		// ����ͼ��
		sobelY,			// ���ͼ��
		CV_8U,			// ���ͼ���������
		0, 1,			// x��y����������
		3,				// �������ں˳ߴ磬����Ϊ����
		0.4, 128);		// ���ű�����ƫ����(Ĭ��ֵΪ1��0)
	cv::imshow("��ֱ����Sobel�˲����(3��3)", sobelY);
	cv::waitKey();

	// ����Sobel�˲�����ģ
	cv::Sobel(gray, sobelX, CV_16S, 1, 0);
	cv::Sobel(gray, sobelY, CV_16S, 0, 1);
	cv::Mat sobel;
	// ����L1ģ
	sobel = abs(sobelX) + abs(sobelY);
	// �ҵ�Sobel���������ֵ
	double sobmin, sobmax;
	cv::minMaxLoc(sobel, &sobmin, &sobmax);
	// ת����8λͼ��
	// sobelImage = -255. / sobmax * sobel + 255
	cv::Mat sobelL1Image;
	/*if (sobel.type() == CV_16S)
		double alpha = -255. / sobmax;*/
	sobel.convertTo(sobelL1Image, CV_8U, -255. / sobmax, 255);
	cv::imshow("Sobel�˲�����L1ģ", sobelL1Image);
	cv::waitKey();

	/* ����Sobel���ӵ�ģ�ͷ��� */
	// ����Sobel���ӣ������ø���������
	cv::Sobel(gray, sobelX, CV_32F, 1, 0);
	cv::Sobel(gray, sobelY, CV_32F, 0, 1);
	// �����ݶȵ�L2ģ�ͷ���
	cv::Mat norm, dir;
	cv::cartToPolar(sobelX, sobelY, norm, dir); // �ѿ�������ת������
	// �ҵ�Sobel���ֵ
	//double sobmin, sobmax;
	cv::minMaxLoc(norm, &sobmin, &sobmax);
	// ת����8λͼ��
	// sobelImage = -alpha * sobel + 255
	cv::Mat sobelL2Image;
	norm.convertTo(sobelL2Image, CV_8U, -255. / sobmax, 255);
	cv::imshow("Sobel�˲�����L2ģ", sobelL2Image);
	cv::waitKey();

	// ��ֵ��
	cv::Mat sobelThresholded;
	double threshold = 230;
	cv::threshold(sobelL1Image, sobelThresholded, threshold, 255, cv::THRESH_BINARY);
	cv::imshow("��ֵ�����", sobelThresholded);
	cv::waitKey();

	// ����ߴ��Sobel�˲�
	cv::Sobel(gray,		// ����ͼ��
		sobelX,			// ���ͼ��
		CV_8U,			// ���ͼ���������
		1, 0,			// x��y����������
		7,				// �������ں˳ߴ磬����Ϊ����
		0.002, 128);	// ���ű�����ƫ����(Ĭ��ֵΪ1��0)
	cv::imshow("ˮƽ����Sobel�˲����(7��7)", sobelX);

	cv::waitKey();
}