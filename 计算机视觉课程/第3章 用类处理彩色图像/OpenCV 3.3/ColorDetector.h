#pragma once
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class ColorDetector
{
private:
	int maxDist;		// ����������ɫ���
	cv::Vec3b target;	// Ŀ����ɫ
						
	// ����������ɫ֮��ľ���
	int getColorDistance(const cv::Vec3b &color1,
		const cv::Vec3b &color2) const {

		// ��opencv��������ŷʽ����
		return static_cast<int>(
			cv::norm<int, 3>(cv::Vec3i(color1[0] - color2[0],
				color1[1] - color2[1], color1[2] - color2[2]))
			);
	}

	// ������Ŀ����ɫ�Ĳ��
	int getDistanceToTargetColor(const cv::Vec3b &color) const {
		return getColorDistance(color, target);
	}

public:
	// �չ��캯�����ڴ˳�ʼ��Ĭ�ϲ���
	ColorDetector() : maxDist(100), target(0, 0, 0) {}

	// �������캯��
	ColorDetector(uchar blue, uchar green, uchar red,
		int maxDist = 100) : maxDist(maxDist) {
		// ����Ŀ����ɫ
		setTargetColor(blue, green, red);
	}

	// ����������
	cv::Mat operator()(const cv::Mat &image) {
		return process(image);
	}

	// ������ͼ��image�м����target��ɫ��������أ��ڽ��ͼ���ð�ɫ��ʾ
	// ������ɫ�����ڽ��ͼ���ú�ɫ��ʾ
	cv::Mat process(const cv::Mat &image) {
		cv::Mat result;
		// ���·����ֵ���ͼ��
		// ������ͼ��ĳߴ���ͬ�������ǵ�ͨ��
		result.create(image.size(), CV_8U);

		// ����ͼ�񣬴���ÿ������
		for (int j = 0; j < image.rows; j++) {
			// ȡ����j���׵�ַ
			const cv::Vec3b *input = image.ptr<cv::Vec3b>(j);
			uchar *output = result.ptr<uchar>(j);
			// �������е�ÿһ������
			for (int i = 0; i < image.cols; i++) {
				// �Ƚ���Ŀ����ɫ�Ĳ��
				if (getDistanceToTargetColor(input[i]) <= maxDist)
					output[i] = 255;
				else
					output[i] = 0;
			}
		}

		return result;
	}

	// ������ɫ������ֵ
	// ��ֵ�������������������Ϊ0
	void setColorDistanceThreshold(int distance) {
		if (distance < 0)
			distance = 0;
		maxDist = distance;
	}

	// ȡ����ɫ������ֵ
	int getColorDistanceThreshold() const {
		return maxDist;
	}

	// ������Ҫ������ɫ
	void setTargetColor(uchar blue, uchar green, uchar red) {
		// ����ΪBGR
		target = cv::Vec3b(blue, green, red);
	}

	// ������Ҫ������ɫ
	void setTargetColor(cv::Vec3b color) {
		target = color;
	}

	// ��ȡ��Ҫ������ɫ
	cv::Vec3b getTargetColor() const {
		return target;
	}
};

