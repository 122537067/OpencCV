#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class ColorDetector {
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

		cv::Mat converted;	// ת��ɫ�ʿռ���ͼ��
		// ת����Labɫ�ʿռ�
		cv::cvtColor(image, converted, CV_BGR2Lab);

		// ����ͼ�񣬴���ÿ������
		for (int j = 0; j < converted.rows; j++) {
			// ȡ����j���׵�ַ
			const cv::Vec3b *input = converted.ptr<cv::Vec3b>(j);
			uchar *output = result.ptr<uchar>(j);
			// �������е�ÿһ������
			for (int i = 0; i < converted.cols; i++) {
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
		// ��ʱ�ĵ�����ͼ��
		cv::Mat tmp(1, 1, CV_8UC3);
		tmp.at<cv::Vec3b>(0, 0) = cv::Vec3b(blue, green, red);

		// Ŀ����ɫת����Labɫ�ʿռ�
		cv::cvtColor(tmp, tmp, CV_BGR2Lab);

		target = tmp.at<cv::Vec3b>(0, 0);
	}

	// ������Ҫ������ɫ
	void setTargetColor(cv::Vec3b color) {
		// ��ʱ�ĵ�����ͼ��
		cv::Mat tmp(1, 1, CV_8UC3);
		tmp.at<cv::Vec3b>(0, 0) = color;

		// Ŀ����ɫת����Labɫ�ʿռ�
		cv::cvtColor(tmp, tmp, CV_BGR2Lab);

		target = tmp.at<cv::Vec3b>(0, 0);
	}

	// ��ȡ��Ҫ������ɫ
	cv::Vec3b getTargetColor() const {
		return target;
	}
};

void main()
{
	// 1. ����ͼ����������
	ColorDetector cdetect(221, 209, 191);

	// 2.��ȡ�����ͼ��
	cv::Mat image = cv::imread("castle.jpg");
	if (image.empty())  // ͼ��Ϊ��?
		return;
	cv::imshow("ԭͼ", image);

	// 3. �����������
	cdetect.setTargetColor(221, 209, 191);	// �����ʾ�����ɫ
	//cdetect.setTargetColor(cv::Vec3b(221, 209, 191));
	cdetect.setColorDistanceThreshold(10);

	// 4. ����ͼ����ʾ���
	//cv::imshow("Result", cdetect.process(image));
	cv::imshow("Result", cdetect(image));

	cv::waitKey();  // �ȴ�����������Ĭ��Ϊ0
}