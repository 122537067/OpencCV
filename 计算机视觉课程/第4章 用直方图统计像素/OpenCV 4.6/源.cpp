#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\tracking.hpp>
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

// �����ɫͼ���ֱ��ͼ
class ColorHistogram {
private:
	int histSize[3];		// ÿ��ά�ȵĴ�С
	float hranges[2];		// ֵ�ķ�Χ
	const float* ranges[3];	// ÿ��ά�ȵķ�Χ
	int channels[3];		// ��Ҫ�����ͨ��

public:
	ColorHistogram() {
		// ׼�����ڲ�ɫͼ���Ĭ�ϲ���
		// ÿ��ά�ȵĴ�С�ͷ�Χ����ȵ�
		histSize[0] = histSize[1] = histSize[2] = 256;
		hranges[0] = 0.0;	// BGR��ΧΪ0~256
		hranges[1] = 256.0;
		// ������У�����ͨ���ķ�Χ�����
		ranges[0] = hranges;
		ranges[1] = hranges;
		ranges[2] = hranges;
		// ����ͨ��
		channels[0] = 0;
		channels[1] = 1;
		channels[2] = 2;
	}

	//����ֱ��ͼ
	cv::SparseMat getHistogram(const cv::Mat &image) {
		cv::SparseMat hist(3,			// ά��
			histSize,			// ÿ��ά�ȵĴ�С
			CV_32F);

		// ����ֱ��ͼ
		cv::calcHist(&image,
			1,			// ��Ϊһ��ͼ���ֱ��ͼ
			channels,	// ʹ�õ�ͨ��
			cv::Mat(),	// ��ʹ������
			hist,		// �õ���ֱ��ͼ
			3,			// ����һ����ά��ֱ��ͼ
			histSize,	// ��Ŀ����
			ranges		// ����ֵ�ķ�Χ
		);

		return hist;
	}

	void setSize(int size) {
		histSize[0] = histSize[1] = histSize[2] = size;
	}

	// ����һάɫ��ֱ��ͼ(������)
	// BGR��ԭͼת����HSV
	// ���Եͱ��Ͷȵ�����
	cv::Mat getHueHistogram(const cv::Mat &image,
		int minSaturation = 0) {
		cv::Mat hist;

		// ת����HSVɫ�ʿռ�
		cv::Mat hsv;
		cv::cvtColor(image, hsv, CV_BGR2HSV);

		cv::Mat mask; // ����(���ÿɲ���)

		if (minSaturation > 0) {
			// ��3��ͨ���ָ��3��ͼ��
			std::vector<cv::Mat> v;
			cv::split(hsv, v);

			// ����mask,���εͱ��Ͷȵ�����
			cv::threshold(v[1], mask, minSaturation, 255, cv::THRESH_BINARY);
		}

		// ׼��һάɫ��ֱ��ͼ�Ĳ���
		// ��ΧΪ0~180
		hranges[0] = 0.0;
		hranges[1] = 180.0;
		channels[0] = 0;  // ɫ��ͨ��

		// ����ֱ��ͼ
		cv::calcHist(&hsv,
			1,			// ��Ϊһ��ͼ���ֱ��ͼ
			channels,	// ʹ�õ�ͨ��
			mask,		// ʹ�õ�����
			hist,		// �õ���ֱ��ͼ
			1,			// ����һάֱ��ͼ
			histSize,	// ��Ŀ����
			ranges		// ����ֵ�ķ�Χ
		);

		return hist;
	}
};

// ����ӳ��ֱ��ͼ
class ContentFinder {
private:
	// ֱ��ͼ����
	float hranges[2];
	const float* ranges[3];
	int channels[3];

	float threshold;			// �ж���ֵ
	cv::Mat histogram;	// ����ֱ��ͼ

public:
	ContentFinder() :threshold(0.1f) {
		// �����У�����ͨ���ķ�Χ��ͬ
		ranges[0] = hranges;
		ranges[1] = hranges;
		ranges[2] = hranges;
	}

