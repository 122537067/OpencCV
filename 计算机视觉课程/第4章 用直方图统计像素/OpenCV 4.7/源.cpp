#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

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
	cv::Mat getHistogram(const cv::Mat &image) {
		cv::Mat hist(3,	// ά��
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

	//����ֱ��ͼ
	cv::SparseMat getSparseHistogram(const cv::Mat &image) {
		cv::SparseMat hist(3,	// ά��
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

// ͼ�����ƶȱȽ���
class ImageComparator {
private:
	cv::Mat refH;	// ��׼ֱ��ͼ
	cv::Mat inputH;	// ����ͼ���ֱ��ͼ
	ColorHistogram hist;	// ����������ɫֱ��ͼ�Ķ���
	int nBins;		// ÿ����ɫͨ��ʹ�õ���Ŀ��

public:
	ImageComparator():nBins(8){}

	// ���ñȽ�ֱ��ͼʱʹ�õ���Ŀ����
	void setNumberOfBins(int bins) {
		nBins = bins;
	}

	// ���û�׼ͼ�񣬲�������ֱ��ͼ
	void setReferenceImage(const cv::Mat& image) {
		hist.setSize(nBins);
		refH = hist.getHistogram(image);
		refH = refH / (image.rows * image.cols);
	}

	// ��BGRֱ��ͼ�Ƚ�ͼ�񣬷������ƶ�
	double compare(const cv::Mat& image) {
		inputH = hist.getHistogram(image);
		inputH = inputH / (image.rows * image.cols);
		return cv::compareHist(refH, inputH, CV_COMP_INTERSECT);
	}
};
 
void main()
{
	// �����׼ͼ��
	cv::Mat image = cv::imread("��׼.jpg");
	cv::imshow("��׼ͼ��", image);

	ImageComparator c;	// ͼ��Ƚ���ʵ��
	c.setReferenceImage(image);	// ���û�׼ͼ��

	// ���ͼ�����Ƚ�
	cv::Mat image1 = cv::imread("1.jpg");
	cv::imshow("ͼ��1", image1);
	double similarity = c.compare(image1);
	std::cout << "��ͼ��1���ƶȣ�" << similarity << std::endl;

	cv::Mat image2 = cv::imread("2.jpg");
	cv::imshow("ͼ��2", image2);
	similarity = c.compare(image2);
	std::cout << "��ͼ��2���ƶȣ�" << similarity << std::endl;

	cv::Mat image3 = cv::imread("3.jpg");
	cv::imshow("ͼ��3", image3);
	similarity = c.compare(image3);
	std::cout << "��ͼ��3���ƶȣ�" << similarity << std::endl;

	cv::Mat image4 = cv::imread("4.jpg");
	cv::imshow("ͼ��4", image4);
	similarity = c.compare(image4);
	std::cout << "��ͼ��4���ƶȣ�" << similarity << std::endl;

	cv::Mat image5 = cv::imread("5.jpg");
	cv::imshow("ͼ��5", image5);
	similarity = c.compare(image5);
	std::cout << "��ͼ��5���ƶȣ�" << similarity << std::endl;

	cv::waitKey();
}