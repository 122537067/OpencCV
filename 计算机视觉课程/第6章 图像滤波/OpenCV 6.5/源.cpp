#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class CLaplacian {
private:
	//cv::Mat laplace;	// 拉普拉斯算子
	int aperture;		// 拉普拉斯内核的孔径大小(尺寸)

public:
	CLaplacian() : aperture(3) {}

	// 设置内核的孔径大小
	void setAperture(int a) {
		aperture = a;
	}

	// 计算浮点数类型的拉普拉斯滤波结果
	cv::Mat computeLaplacian(const cv::Mat& image) {
		cv::Mat laplace;	// 存放拉普拉斯滤波结果图
		// 计算拉普拉斯滤波结果
		cv::Laplacian(image, laplace, CV_32F, aperture);
		return laplace;
	}

	// 应用拉普拉斯滤波并获得可显示的结果图
	cv::Mat getLaplacianImage(const cv::Mat& image, double scale = -1.0) {
		// 应用拉普拉斯滤波
		cv::Mat laplace = computeLaplacian(image); 

		// 如果scale<0，自动设置缩放系数，
		// 保证后面convertTo结果像素值在(0, 255]
		if (scale < 0) {
			double lapmin, lapmax;
			// 取得最小和最大拉普拉斯值
			cv::minMaxLoc(laplace, &lapmin, &lapmax);
			// 缩放拉普拉斯算子到127
			scale = 127 / std::max(-lapmin, lapmax);
		}

		// 生成灰度图像
		cv::Mat laplaceImage;
		laplace.convertTo(laplaceImage, CV_8U, scale, 128);
		return laplaceImage;
	}

	// 获得过零点的二值图像
	// 拉普拉斯图像的类型必须是CV_32F
	cv::Mat getZeroCrossings(cv::Mat laplace) {
		// 阈值为0，阈值化后负数对应黑色，正数对应白色
		cv::Mat signImage;
		cv::threshold(laplace, signImage, 0, 255, cv::THRESH_BINARY);

		// 把+/-图像转换成CV_8U
		cv::Mat binary;
		signImage.convertTo(binary, CV_8U);
		//cv::imshow("阈值化结果", signImage);

		// 膨胀+/-区域的二值图像
		cv::Mat dilated;
		cv::dilate(binary, dilated, cv::Mat());
		//cv::imshow("膨胀结果", dilated);

		// 返回过零点的轮廓
		return dilated - binary;
	}

	// 进行拉普拉斯边缘增强(锐化)
	// factor为增强系数
	cv::Mat getLaplacianEnhancedImage(const cv::Mat& image, const float factor = 1) {
		// 将原图转为浮点类型图像
		cv::Mat fimage;
		image.convertTo(fimage, CV_32F);

		// 拉普拉斯滤波
		cv::Mat laplace = computeLaplacian(image);

		// 进行拉普拉斯边缘增强
		cv::Mat result = fimage - factor * laplace;

		// 将result转为可显示的8位无符号字节类型
		result.convertTo(result, CV_8U);

		return result;
	}
};

void main()
{
	// 读取并显示原图
	cv::Mat image = cv::imread("castle.jpg");
	cv::imshow("原图", image);

	// 转灰度图
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	cv::imshow("灰度图", gray);

	// 用CLaplacian类应用拉普拉斯算子
	cv::Mat result;
	CLaplacian laplacian;
	laplacian.setAperture(3);	// 3×3的拉普拉斯算子
	cv::Mat laplaceImg3 = laplacian.getLaplacianImage(gray);
	cv::imshow("拉普拉斯滤波结果(3×3)", laplaceImg3);
	result = laplacian.getLaplacianEnhancedImage(image, 0.2);
	cv::imshow("拉普拉斯增强结果(3×3)", result);

	laplacian.setAperture(7);	// 7×7的拉普拉斯算子
	cv::Mat laplaceImg7 = laplacian.getLaplacianImage(gray);
	cv::imshow("拉普拉斯滤波结果(7×7)", laplaceImg7);
	result = laplacian.getLaplacianEnhancedImage(image, 0.002);
	cv::imshow("拉普拉斯增强结果(7×7)", result);

	//// 通过检测拉普拉斯算子结果图像的过零点来检测轮廓
	//laplace = laplacian.computeLaplacian(gray);
	//cv::Mat contour = laplacian.getZeroCrossings(laplace);
	//cv::imshow("过零点轮廓", contour);

	//// 拉普拉斯增强
	//cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
	//kernel.at<float>(1, 1) = 5.0;
	//kernel.at<float>(0, 1) = -1.0;
	//kernel.at<float>(1, 0) = -1.0;
	//kernel.at<float>(1, 2) = -1.0;
	//kernel.at<float>(2, 1) = -1.0;
	//cv::Mat result;
	//cv::filter2D(image, result, image.depth(), kernel);
	//cv::imshow("拉普拉斯增强结果", result);

	//// 高斯差分
	//cv::Mat gauss20, gauss22, dog, zeros;
	//cv::GaussianBlur(gray, gauss20, cv::Size(), 2.0);
	//cv::GaussianBlur(gray, gauss22, cv::Size(), 2.2);
	//// 计算高斯差分
	//cv::subtract(gauss22, gauss20, dog, cv::Mat(), CV_32F);
	//// 计算DoG的过零点
	//zeros = laplacian.getZeroCrossings(dog);
	//cv::imshow("DoG过零点", zeros);

	cv::waitKey();
}