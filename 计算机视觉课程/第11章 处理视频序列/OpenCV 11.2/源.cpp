#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\tracking.hpp>
#include <iostream>

int main()
{
	// ����Ƶ�ļ�
	//cv::VideoCapture capture("bike.avi");
	cv::VideoCapture capture(0);
	//cv::VideoCapture capture;
	//capture.open("test.mp4");

	// �����Ƿ�ɹ�
	if (!capture.isOpened())
		return EXIT_FAILURE;

	// ȡ��֡����,getʼ�շ���double������ֵ
	double rate = capture.get(CV_CAP_PROP_FPS);
	if (rate == 0) rate = 60;
	std::cout << "֡���ʣ�" << rate << std::endl;
	// �����֡��
	long t = static_cast<long>(
		capture.get(CV_CAP_PROP_FRAME_COUNT));
	std::cout << "��֡����" << t << std::endl;
	// �����Ƶͼ��ߴ�
	int width = static_cast<int>(
		capture.get(CV_CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(
		capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	std::cout << "ͼ��ߴ磺" << width 
		<< " �� " << height << std::endl;

	// ��ת����100֡
	//double position = 100.0;
	//capture.set(CV_CAP_PROP_POS_FRAMES, position);

	bool stop(false); // ���ڿ��ƺ����whileѭ��
	cv::Mat frame;	  // �洢��ǰ��Ƶ֡

	// ����֡���ʼ���֮֡��ĵȴ�ʱ�䣬��λms
	// ��ͨ���޸Ĵ���ֵ������Ƶ��������
	// ʱ��Ҫ��֤�������㹻��ʱ�����ˢ��
	int delay = 1000 / rate;

	// ѭ��������Ƶ�е�ȫ��֡
	while (!stop) {
		// ��ȡ��һ֡(�����)
		if (!capture.read(frame))
			break;

		cv::imshow("Extracted Frame", frame);

		// �ȴ�һ��ʱ�䣬����ͨ������ֹͣ
		// ����ǰ���ֹͣ���򷵻ذ���ASCII��(>=0)
		if (cv::waitKey(delay) >= 0)
			stop = true;
	}

	// �ر���Ƶ�ļ�
	// ���Ǳ���ģ���Ϊ����������������
	capture.release();
	return EXIT_SUCCESS;
}