	// ����ֱ��ͼ����ֵ[0,1]
	void setThreshold(float t) {
		threshold = t;
	}

	// ȡ����ֵ
	float getThreshold() {
		return threshold;
	}

	// �������õ�ֱ��ͼ����һ��
	void setHistogram(const cv::Mat& h) {
		histogram = h;
		cv::normalize(histogram, histogram, 1.0, cv::NORM_L1);
	}

	// ʹ��ȫ��ͨ������Χ[0,256]
	cv::Mat find(const cv::Mat& image) {
		cv::Mat result;

		// Ĭ�Ϸ�Χ[0,256]
		hranges[0] = 0.0;
		hranges[1] = 256.0;
		// ����ͨ��
		channels[0] = 0;
		channels[1] = 1;
		channels[2] = 2;

		return find(image, hranges[0], hranges[1], channels);
	}

	// ��������ֱ��ͼ������
	cv::Mat find(const cv::Mat& image,
		float minValue, float maxValue,
		int *channels) {
		cv::Mat result;

		hranges[0] = minValue;
		hranges[1] = maxValue;

		// ֱ��ͼ��ά������ͨ���б�һ��
		for (int i = 0; i < histogram.dims; i++)
			this->channels[i] = channels[i];

		cv::calcBackProject(&image,
			1,			// һ��ֻʹ��һ��ͼ��
			channels,	// ������ʾ�ĸ�ֱ��ͼά�������ĸ�ͼ��ͨ��
			histogram,	// �õ���ֱ��ͼ
			result,		// ����ͶӰ��ͼ��
			ranges,		// ÿ��ά�ȵ�ֵ��Χ
			255.0		// ѡ�õĻ���ϵ�����Ѹ���ֵ1ӳ�䵽255
		);

		// �Է���ͶӰ�������ֵ�����õ���ֵͼ��
		if (threshold > 0.0)
			cv::threshold(result, result,
				255.0 * threshold, 255.0, cv::THRESH_BINARY);

		return result;
	}
};

void main()
{
	//cv::Mat image = cv::imread("Me1.jpg");
	//cv::namedWindow("Me1");
	//// �������ص�����
	//cv::setMouseCallback("Me1", onMouse, reinterpret_cast<void*>(&image));
	//cv::imshow("Me1", image);
	//cv::waitKey();


	// ��ȡ�ο�ͼ��
	cv::Mat image = cv::imread("Me1.jpg");
	cv::namedWindow("Me1");

	// �������ص�����
	cv::setMouseCallback("Me1", onMouse, reinterpret_cast<void*>(&image));

	// ����ROI
	cv::Rect rect(227, 118, 110, 160);
	cv::Mat imageROI = image(rect);
	cv::rectangle(image, rect, cv::Scalar(0, 0, 255));

	cv::imshow("Me1", image);

	// �õ�ɫ��ֱ��ͼ
	int minSat = 65;	// ��С���Ͷ�
	ColorHistogram hc;
	cv::Mat colorhist = hc.getHueHistogram(imageROI, minSat);

	ContentFinder finder;
	finder.setHistogram(colorhist);

	// �򿪵�2��ͼ��
	image = cv::imread("Me2.jpg");
	// ת����HSVɫ�ʿռ�
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);
	// �õ�ɫ��ֱ��ͼ�ķ���ͶӰ
	int ch[1] = { 0 };
	finder.setThreshold(-1.0f);	// ������ֵ��
	cv::Mat result = finder.find(hsv, 0.0f, 180.0f, ch);
	/*std::vector<cv::Mat> hsvs;
	cv::split(hsv, hsvs);
	cv::imshow("h", hsvs[0]);
	cv::imshow("s", hsvs[1]);
	cv::imshow("v", hsvs[2]);*/
	cv::imshow("result", result);

	// �þ�ֵƽ�Ʒ���������
	cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER,
		10, 0.01);
	cv::CamShift(result, rect, criteria);
	// ��ʾ������
	cv::rectangle(image, rect, cv::Scalar(0, 0, 255));
	cv::imshow("Me2", image);

	cv::waitKey();
}