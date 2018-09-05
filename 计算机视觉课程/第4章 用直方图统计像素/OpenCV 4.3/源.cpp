#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

// 应用查找表的函数
cv::Mat applyLookUp(
	const cv::Mat& image,	// 输入图像
	const cv::Mat& lookup) { // uchar类型的1×256数组
	
	cv::Mat result;	// 输出图像
	
	// 应用查找表
	cv::LUT(image, lookup, result);

	return result;
}

// 创建灰度图像的直方图
class Histogram1D {
private:
	int histSize[1];		// 直方图中条目的数量
	float hranges[2];		// 值范围
	const float* ranges[1];	// 值范围的指针
	int channels[1];		// 要检查的通道数量

public:
	Histogram1D() {
		// 准备一维直方图的默认参数
		histSize[0] = 256;	// 256个条目
		hranges[0] = 0.0;	// 从0开始(含)
		hranges[1] = 256.0;	// 到256(不含)
		ranges[0] = hranges;// 值范围的指针
		channels[0] = 0;	// 先关注通道0
	}

	//计算一维直方图
	cv::Mat getHistogram(const cv::Mat &image) {
		cv::Mat hist;

		// 计算直方图
		cv::calcHist(&image,
			1,			// 仅为一个图像的直方图
			channels,	// 使用的通道
			cv::Mat(),	// 不使用掩码
			hist,		// 作为结果的直方图
			1,			// 这是一维的直方图
			histSize,	// 条目数量
			ranges		// 像素值的范围
		);

		return hist;
	}

	// 计算一维直方图，并返回它的图像
	cv::Mat getHistogramImage(const cv::Mat &image,
		int zoom = 1) {
		// 首先计算直方图
		cv::Mat hist = getHistogram(image);

		// 创建图像
		return getImageOfHistogram(hist, zoom);
	}

	// 创建表示一个直方图的图像(静态方法)
	static cv::Mat getImageOfHistogram(
		const cv::Mat &hist, int zoom) {	// zoom决定了图像的大小
											// 取得条目值的最大值和最小值
		double maxVal = 0;
		double minVal = 0;
		cv::minMaxLoc(hist,
			&minVal,	// 存最小值的指针
			&maxVal,	// 存最大值的指针
			NULL,		// 最小值位置指针	
			NULL		// 最大值位置指针
		);

		// 取得直方图的大小
		int histSize = hist.rows; // 直方图是n行1列的图像
								  //cout << hist.rows << "X" << hist.cols;

								  // 用于显示直方图的方形图像
		cv::Mat histImg(histSize * zoom, histSize * zoom,
			CV_8U, cv::Scalar(255));

		// 设置最高点为90%(即图像高度)的条目个数
		int hpt = static_cast<int>(0.9 * histSize);

		// 为每个条目画垂直线
		for (int h = 0; h < histSize; h++) {
			float binVal = hist.at<float>(h);
			if (binVal > 0) { // 大于0才需要画
				int intensity = static_cast<int>(binVal * hpt / maxVal);
				cv::line(histImg, cv::Point(h * zoom, histSize * zoom),
					cv::Point(h * zoom, (histSize - intensity) * zoom),
					cv::Scalar(0), zoom);
			}
		}

		return histImg;
	}

	// 直方图拉伸
	cv::Mat stretch(const cv::Mat &image, float minPercent = 0) {
		// 将minPercent转为具体像素数
		float minValue = minPercent * image.rows * image.cols;

		// 首先计算直方图
		cv::Mat hist = getHistogram(image);

		// 找到直方图的左边限值
		int imin = 0;
		float sum = 0; // 累积条目值
		for (; imin < histSize[0]; imin++) {
			sum += hist.at<float>(imin);
			if (sum > minValue)
				break;
		}

		// 找到直方图的右边限值
		int imax = histSize[0] - 1;
		sum = 0; // 累积条目值
		for (; imax >= 0; imax--) {
			sum += hist.at<float>(imax);
			if (sum > minValue)
				break;
		}

		// 创建查找表
		int dim(256);
		cv::Mat lookup(1,	// 一行
			&dim,		// 256列
			CV_8U);		// uchar类型

		// 构建查找表
		for (int i = 0; i < 256; i++) {
			// 在imin和imax之间拉伸
			if (i < imin) lookup.at<uchar>(i) = 0;
			else if (i > imax) lookup.at<uchar>(i) = 255;
			// 线性映射
			else lookup.at<uchar>(i) = 
				cvRound(255.0 * (i - imin) / (imax - imin));
		}

		// 应用查找表
		cv::Mat result;
		result = applyLookUp(image, lookup);

		return result;
	}
};

void main()
{
	// 读取输入的图像
	cv::Mat image = cv::imread("group.jpg",
		CV_LOAD_IMAGE_GRAYSCALE);  // 作为灰度图导入
	cv::imshow("原图", image);

	// 创建一个图像反转的查找表
	int dim(256);
	cv::Mat lut(1,	// 一行
		&dim,		// 256列
		CV_8U);		// uchar类型

	uchar* data = lut.ptr<uchar>(0);	// 第0行首址
	for (int i = 0; i < 256; i++) {
		data[i] = 255 - i;
	}

	cv::Mat result = applyLookUp(image, lut);
	cv::imshow("像素值反转后图像", result);

	// 直方图拉伸，把0.1%的像素设为黑色，0.1%的像素设为白色
	Histogram1D h;	// 一维直方图对象
	cv::Mat stretched = h.stretch(image, 0.001f);
	cv::imshow("直方图拉伸结果", stretched);
	cv::imshow("原始直方图", h.getHistogramImage(image));
	cv::imshow("拉伸后直方图", h.getHistogramImage(stretched));

	cv::waitKey();	// 等待按键
}