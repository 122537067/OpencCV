#pragma once
#include "ColorDetector.h"

class ColorDetectController
{
private:
	// �����㷨����
	ColorDetector *cdetect;

	cv::Mat image;		// �������ͼ��
	cv::Mat result;		// ���ͼ��

public:

	ColorDetectController()
	{
		// �����㷨�����
		cdetect = new ColorDetector();
	}

	~ColorDetectController()
	{
		delete cdetect;	// �ͷŶ�̬�������ʵ�����ڴ�
	}

	// ������ɫ������ֵ
	void setColorDistanceThreshold(int distance) {
		cdetect->setColorDistanceThreshold(distance);
	}

	// ȡ����ɫ������ֵ
	int getColorDistanceThreshold() const {
		return cdetect->getColorDistanceThreshold();
	}

	// ���ñ�������ɫ
	void setTargetColor(unsigned char red,
		unsigned char green, unsigned char blue) {
		cdetect->setTargetColor(blue, green, red);
	}

	// ȡ�ñ�������ɫ
	void getTargetColor(unsigned char &red,
		unsigned char &green, unsigned char &blue) const {
		cv::Vec3b color = cdetect->getTargetColor();

		red = color[2];
		green = color[1];
		blue = color[0];
	}

	// ��������ͼ�񣬴��ļ��ж�ȡ��
	bool setInputImage(std::string filename) {
		image = cv::imread(filename);
		return !image.empty();
	}

	// ���ص�ǰ������ͼ��
	const cv::Mat getInputImage() const {
		return image;
	}

	// ִ��ͼ����
	void process() {
		result = cdetect->process(image);
	}

	// ���������Ľ��ͼ��
	const cv::Mat getLastResult() const {
		return result;
	}
};

