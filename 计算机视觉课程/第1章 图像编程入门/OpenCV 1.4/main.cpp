#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>

// ���Ժ�����������һ��ͼ��
cv::Mat function() {
	// ����ͼ��
	cv::Mat ima(500, 500, CV_8U, 50);
	// ����ͼ��
	return ima;
}

int main() {
	// ����ͼ�񴰿�
	/*cv::namedWindow("Image 1");
	cv::namedWindow("Image 2");
	cv::namedWindow("Image 3");
	cv::namedWindow("Image 4");
	cv::namedWindow("Image 5");
	cv::namedWindow("Image");*/

	// ����һ��240�С�320�е���ͼ��
	cv::Mat image1(240, 320, CV_8U, 100);
	cv::imshow("Image", image1);	// ��ʾͼ��
	cv::waitKey(0);		// �ȴ�����
	
	// ���·���һ���µ�ͼ��(���ڴ�С�����Ͳ�ͬʱ)
	image1.create(200, 200, CV_8U);
	image1 = 200; // ������������ֵΪ200

	cv::imshow("Image", image1);	// ��ʾͼ��
	cv::waitKey(0);		// �ȴ�����

	// ����һ����ɫ��ͼ��
	// ͨ������ΪBGR
	cv::Mat image2(240, 320, CV_8UC3, cv::Scalar(0, 0, 255));

	// ���ߣ�
	//cv::Mat image2(cv::Size(320, 240), CV_8UC3);
	//image2 = cv::Scalar(0, 0, 255);

	cv::imshow("Image", image2);	// ��ʾͼ��
	cv::waitKey(0);		// �ȴ�����

	// ����һ��ͼ��
	cv::Mat image3 = cv::imread("puppy.jpg");

	// ������Щͼ��ָ��ͬһ�����ݿ�
	cv::Mat image4(image3);
	image1 = image3;

	// ��Щͼ����ԭͼ��ĸ���ͼ��
	image3.copyTo(image2);
	cv::Mat image5 = image3.clone();

	// ת��ͼ����������
	cv::flip(image3, image3, 1);

	// �����Щͼ���ڴ���������ܵ���Ӱ��
	cv::imshow("Image 1", image1);
	cv::imshow("Image 2", image2);
	cv::imshow("Image 3", image3);
	cv::imshow("Image 4", image4);
	cv::imshow("Image 5", image5);
	cv::waitKey(0);		// �ȴ�����

	// �Ӻ����л�ȡһ���Ҷ�ͼ��
	cv::Mat gray = function();
	cv::imshow("Image", gray); // ��ʾͼ��
	cv::waitKey(0);	// �ȴ�����

	// ��Ϊ�Ҷ�ͼ�����
	image1 = cv::imread("puppy.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	image1.convertTo(image2, CV_32F, 1/255.0, 0.0);
	std::cout << image2 << std::endl;
	cv::imshow("Image", image2); // ��ʾͼ��
	cv::waitKey(0);	// �ȴ�����

	return 0;
}