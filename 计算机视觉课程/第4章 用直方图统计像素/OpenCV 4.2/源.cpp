#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
using namespace std;

// �����Ҷ�ͼ���ֱ��ͼ
class Histogram1D {
private:
	int histSize[1];		// ֱ��ͼ����Ŀ������
	float hranges[2];		// ֵ��Χ
	const float* ranges[1];	// ֵ��Χ��ָ��
	int channels[1];		// Ҫ����ͨ������

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
		//cout << hist.rows << "X" << hist.cols;

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
};

void main()
{
	// ��ȡ�����ͼ��
	cv::Mat image = cv::imread("group.jpg", 
		CV_LOAD_IMAGE_GRAYSCALE);  // ��Ϊ�Ҷ�ͼ����
	cv::imshow("ԭͼ", image);

	// ֱ��ͼ����
	Histogram1D h;

	// ����ֱ��ͼ
	cv::Mat histo = h.getHistogram(image);
	
	// ѭ�����������ÿ����Ŀ
	for (int i = 0; i < 256; i++)
		cout << "Value " << i << " = " <<
		histo.at<float>(i) << endl;
	
	// ��ʾֱ��ͼͼ��
	cv::imshow("Histogram", h.getHistogramImage(image));

	// ����ֱ��ͼ���Խ�����ֵ�ָ�
	cv::Mat thresholded; // �����ֵͼ��
	cv::threshold(image, thresholded,
		17,		// ��ֵ
		255,	// �Գ�����ֵ�����ظ�ֵ
		cv::THRESH_BINARY	// ��ֵ������
	);
	cv::imshow("�ָ���", thresholded); // ��ʾ�ָ���
	//cv::imwrite("binary.bmp", thresholded);
	cv::waitKey();
}