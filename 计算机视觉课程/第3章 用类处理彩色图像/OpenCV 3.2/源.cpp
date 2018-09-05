#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class ColorDetector {
private:
	int maxDist;		// ����������ɫ���
	cv::Vec3b target;	// Ŀ����ɫ
	//cv::Mat result;		// �洢��ֵӳ������ͼ��(�����ǻҶ�ͼ)

	// ����������ɫ֮��ľ���
	int getColorDistance(const cv::Vec3b &color1,
		const cv::Vec3b &color2) const {
		// �����������
		/*return abs(color1[0] - color2[0]) +
			abs(color1[1] - color2[1]) +
			abs(color1[2] - color2[2]);*/

		// ����ŷʽ����
		/*return sqrt((color1[0] - color2[0]) * (color1[0] - color2[0]) +
			(color1[1] - color2[1]) * (color1[1] - color2[1]) +
			(color1[2] - color2[2]) * (color1[2] - color2[2]));*/

		// ��opencv��������ŷʽ����
		return static_cast<int>(
			cv::norm<int, 3>(cv::Vec3i(color1[0] - color2[0],
				color1[1] - color2[1], color1[2] - color2[2]))
			);

		//cv::Vec3b dist;
		//cv::absdiff(color1, color2, dist);
		//return cv::sum(dist)[0]; // ��[0]��Ϊ����ɴ�cv::scalar��int��ת��
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

	// ��һ��ʵ�ַ���(ʹ�ó�������)
	//cv::Mat process(const cv::Mat &image) {
	//	cv::Mat result;
	//	// ������Ŀ����ɫ�ľ���(����ֵ��ľ���ֵ)
	//	cv::absdiff(image, cv::Scalar(target), result);
	//	// ��ͨ���ָ��3��ͼ��
	//	std::vector<cv::Mat> images;
	//	cv::split(result, images);
	//	// 3��ͨ�����(������ܳ��ֱ��͵����)
	//	result = images[0] + images[1] + images[2];
	//	// Ӧ����ֵ(��С�ڵ�����ֵ������ֵ��Ϊ���ֵ(��ɫ)�����ڵ���Ϊ0(��ɫ))
	//	cv::threshold(	result,					// ����ͼ��
	//					result,					// ���ͼ��
	//					maxDist,				// ��ֵ(����<256)
	//					255,					// ���ֵ
	//					cv::THRESH_BINARY_INV); // ��ֵ��ģʽ

	//	return result;
	//}

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
	//cdetect.setTargetColor(221, 209, 191);	// �����ʾ�����ɫ
	//cdetect.setColorDistanceThreshold(100);

	// 4. ����ͼ����ʾ���
	//cv::imshow("Result", cdetect.process(image));
	cv::imshow("Result", cdetect(image));

	cv::waitKey();  // �ȴ�����������Ĭ��Ϊ0
}