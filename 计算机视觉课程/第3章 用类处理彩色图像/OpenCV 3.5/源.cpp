#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

void detectHScolor(const cv::Mat &image,	// ����ͼ��
	double minHue, double maxHue,			// ɫ������
	double minSat, double maxSat,			// ���Ͷ�����
	cv::Mat &mask) {						// �������
	// ת����HSV�ռ�
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// �ָ�3��ͨ���������3��ͼ��
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);
	// channels[0]��ɫ��
	// channels[1]�Ǳ��Ͷ�
	// channels[2]������

	// ɫ������
	cv::Mat mask1;	// С��maxHue
	// channels[0]������ֵС��maxHue��mask1�ж�Ӧ����ֵ��255
	cv::threshold(channels[0], mask1, maxHue, 255, 
		cv::THRESH_BINARY_INV);
	cv::Mat mask2;	// ����minHue
	// channels[0]������ֵС��minHue��mask2�ж�Ӧ����ֵ��255
	cv::threshold(channels[0], mask2, minHue, 255,
		cv::THRESH_BINARY);

	cv::Mat hueMask;	// ɫ������
	if (minHue < maxHue)
		hueMask = mask1 & mask2; // ��λ��
	else // ������䴩Խ0��������
		hueMask = mask1 | mask2; // ��λ��

	// ���Ͷ�����
	// channels[1]������ֵС��maxSat��mask1�ж�Ӧ����ֵ��255
	cv::threshold(channels[1], mask1, maxSat, 255,
		cv::THRESH_BINARY_INV);
	// channels[1]������ֵ����minSat��mask2�ж�Ӧ����ֵ��255
	cv::threshold(channels[1], mask2, minSat, 255,
		cv::THRESH_BINARY);

	cv::Mat satMask;	// ���Ͷ�����
	satMask = mask1 & mask2;

	// �������
	mask = hueMask & satMask;
}


// ��YCrCb�ռ�����ԲƤ��ģ�ͽ��з�ɫ���
void detectSkinColor(const cv::Mat &image,	// ����ͼ��
	cv::Mat &mask) {						// �������
	
	/*��ʼ����ԲƤ��ģ��*/
	// ���ڷ�ɫ��������ͼ��256��256��С����ʼΪȫ0
	cv::Mat skinCrCb(cv::Size(256, 256), CV_8UC1, cv::Scalar(0));
	// ������ͼ���л���һ��ɫ��Բ
	cv::ellipse(skinCrCb, // Ŀ��ͼ��
		cv::Point(113, 155.6), // ��Բ��������
		cv::Size(23.4, 15.2),  // ��Բ������ĳ���
		43.0,				   // ��Բ������ˮƽ����н�
		0.0,				   // ��ʼ�Ƕ�
		360.0,                 // ��ֹ�Ƕ�
		cv::Scalar(255),	   // ��ԲΪ��ɫ
		-1					   // �߿��ȣ�Ϊ������ʾ���������Բ
	);
	cv::imshow("��Բģ��", skinCrCb);
	
	// ת����YCrCb�ռ�
	cv::Mat matYCrCb;
	cv::cvtColor(image, matYCrCb, CV_BGR2YCrCb);

	// �б�Ҫ�Ļ����·���mask�����ݿռ�
	mask.create(image.size(), CV_8UC1);

	// ����ͼ�񣬼��ÿ��������ɫ�Ƿ�λ����Բ�ڲ�
	for (int j = 0; j < matYCrCb.rows; j++) {
		cv::Vec3b *input = matYCrCb.ptr<cv::Vec3b>(j);
		uchar *output = mask.ptr<uchar>(j);
		for (int i = 0; i < matYCrCb.cols; i++) {
			// �����ص�CrCbֵ��Ϊ�������skinCrCbͼ������Բ�ڲ�������Ƿ�ɫ
			if (skinCrCb.at<uchar>(input[i][1], input[i][2]) > 0)
				output[i] = 255;
			else
				output[i] = 0;
		}
	}
	cv::imshow("mask", mask);
}

void AdjustSaturation(const cv::Mat &image, cv::Mat &result, float s)
{
	result.create(image.size(), image.type());

	for (int row = 0; row < image.rows; row++) {
		const cv::Vec3b *input = image.ptr<cv::Vec3b>(row);
		cv::Vec3b *output = result.ptr<cv::Vec3b>(row);
		for (int col = 0; col < image.cols; col++) {
			uchar Y = input[col][0] * 0.114 +
				input[col][1] * 0.587 +
				input[col][2] * 0.299;
			output[col] = cv::Vec3b(cv::saturate_cast<uchar>(Y + s * (input[col][0] - Y)), 
				cv::saturate_cast<uchar>(Y + s * (input[col][1] - Y)),
				cv::saturate_cast<uchar>(Y + s * (input[col][2] - Y)));
		}
	}
}

void main()
{
	cv::Mat image = cv::imread("castle.jpg");
	if (image.empty())  // ͼ��Ϊ��?
		return;
	cv::imshow("ԭͼ", image);

	// ת����HSVɫ�ʿռ�
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// ������ͨ���ָ������ͼ����
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);

	// channels[0]��ɫ��
	cv::imshow("ɫ��", channels[0]);
	// channels[1]�Ǳ��Ͷ�
	cv::imshow("���Ͷ�", channels[1]);
	// channels[2]������
	cv::imshow("����", channels[2]);

	// �������ص�����ͨ����Ϊ255
	channels[2] = 255;
	// ���ºϲ�ͨ��
	cv::merge(channels, hsv);
	// ת����BGR
	cv::Mat newImage;
	cv::cvtColor(hsv, newImage, CV_HSV2BGR);
	cv::imshow("������ͳһ��Ϊ255���ͼ��", newImage);

	//cv::Mat image = cv::imread("girl.jpg");
	//cv::imshow("ԭͼ", image);

	//// ����ɫ
	//cv::Mat mask;
	////detectHScolor(image,
	////	160, 10, // ɫ����320�ȵ�20��
	////	25, 166, // ���Ͷȴ�0.1��0.65
	////	mask);
	//detectSkinColor(image, mask);

	//// ��ʾʹ��������ͼ��
	//// ����ֵ��ʼ��Ϊȫ��
	//cv::Mat detected(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	//image.copyTo(detected, mask); // ������maskΪ��ɫ�Ĳ���
	//cv::imshow("��ɫ�����", detected);

	//cv::Mat result;
	//AdjustSaturation(image, result, 0.5);
	//imshow("���Ͷȵ��ڽ��", result);

	cv::waitKey(); // �ȴ�����
}