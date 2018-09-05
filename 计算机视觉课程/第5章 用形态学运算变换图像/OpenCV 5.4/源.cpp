#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class MorphoFeatures {
private:
	// 用于产生二值图像的阈值
	int threshold;
	// 用于检测角点的结构元素
	cv::Mat_<uchar> cross;
	cv::Mat_<uchar> diamond;
	cv::Mat_<uchar> square;
	cv::Mat_<uchar> x;

	// 对result图像应用阈值
	void applyThreshold(cv::Mat& result) {
		if (threshold > 0)
			cv::threshold(result, result, threshold, 255, 
				cv::THRESH_BINARY_INV);
	}

public:
	MorphoFeatures() :threshold(-1),
		cross(5, 5), diamond(5, 5), square(5, 5), x(5, 5) {
		// 创建十字形结构元素
		cross <<
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0,
			1, 1, 1, 1, 1,
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0;

		// 创建菱形结构元素
		diamond <<
			0, 0, 1, 0, 0,
			0, 1, 1, 1, 0,
			1, 1, 1, 1, 1,
			0, 1, 1, 1, 0,
			0, 0, 1, 0, 0;

		// 创建正方形结构元素
		square = 1; // 设所有像素值为1

		// 创建X形结构元素
		x <<
			1, 0, 0, 0, 1,
			0, 1, 0, 1, 0,
			0, 0, 1, 0, 0,
			0, 1, 0, 1, 0,
			1, 0, 0, 0, 1;
	}

	// 设置阈值
	void setThreshold(int t) {
		threshold = t;
	}

	// 获取阈值
	int getThreshold() const {
		return threshold;
	}

	// 获取image图像中的边缘
	cv::Mat getEdges(const cv::Mat &image, const cv::Mat &element = cv::Mat()) {
		// 获得梯度图
		cv::Mat result;
		cv::morphologyEx(image, result, cv::MORPH_GRADIENT, element);

		// 应用阈值以得到二值图像
		applyThreshold(result);

		return result;
	}

	// 依次应用所有结构元素以检测角点
	cv::Mat getCorners(const cv::Mat &image) {
		cv::Mat result;

		// 用十字形元素膨胀
		cv::dilate(image, result, cross);
		//cv::imshow("用十字形元素膨胀", result);
		//cv::waitKey();

		// 用菱形元素腐蚀
		cv::erode(result, result, diamond);
		//cv::imshow("用菱形元素腐蚀", result);
		//cv::waitKey();

		cv::Mat result2;
		// 用X形元素膨胀
		cv::dilate(image, result2, x);
		//cv::imshow("用X形元素膨胀", result2);
		//cv::waitKey();

		// 用正方形元素腐蚀
		cv::erode(result2, result2, square);
		//cv::imshow("用正方形元素腐蚀", result2);
		//cv::waitKey();

		// 计算两个闭运算图像之差
		cv::absdiff(result2, result, result);
		//cv::imshow("两个闭运算图像之差", result);
		//cv::waitKey();

		// 应用阈值，获得二值图像
		applyThreshold(result);
		//cv::imshow("二值图像", result);
		//cv::waitKey();

		return result;
	}

	// 对于二值图像binary中像素值为0的位置，在灰度图像image中同样位置处绘制圆圈
	void drawOnImage(const cv::Mat& binary, cv::Mat& image) {
		// 用于遍历灰度图像binary的迭代器
		cv::Mat_<uchar>::const_iterator it = binary.begin<uchar>();
		cv::Mat_<uchar>::const_iterator itend = binary.end<uchar>();

		// 遍历每一个像素
		for (int i = 0; it != itend; ++it, ++i) {
			if (!*it) // 如果像素值非0
				cv::circle(image, cv::Point(i % image.step, i / image.step), 
					5, cv::Scalar(255, 0, 0));
		}
	}
};

void main()
{
	// 读取输入图像
	//cv::Mat image = cv::imread("building.bmp", CV_LOAD_IMAGE_UNCHANGED);
	//cv::imshow("原图", image);

	cv::Mat image = cv::imread("building.jpg");
	cv::imshow("原图", image);
	cv::Mat gray;
	cv::cvtColor(image, gray, CV_BGR2GRAY);
	cv::imshow("原灰度图", gray);
	
	// 用3×3结构元素得到梯度图像
	//cv::Mat result;
	//cv::morphologyEx(image, result, 
	//	cv::MORPH_GRADIENT, cv::Mat());
	//cv::imshow("梯度图像", result);

	//// 对图像阈值化得到一个二值图像
	//int threshold = 40;
	//cv::threshold(result, result,
	//	threshold, 255, cv::THRESH_BINARY);
	//cv::imshow("阈值化结果", result);

	// 检测图像中的边缘
	MorphoFeatures morpho;
	morpho.setThreshold(40);
	cv::Mat element(3, 3, CV_8U, 1);
	cv::Mat result = morpho.getEdges(gray, element);
	cv::imshow("边缘检测结果", result);

	cv::Mat dilated;
	cv::dilate(gray, dilated, cv::Mat());
	result = dilated - gray;
	cv::threshold(result, result, 30, 255,
		cv::THRESH_BINARY_INV);
	cv::imshow("边缘检测结果新", result);

	// 角点检测
	morpho.setThreshold(35);
	cv::Mat corners = morpho.getCorners(gray);
	/*cv::imshow("角点检测结果corners", corners);
	cv::morphologyEx(corners, corners, cv::MORPH_TOPHAT, cv::Mat());
	cv::imshow("角点检测结果MORPH_TOPHAT", corners);
	cv::threshold(corners, corners, 35, 255, cv::THRESH_BINARY_INV);
	cv::imshow("角点检测结果threshold", corners);*/
	// 在图像上显示角点
	morpho.drawOnImage(corners, gray);
	cv::imshow("角点检测结果", gray);

	cv::waitKey();
}