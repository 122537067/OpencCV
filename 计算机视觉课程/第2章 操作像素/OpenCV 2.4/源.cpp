#include <opencv2\highgui\highgui.hpp>

// ����ͼ��image����ɫ������ÿ��ͨ������ɫֵ����������ԭ����1/div
void colorReduce(cv::Mat image, int div = 64) {
	// �ڳ�ʼλ�û�õ�����
	cv::MatIterator_<cv::Vec3b> it =
		image.begin<cv::Vec3b>();
	// ��ý���λ��
	cv::MatIterator_<cv::Vec3b> itend =
		image.end<cv::Vec3b>();

	// ������������
	for (; it != itend; ++it) { 
		// ����ÿ������-------------------------
		(*it)[0] = (*it)[0] / div * div + div / 2;
		(*it)[1] = (*it)[1] / div * div + div / 2;
		(*it)[2] = (*it)[2] / div * div + div / 2;
		// ���ش������-------------------------
	}
}

void main()
{
	// ��ȡͼ��
	cv::Mat image = cv::imread("castle.jpg");

	// ����ͼ��
	colorReduce(image, 64);

	// ��ʾͼ��
	cv::imshow("������ɫ����", image);

	// �ȴ�����
	cv::waitKey(0);
}