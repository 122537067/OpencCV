#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
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

// �����Ҷ�ͼ���ֱ��ͼ
class Histogram1D {
private:
	int histSize[1];		// ֱ��ͼ����Ŀ������
	float hranges[2];		// ֵ��Χ
	const float* ranges[1];	// ֵ��Χ��ָ��
	int channels[1];		// Ҫ����ͨ��

public:
	Histogram1D() {
		// ׼��һάֱ��ͼ��Ĭ�ϲ���
		histSize[0] = 256;	// 256����Ŀ
		hranges[0] = 0.0;	// ��0��ʼ(��)
		hranges[1] = 256.0;	// ��256(����)
		ranges[0] = hranges;// ֵ��Χ��ָ��
		channels[0] = 0;	// �ȹ�עͨ��0
	}

	//����һάֱ��ͼ
	cv::Mat getHistogram(const cv::Mat &image) {
		cv::Mat hist;

		// ����ֱ��ͼ
		cv::calcHist(&image,
			1,			// ��Ϊһ��ͼ���ֱ��ͼ
			channels,	// ʹ�õ�ͨ��
			cv::Mat(),	// ��ʹ������
			hist,		// ��Ϊ�����ֱ��ͼ
			1,			// ����һά��ֱ��ͼ
			histSize,	// ��Ŀ����
			ranges		// ����ֵ�ķ�Χ
		);

		return hist;
	}

	// ����һάֱ��ͼ������������ͼ��
	cv::Mat getHistogramImage(const cv::Mat &image,
		int zoom = 1) {
		// ���ȼ���ֱ��ͼ
		cv::Mat hist = getHistogram(image);

		// ����ͼ��
		return getImageOfHistogram(hist, zoom);
	}

	// ������ʾһ��ֱ��ͼ��ͼ��(��̬����)
	static cv::Mat getImageOfHistogram(
		const cv::Mat &hist, int zoom) {	// zoom������ͼ��Ĵ�С
		// ȡ����Ŀֵ�����ֵ����Сֵ
		double maxVal = 0;
		double minVal = 0;
		cv::minMaxLoc(hist,
			&minVal,	// ����Сֵ��ָ��
			&maxVal,	// �����ֵ��ָ��
			NULL,		// ��Сֵλ��ָ��	
			NULL		// ���ֵλ��ָ��
		);

		// ȡ��ֱ��ͼ�Ĵ�С
		int histSize = hist.rows; // ֱ��ͼ��n��1�е�ͼ��

		// ������ʾֱ��ͼ�ķ���ͼ��
		cv::Mat histImg(histSize * zoom, histSize * zoom,
			CV_8U, cv::Scalar(255));

		// ������ߵ�Ϊ90%(��ͼ��߶�)����Ŀ����
		int hpt = static_cast<int>(0.9 * histSize);

		// Ϊÿ����Ŀ����ֱ��
		for (int h = 0; h < histSize; h++) {
			float binVal = hist.at<float>(h);
			if (binVal > 0) { // ����0����Ҫ��
				int intensity = static_cast<int>(binVal * hpt / maxVal);
				cv::line(histImg, cv::Point(h * zoom, histSize * zoom),
					cv::Point(h * zoom, (histSize - intensity) * zoom),
					cv::Scalar(0), zoom);
			}
		}

		return histImg;
	}

	// ����ͶӰֱ��ͼ
	void backProject(const cv::Mat& image, const cv::Mat& hist, cv::Mat& result) {
		cv::calcBackProject(&image,
			1,			// һ��ͼ��
			channels,	// �õ���ͨ����ȡ����ֱ��ͼ��ά��
			hist,		// ��Ҫ����ͶӰ��ֱ��ͼ
			result,		// ����ͶӰ�õ��Ľ��
			ranges,		// ֵ�ķ�Χ
			255.0		// ѡ�õĻ���ϵ�����Ѹ���ֵ1ӳ�䵽255
			);
	}
};

// ����ӳ����ɫֱ��ͼ
class ContentFinder {
private:
	// ֱ��ͼ����
	float hranges[2];
	const float* ranges[3];
	int channels[3];

	float threshold;			// �ж���ֵ
	cv::SparseMat histogram;	// ����ֱ��ͼ

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
	void setHistogram(const cv::SparseMat& h) {
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
		for (int i = 0; i < histogram.dims(); i++)
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

// �����ɫBGRͼ���ֱ��ͼ
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
};

void main()
{
	// ��ȡ�����ͼ��
	/*cv::Mat image = cv::imread("cloud.jpg",
		CV_LOAD_IMAGE_GRAYSCALE);  // ��Ϊ�Ҷ�ͼ����

	// �������ص�����
	cv::setMouseCallback("ԭͼ", onMouse, reinterpret_cast<void*>(&image));

	// ������Ȥ����(ROI)
	cv::Rect rect(556, 159, 40, 40);
	cv::Mat imageROI = image(rect);	// �Ʋ�����
	// ��ʾ��ROI��ԭͼ
	cv::Mat imageShown = image.clone(); // Ϊ�����޸�image�����п�¡
	cv::rectangle(imageShown, rect, cv::Scalar(0));
	cv::imshow("ԭͼ", imageShown);

	// ��ȡROI��ֱ��ͼ
	Histogram1D h;
	cv::Mat hist = h.getHistogram(imageROI);

	// ��һ��ֱ��ͼ���õ�ROI�и����������Ʋʵĸ���
	cv::normalize(
		hist,	// ��������
		hist,	// �������
		1.0,	// ��һ�����������1
		0.0,	// �˲������range��һ������������
		cv::NORM_L1);	// ��ʾ�����Ԫ��֮��Ϊ1.0

	// ����ͶӰֱ��ͼ
	cv::Mat result;
	h.backProject(image, hist, result);
	cv::imshow("����ͶӰ���", result);
	//cv::imshow("����ͶӰ���ֱ��ͼ", h.getHistogramImage(result));

	// ��ֵ��
	cv::threshold(result, result, 2, 255, cv::THRESH_BINARY);
	cv::imshow("��ֵ�����", result);*/
	
	/*����ӳ���ɫֱ��ͼ*/
	ColorHistogram hc;
	cv::Mat color = cv::imread("cloud.jpg");  // װ�ز�ɫͼ��

	// ������Ȥ����
	cv::Rect rect(556, 159, 40, 40);
	cv::Mat imageROI = color(rect);	// �Ʋ�����
	// ��ʾ��ROI��ԭͼ
	cv::Mat imageShown = color.clone(); // Ϊ�����޸�image�����п�¡
	cv::rectangle(imageShown, rect, cv::Scalar(0, 0, 255));
	cv::imshow("ԭͼ", imageShown);

	// ȡ��3D��ɫֱ��ͼ(ÿ��ͨ����8����Ŀ)
	hc.setSize(8); // 8��8��8
	cv::SparseMat shist = hc.getHistogram(imageROI);

	// ����������Ѱ��
	ContentFinder finder;
	// ������������ͶӰ��ֱ��ͼ
	finder.setHistogram(shist);
	finder.setThreshold(0.05f);

	// ȡ����ɫֱ��ͼ�ķ���ͶӰ
	cv::Mat result = finder.find(color);
	cv::imshow("�����", result);

	cv::waitKey();	// �ȴ�����
}