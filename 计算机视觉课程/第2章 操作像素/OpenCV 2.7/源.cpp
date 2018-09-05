#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

void main()
{
	cv::Mat image1 = cv::imread("castle.jpg"); // ��ȡͼ��
	cv::imshow("ͼ��1", image1);

	cv::Mat image2 = cv::imread("rain.jpg"); // ��ȡ��һͼ��(��С�ú�image1һ��)
	cv::imshow("ͼ��2", image2);

	cv::resize(image2, image2, image1.size());
	
	cv::Mat result;	// ���ͼ��
	// result[i] = image1[i] + image2[i]
	//cv::add(image1, image2, result);	// ͼ�����

	// result[i] = image1[i] + cv::Scalar(-50, -50, -50)
	//cv::add(image1, cv::Scalar(-50, -50, -50), result); // ����ͼ������

	//result[i] = image1[i] * 0.4 + image2[i] * 0.6 + 0;
	//cv::addWeighted(image1, 0.4, image2, 0.6, 0, result); // ��Ȩ��

	//result = image1 * 0.4 + image2 * 0.6;

	/*uchar mask = 0xFF << 6;
	int div = pow(2, 6);
	result = (image1 & cv::Scalar(mask, mask, mask)) 
		+ cv::Scalar(div / 2, div / 2, div / 2);*/

	// ������������ͼ�������
	std::vector<cv::Mat> planes;
	// �ָ�һ����ͨ��ͼ��Ϊ������ͨ��ͼ��
	cv::split(image1, planes);
	// ��ɫͨ������ֵ����
	planes[2] += 50;
	// �ϲ�������ͨ��ͼ��Ϊһ����ͨ��ͼ��
	cv::merge(planes, result);

	cv::imshow("Result", result);

	cv::waitKey(0);	// �ȴ�����
}