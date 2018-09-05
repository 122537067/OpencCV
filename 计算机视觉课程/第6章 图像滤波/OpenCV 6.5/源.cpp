#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class CLaplacian {
private:
	//cv::Mat laplace;	// ������˹����
	int aperture;		// ������˹�ں˵Ŀ׾���С(�ߴ�)

public:
	CLaplacian() : aperture(3) {}

	// �����ں˵Ŀ׾���С
	void setAperture(int a) {
		aperture = a;
	}

	// ���㸡�������͵�������˹�˲����
	cv::Mat computeLaplacian(const cv::Mat& image) {
		cv::Mat laplace;	// ���������˹�˲����ͼ
		// ����������˹�˲����
		cv::Laplacian(image, laplace, CV_32F, aperture);
		return laplace;
	}

	// Ӧ��������˹�˲�����ÿ���ʾ�Ľ��ͼ
	cv::Mat getLaplacianImage(const cv::Mat& image, double scale = -1.0) {
		// Ӧ��������˹�˲�
		cv::Mat laplace = computeLaplacian(image); 

		// ���scale<0���Զ���������ϵ����
		// ��֤����convertTo�������ֵ��(0, 255]
		if (scale < 0) {
			double lapmin, lapmax;
			// ȡ����С�����������˹ֵ
			cv::minMaxLoc(laplace, &lapmin, &lapmax);
			// ����������˹���ӵ�127
			scale = 127 / std::max(-lapmin, lapmax);
		}

		// ���ɻҶ�ͼ��
		cv::Mat laplaceImage;
		laplace.convertTo(laplaceImage, CV_8U, scale, 128);
		return laplaceImage;
	}

	// ��ù����Ķ�ֵͼ��
	// ������˹ͼ������ͱ�����CV_32F
	cv::Mat getZeroCrossings(cv::Mat laplace) {
		// ��ֵΪ0����ֵ��������Ӧ��ɫ��������Ӧ��ɫ
		cv::Mat signImage;
		cv::threshold(laplace, signImage, 0, 255, cv::THRESH_BINARY);

		// ��+/-ͼ��ת����CV_8U
		cv::Mat binary;
		signImage.convertTo(binary, CV_8U);
		//cv::imshow("��ֵ�����", signImage);

		// ����+/-����Ķ�ֵͼ��
		cv::Mat dilated;
		cv::dilate(binary, dilated, cv::Mat());
		//cv::imshow("���ͽ��", dilated);

		// ���ع���������
		return dilated - binary;
	}

	// ����������˹��Ե��ǿ(��)
	// factorΪ��ǿϵ��
	cv::Mat getLaplacianEnhancedImage(const cv::Mat& image, const float factor = 1) {
		// ��ԭͼתΪ��������ͼ��
		cv::Mat fimage;
		image.convertTo(fimage, CV_32F);

		// ������˹�˲�
		cv::Mat laplace = computeLaplacian(image);

		// ����������˹��Ե��ǿ
		cv::Mat result = fimage - factor * laplace;

		// ��resultתΪ����ʾ��8λ�޷����ֽ�����
		result.convertTo(result, CV_8U);

		return result;
	}
};

void main()
{
	// ��ȡ����ʾԭͼ
	cv::Mat image = cv::imread("castle.jpg");
	cv::imshow("ԭͼ", image);

	// ת�Ҷ�ͼ
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	cv::imshow("�Ҷ�ͼ", gray);

	// ��CLaplacian��Ӧ��������˹����
	cv::Mat result;
	CLaplacian laplacian;
	laplacian.setAperture(3);	// 3��3��������˹����
	cv::Mat laplaceImg3 = laplacian.getLaplacianImage(gray);
	cv::imshow("������˹�˲����(3��3)", laplaceImg3);
	result = laplacian.getLaplacianEnhancedImage(image, 0.2);
	cv::imshow("������˹��ǿ���(3��3)", result);

	laplacian.setAperture(7);	// 7��7��������˹����
	cv::Mat laplaceImg7 = laplacian.getLaplacianImage(gray);
	cv::imshow("������˹�˲����(7��7)", laplaceImg7);
	result = laplacian.getLaplacianEnhancedImage(image, 0.002);
	cv::imshow("������˹��ǿ���(7��7)", result);

	//// ͨ�����������˹���ӽ��ͼ��Ĺ�������������
	//laplace = laplacian.computeLaplacian(gray);
	//cv::Mat contour = laplacian.getZeroCrossings(laplace);
	//cv::imshow("���������", contour);

	//// ������˹��ǿ
	//cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
	//kernel.at<float>(1, 1) = 5.0;
	//kernel.at<float>(0, 1) = -1.0;
	//kernel.at<float>(1, 0) = -1.0;
	//kernel.at<float>(1, 2) = -1.0;
	//kernel.at<float>(2, 1) = -1.0;
	//cv::Mat result;
	//cv::filter2D(image, result, image.depth(), kernel);
	//cv::imshow("������˹��ǿ���", result);

	//// ��˹���
	//cv::Mat gauss20, gauss22, dog, zeros;
	//cv::GaussianBlur(gray, gauss20, cv::Size(), 2.0);
	//cv::GaussianBlur(gray, gauss22, cv::Size(), 2.2);
	//// �����˹���
	//cv::subtract(gauss22, gauss20, dog, cv::Mat(), CV_32F);
	//// ����DoG�Ĺ����
	//zeros = laplacian.getZeroCrossings(dog);
	//cv::imshow("DoG�����", zeros);

	cv::waitKey();
}