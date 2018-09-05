#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class MorphoFeatures {
private:
	// ���ڲ�����ֵͼ�����ֵ
	int threshold;
	// ���ڼ��ǵ�ĽṹԪ��
	cv::Mat_<uchar> cross;
	cv::Mat_<uchar> diamond;
	cv::Mat_<uchar> square;
	cv::Mat_<uchar> x;

	// ��resultͼ��Ӧ����ֵ
	void applyThreshold(cv::Mat& result) {
		if (threshold > 0)
			cv::threshold(result, result, threshold, 255, 
				cv::THRESH_BINARY_INV);
	}

public:
	MorphoFeatures() :threshold(-1),
		cross(5, 5), diamond(5, 5), square(5, 5), x(5, 5) {
		// ����ʮ���νṹԪ��
		cross <<
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0,
			1, 1, 1, 1, 1,
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0;

		// �������νṹԪ��
		diamond <<
			0, 0, 1, 0, 0,
			0, 1, 1, 1, 0,
			1, 1, 1, 1, 1,
			0, 1, 1, 1, 0,
			0, 0, 1, 0, 0;

		// ���������νṹԪ��
		square = 1; // ����������ֵΪ1

		// ����X�νṹԪ��
		x <<
			1, 0, 0, 0, 1,
			0, 1, 0, 1, 0,
			0, 0, 1, 0, 0,
			0, 1, 0, 1, 0,
			1, 0, 0, 0, 1;
	}

	// ������ֵ
	void setThreshold(int t) {
		threshold = t;
	}

	// ��ȡ��ֵ
	int getThreshold() const {
		return threshold;
	}

	// ��ȡimageͼ���еı�Ե
	cv::Mat getEdges(const cv::Mat &image, const cv::Mat &element = cv::Mat()) {
		// ����ݶ�ͼ
		cv::Mat result;
		cv::morphologyEx(image, result, cv::MORPH_GRADIENT, element);

		// Ӧ����ֵ�Եõ���ֵͼ��
		applyThreshold(result);

		return result;
	}

	// ����Ӧ�����нṹԪ���Լ��ǵ�
	cv::Mat getCorners(const cv::Mat &image) {
		cv::Mat result;

		// ��ʮ����Ԫ������
		cv::dilate(image, result, cross);
		//cv::imshow("��ʮ����Ԫ������", result);
		//cv::waitKey();

		// ������Ԫ�ظ�ʴ
		cv::erode(result, result, diamond);
		//cv::imshow("������Ԫ�ظ�ʴ", result);
		//cv::waitKey();

		cv::Mat result2;
		// ��X��Ԫ������
		cv::dilate(image, result2, x);
		//cv::imshow("��X��Ԫ������", result2);
		//cv::waitKey();

		// ��������Ԫ�ظ�ʴ
		cv::erode(result2, result2, square);
		//cv::imshow("��������Ԫ�ظ�ʴ", result2);
		//cv::waitKey();

		// ��������������ͼ��֮��
		cv::absdiff(result2, result, result);
		//cv::imshow("����������ͼ��֮��", result);
		//cv::waitKey();

		// Ӧ����ֵ����ö�ֵͼ��
		applyThreshold(result);
		//cv::imshow("��ֵͼ��", result);
		//cv::waitKey();

		return result;
	}

	// ���ڶ�ֵͼ��binary������ֵΪ0��λ�ã��ڻҶ�ͼ��image��ͬ��λ�ô�����ԲȦ
	void drawOnImage(const cv::Mat& binary, cv::Mat& image) {
		// ���ڱ����Ҷ�ͼ��binary�ĵ�����
		cv::Mat_<uchar>::const_iterator it = binary.begin<uchar>();
		cv::Mat_<uchar>::const_iterator itend = binary.end<uchar>();

		// ����ÿһ������
		for (int i = 0; it != itend; ++it, ++i) {
			if (!*it) // �������ֵ��0
				cv::circle(image, cv::Point(i % image.step, i / image.step), 
					5, cv::Scalar(255, 0, 0));
		}
	}
};

void main()
{
	// ��ȡ����ͼ��
	//cv::Mat image = cv::imread("building.bmp", CV_LOAD_IMAGE_UNCHANGED);
	//cv::imshow("ԭͼ", image);

	cv::Mat image = cv::imread("building.jpg");
	cv::imshow("ԭͼ", image);
	cv::Mat gray;
	cv::cvtColor(image, gray, CV_BGR2GRAY);
	cv::imshow("ԭ�Ҷ�ͼ", gray);
	
	// ��3��3�ṹԪ�صõ��ݶ�ͼ��
	//cv::Mat result;
	//cv::morphologyEx(image, result, 
	//	cv::MORPH_GRADIENT, cv::Mat());
	//cv::imshow("�ݶ�ͼ��", result);

	//// ��ͼ����ֵ���õ�һ����ֵͼ��
	//int threshold = 40;
	//cv::threshold(result, result,
	//	threshold, 255, cv::THRESH_BINARY);
	//cv::imshow("��ֵ�����", result);

	// ���ͼ���еı�Ե
	MorphoFeatures morpho;
	morpho.setThreshold(40);
	cv::Mat element(3, 3, CV_8U, 1);
	cv::Mat result = morpho.getEdges(gray, element);
	cv::imshow("��Ե�����", result);

	cv::Mat dilated;
	cv::dilate(gray, dilated, cv::Mat());
	result = dilated - gray;
	cv::threshold(result, result, 30, 255,
		cv::THRESH_BINARY_INV);
	cv::imshow("��Ե�������", result);

	// �ǵ���
	morpho.setThreshold(35);
	cv::Mat corners = morpho.getCorners(gray);
	/*cv::imshow("�ǵ�����corners", corners);
	cv::morphologyEx(corners, corners, cv::MORPH_TOPHAT, cv::Mat());
	cv::imshow("�ǵ�����MORPH_TOPHAT", corners);
	cv::threshold(corners, corners, 35, 255, cv::THRESH_BINARY_INV);
	cv::imshow("�ǵ�����threshold", corners);*/
	// ��ͼ������ʾ�ǵ�
	morpho.drawOnImage(corners, gray);
	cv::imshow("�ǵ�����", gray);

	cv::waitKey();
}