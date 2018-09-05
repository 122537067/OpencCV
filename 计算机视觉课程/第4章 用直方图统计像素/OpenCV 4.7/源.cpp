#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

// 计算彩色图像的直方图
class ColorHistogram {
private:
	int histSize[3];		// 每个维度的大小
	float hranges[2];		// 值的范围
	const float* ranges[3];	// 每个维度的范围
	int channels[3];		// 需要处理的通道

public:
	ColorHistogram() {
		// 准备用于彩色图像的默认参数
		// 每个维度的大小和范围是相等的
		histSize[0] = histSize[1] = histSize[2] = 256;
		hranges[0] = 0.0;	// BGR范围为0~256
		hranges[1] = 256.0;
		// 这个类中，所有通道的范围都相等
		ranges[0] = hranges;
		ranges[1] = hranges;
		ranges[2] = hranges;
		// 三个通道
		channels[0] = 0;
		channels[1] = 1;
		channels[2] = 2;
	}

	//计算直方图
	cv::Mat getHistogram(const cv::Mat &image) {
		cv::Mat hist(3,	// 维数
			histSize,			// 每个维度的大小
			CV_32F);

		// 计算直方图
		cv::calcHist(&image,
			1,			// 仅为一个图像的直方图
			channels,	// 使用的通道
			cv::Mat(),	// 不使用掩码
			hist,		// 得到的直方图
			3,			// 这是一个三维的直方图
			histSize,	// 条目数量
			ranges		// 像素值的范围
		);

		return hist;
	}

	//计算直方图
	cv::SparseMat getSparseHistogram(const cv::Mat &image) {
		cv::SparseMat hist(3,	// 维数
			histSize,			// 每个维度的大小
			CV_32F);

		// 计算直方图
		cv::calcHist(&image,
			1,			// 仅为一个图像的直方图
			channels,	// 使用的通道
			cv::Mat(),	// 不使用掩码
			hist,		// 得到的直方图
			3,			// 这是一个三维的直方图
			histSize,	// 条目数量
			ranges		// 像素值的范围
		);

		return hist;
	}

	void setSize(int size) {
		histSize[0] = histSize[1] = histSize[2] = size;
	}

	// 计算一维色调直方图(带掩码)
	// BGR的原图转换成HSV
	// 忽略低饱和度的像素
	cv::Mat getHueHistogram(const cv::Mat &image,
		int minSaturation = 0) {
		cv::Mat hist;

		// 转换成HSV色彩空间
		cv::Mat hsv;
		cv::cvtColor(image, hsv, CV_BGR2HSV);

		cv::Mat mask; // 掩码(可用可不用)

		if (minSaturation > 0) {
			// 把3个通道分割进3个图像
			std::vector<cv::Mat> v;
			cv::split(hsv, v);

			// 构造mask,屏蔽低饱和度的像素
			cv::threshold(v[1], mask, minSaturation, 255, cv::THRESH_BINARY);
		}

		// 准备一维色调直方图的参数
		// 范围为0~180
		hranges[0] = 0.0;
		hranges[1] = 180.0;
		channels[0] = 0;  // 色调通道

		// 计算直方图
		cv::calcHist(&hsv,
			1,			// 仅为一个图像的直方图
			channels,	// 使用的通道
			mask,		// 使用的掩码
			hist,		// 得到的直方图
			1,			// 这是一维直方图
			histSize,	// 条目数量
			ranges		// 像素值的范围
		);

		return hist;
	}
};

// 图像相似度比较类
class ImageComparator {
private:
	cv::Mat refH;	// 基准直方图
	cv::Mat inputH;	// 输入图像的直方图
	ColorHistogram hist;	// 用于生成颜色直方图的对象
	int nBins;		// 每个颜色通道使用的条目数

public:
	ImageComparator():nBins(8){}

	// 设置比较直方图时使用的条目数量
	void setNumberOfBins(int bins) {
		nBins = bins;
	}

	// 设置基准图像，并计算其直方图
	void setReferenceImage(const cv::Mat& image) {
		hist.setSize(nBins);
		refH = hist.getHistogram(image);
		refH = refH / (image.rows * image.cols);
	}

	// 用BGR直方图比较图像，返回相似度
	double compare(const cv::Mat& image) {
		inputH = hist.getHistogram(image);
		inputH = inputH / (image.rows * image.cols);
		return cv::compareHist(refH, inputH, CV_COMP_INTERSECT);
	}
};
 
void main()
{
	// 读入基准图像
	cv::Mat image = cv::imread("基准.jpg");
	cv::imshow("基准图像", image);

	ImageComparator c;	// 图像比较类实例
	c.setReferenceImage(image);	// 设置基准图像

	// 与各图像做比较
	cv::Mat image1 = cv::imread("1.jpg");
	cv::imshow("图像1", image1);
	double similarity = c.compare(image1);
	std::cout << "与图像1相似度：" << similarity << std::endl;

	cv::Mat image2 = cv::imread("2.jpg");
	cv::imshow("图像2", image2);
	similarity = c.compare(image2);
	std::cout << "与图像2相似度：" << similarity << std::endl;

	cv::Mat image3 = cv::imread("3.jpg");
	cv::imshow("图像3", image3);
	similarity = c.compare(image3);
	std::cout << "与图像3相似度：" << similarity << std::endl;

	cv::Mat image4 = cv::imread("4.jpg");
	cv::imshow("图像4", image4);
	similarity = c.compare(image4);
	std::cout << "与图像4相似度：" << similarity << std::endl;

	cv::Mat image5 = cv::imread("5.jpg");
	cv::imshow("图像5", image5);
	similarity = c.compare(image5);
	std::cout << "与图像5相似度：" << similarity << std::endl;

	cv::waitKey();
}