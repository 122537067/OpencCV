#include <opencv2/core/core.hpp>  // ��ʡ
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

// ����¼��ص�����
void onMouse(int event, int x, int y, int flags, void* param) {
	cv::Mat *im = reinterpret_cast<cv::Mat*>(param);

	switch (event) { // ʲô����¼���
	case CV_EVENT_LBUTTONDOWN:	// �����������¼�
								// �������ֵ(x, y)
		std::cout << "���� (" << x << "," << y << ") ������ֵΪ: "
			<< static_cast<int>(im->at <uchar>(cv::Point(x, y))) << std::endl;
		break;
	}
}

void main()
{
	cv::Mat image;	// ����һ����ͼ��(�ߴ�Ϊ0��0)

	// ��֤��ͼ��Ĵ�С
	std::cout << "ͼ���СΪ " << image.rows << " x "
		<< image.cols << std::endl;

	//image = cv::imread("puppy.jpg");	// ��ȡ����ͼ��

	// ����һ��ͼ���ļ���ת��Ϊ�Ҷ�ͼ��
	//image = cv::imread("puppy.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	// ��ȡͼ�񣬲�ת��Ϊ��ͨ����ɫͼ��
	//image = cv::imread("puppy.jpg", CV_LOAD_IMAGE_COLOR);  // Ĭ�����

	// ����ͼ��ʹ���ļ������ʽ
	image = cv::imread("puppy.jpg", CV_LOAD_IMAGE_UNCHANGED);

	if (image.empty()) {	// ͼ��Ϊ�գ�
		std::cout << "��ȡͼ��ʧ��!" << std::endl;
		exit(EXIT_FAILURE); // �˳�����
	}

	std::cout << "��ͼ���� " << image.channels() << " ͨ��" << std::endl;

	// ��������(��ѡ)
	cv::namedWindow("ԭʼͼ��");

	// ��ʾͼ��
	cv::imshow("ԭʼͼ��", image);

	cv::Mat result;	// ������һ���յ�ͼ��
	// ˮƽ��תͼ��
	cv::flip(image,	// ����ͼ�� 
		result,		// ���ͼ��
		1);			// ������ʾˮƽ��ת��0��ʾ��ֱ��ת��������ʾˮƽ�ʹ�ֱͬʱ��ת

	// �͵ش���
	//cv::flip(image, image, 1);

	// �������ص�����
	cv::setMouseCallback("ԭʼͼ��", onMouse, reinterpret_cast<void*>(&image));

	// ����һ��������ʾ���
	cv::namedWindow("���ͼ��"); // �������
	cv::imshow("���ͼ��", result);

	cv::circle(image,  // Ŀ��ͼ��
		cv::Point(386, 325),  // ���ĵ�����
		300,  // �뾶
		0,  // ��ɫ(�����ú�ɫ)
		3); // �ߵĴ�ϸ

	cv::putText(image,			// Ŀ��ͼ��
		"I am not a dog!",		// �ı�
		cv::Point(600, 592),	// �ı�λ��
		cv::FONT_HERSHEY_PLAIN,	// ��������
		2.0,					// �����С
		255,					// ������ɫ(�����ð�ɫ�����ڲ�ɫͼ��������ɫ)
		2);						// �ı���ϸ
	cv::imshow("ԭʼͼ��", image);

	// �������˳�
	cv::waitKey(0); // 0��ʾ��Զ�صȴ�������������ʾ�ȴ�ָ���ĺ�����

	cv::imwrite("output.bmp", result);	// �������ͼ�񱣴�����
}