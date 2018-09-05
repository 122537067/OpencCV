#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

// Ӧ�ò��ұ�ĺ���
cv::Mat applyLookUp(
	const cv::Mat& image,	// ����ͼ��
	const cv::Mat& lookup) { // uchar���͵�1��256����

	cv::Mat result;	// ���ͼ��

					// Ӧ�ò��ұ�
	cv::LUT(image, lookup, result);

	return result;
}

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

	// ֱ��ͼ����
	cv::Mat stretch(const cv::Mat &image, float minPercent = 0) {
		// ��minPercentתΪ����������
		float minValue = minPercent * image.rows * image.cols;

		// ���ȼ���ֱ��ͼ
		cv::Mat hist = getHistogram(image);

		// �ҵ�ֱ��ͼ�������ֵ
		int imin = 0;
		float sum = 0; // �ۻ���Ŀֵ
		for (; imin < histSize[0]; imin++) {
			sum += hist.at<float>(imin);
			if (sum > minValue)
				break;
		}

		// �ҵ�ֱ��ͼ���ұ���ֵ
		int imax = histSize[0] - 1;
		sum = 0; // �ۻ���Ŀֵ
		for (; imax >= 0; imax--) {
			sum += hist.at<float>(imax);
			if (sum > minValue)
				break;
		}

		// �������ұ�
		int dim(256);
		cv::Mat lookup(1,	// һ��
			&dim,		// 256��
			CV_8U);		// uchar����

						// �������ұ�
		for (int i = 0; i < 256; i++) {
			// ��imin��imax֮������
			if (i < imin) lookup.at<uchar>(i) = 0;
			else if (i > imax) lookup.at<uchar>(i) = 255;
			// ����ӳ��
			else lookup.at<uchar>(i) =
				cvRound(255.0 * (i - imin) / (imax - imin));
		}

		// Ӧ�ò��ұ�
		cv::Mat result;
		result = applyLookUp(image, lookup);

		return result;
	}
};

void main()
{
	// ��ȡ�����ͼ��
	cv::Mat image = cv::imread("group.jpg",
		CV_LOAD_IMAGE_GRAYSCALE);  // ��Ϊ�Ҷ�ͼ����
	cv::imshow("ԭͼ", image);

	// ֱ��ͼ���⻯
	cv::Mat result;
	cv::equalizeHist(image, result);
	cv::imshow("ֱ��ͼ���⻯���", result);

	Histogram1D h;	// һάֱ��ͼ����
	cv::imshow("ԭʼֱ��ͼ", h.getHistogramImage(image));
	cv::imshow("���⻯��ֱ��ͼ", h.getHistogramImage(result));

	cv::waitKey();	// �ȴ�����
}