#include <opencv2\highgui\highgui.hpp>

// Ϊͼ��image�����������nΪ������������
void salt(cv::Mat image, int n)
{
	int i, j; 
	for (int k = 0; k < n; k++) {
		// rand()�������������
		i = rand() % image.cols;
		j = rand() % image.rows;

		if (image.type() == CV_8UC1)  // �Ҷ�ͼ��(CV_8UC1��CV_8Uֵ���)
			image.at<uchar>(j, i) = 255;  // ������ֵ��Ϊ��ɫ
		else if (image.type() == CV_8UC3) { // ��ɫͼ��
			// ������ֵ��Ϊ��ɫ
			//image.at<cv::Vec3b>(j, i)  [0] = 255;	 // ��ɫͨ������ֵ
			//image.at<cv::Vec3b>(j, i)[1] = 255;	 // ��ɫͨ������ֵ
			//image.at<cv::Vec3b>(j, i)[2] = 255;	 // ��ɫͨ������ֵ
			image.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255);
		}
	}
}

// ��Բ�ɫͼ��ļ򻯰汾
void salt(cv::Mat_<cv::Vec3b> image, int n)
{
	int i, j;
	for (int k = 0; k < n; k++) {
		// rand()�������������
		i = rand() % image.cols;  // ����к�
		j = rand() % image.rows;  // ����к�
		
		// ������ֵ��Ϊ��ɫ
		//image(j, i)[0] = 255;	 // ��ɫͨ������ֵ
		//image(j, i)[1] = 255;	 // ��ɫͨ������ֵ
		//image(j, i)[2] = 255;	 // ��ɫͨ������ֵ
		image(j, i) = cv::Vec3b(255, 255, 255);
	}
}

void main()
{
	// ��ͼ��
	cv::Mat image = cv::imread("castle.jpg");
	//cv::Mat_<cv::Vec3b> image = cv::imread("castle.jpg");

	// ���ú��������������
	salt(image, 30000);

	// ��ʾͼ��
	cv::imshow("���������", image);
	//cv::imwrite("castleWithNoises.jpg", image);

	// �ȴ�����
	cv::waitKey(0);
}