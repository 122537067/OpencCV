#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class ColorDetector {
private:
	int maxDist;		// 允许的最大颜色差距
	cv::Vec3b target;	// 目标颜色
	//cv::Mat result;		// 存储二值映射结果的图像(类型是灰度图)

	// 计算两个颜色之间的距离
	int getColorDistance(const cv::Vec3b &color1,
		const cv::Vec3b &color2) const {
		// 计算城区距离
		/*return abs(color1[0] - color2[0]) +
			abs(color1[1] - color2[1]) +
			abs(color1[2] - color2[2]);*/

		// 计算欧式距离
		/*return sqrt((color1[0] - color2[0]) * (color1[0] - color2[0]) +
			(color1[1] - color2[1]) * (color1[1] - color2[1]) +
			(color1[2] - color2[2]) * (color1[2] - color2[2]));*/

		// 用opencv函数计算欧式距离
		return static_cast<int>(
			cv::norm<int, 3>(cv::Vec3i(color1[0] - color2[0],
				color1[1] - color2[1], color1[2] - color2[2]))
			);

		//cv::Vec3b dist;
		//cv::absdiff(color1, color2, dist);
		//return cv::sum(dist)[0]; // 加[0]是为了完成从cv::scalar到int的转换
	}

	// 计算与目标颜色的差距
	int getDistanceToTargetColor(const cv::Vec3b &color) const {
		return getColorDistance(color, target);
	}

public:
	// 空构造函数，在此初始化默认参数
	ColorDetector() : maxDist(100), target(0, 0, 0) {}
	
	// 完整构造函数
	ColorDetector(uchar blue, uchar green, uchar red,
		int maxDist = 100) : maxDist(maxDist) {
		// 设置目标颜色
		setTargetColor(blue, green, red);
	}

	// 操作符重载
	cv::Mat operator()(const cv::Mat &image) {
		return process(image);
	}

	// 从输入图像image中检测与target颜色相近的像素，在结果图中用白色表示
	// 其他颜色像素在结果图中用黑色表示
	cv::Mat process(const cv::Mat &image) {
		cv::Mat result;
		// 重新分配二值结果图像
		// 与输入图像的尺寸相同，不过是单通道
		result.create(image.size(), CV_8U);

		// 遍历图像，处理每个像素
		for (int j = 0; j < image.rows; j++) {
			// 取得行j的首地址
			const cv::Vec3b *input = image.ptr<cv::Vec3b>(j);
			uchar *output = result.ptr<uchar>(j);
			// 遍历该行的每一个像素
			for (int i = 0; i < image.cols; i++) {
				// 比较与目标颜色的差距
				if (getDistanceToTargetColor(input[i]) <= maxDist)
					output[i] = 255;
				else
					output[i] = 0;
			}
		}

		return result;
	}

	// 另一种实现方法(使用城区距离)
	//cv::Mat process(const cv::Mat &image) {
	//	cv::Mat result;
	//	// 计算与目标颜色的距离(像素值差的绝对值)
	//	cv::absdiff(image, cv::Scalar(target), result);
	//	// 把通道分割进3个图像
	//	std::vector<cv::Mat> images;
	//	cv::split(result, images);
	//	// 3个通道相加(这里可能出现饱和的情况)
	//	result = images[0] + images[1] + images[2];
	//	// 应用阈值(将小于等于阈值的像素值置为最大值(白色)，大于的置为0(黑色))
	//	cv::threshold(	result,					// 输入图像
	//					result,					// 输出图像
	//					maxDist,				// 阈值(必须<256)
	//					255,					// 最大值
	//					cv::THRESH_BINARY_INV); // 阈值化模式

	//	return result;
	//}

	// 设置颜色差距的阈值
	// 阈值必须是正数，否则就置为0
	void setColorDistanceThreshold(int distance) {
		if (distance < 0)
			distance = 0;
		maxDist = distance;
	}

	// 取得颜色差距的阈值
	int getColorDistanceThreshold() const {
		return maxDist;
	}

	// 设置需要检测的颜色
	void setTargetColor(uchar blue, uchar green, uchar red) {
		// 次序为BGR
		target = cv::Vec3b(blue, green, red);
	}

	// 设置需要检测的颜色
	void setTargetColor(cv::Vec3b color) {
		target = color;
	}

	// 获取需要检测的颜色
	cv::Vec3b getTargetColor() const {
		return target;
	}
};

void main()
{
	// 1. 创建图像处理器对象
	ColorDetector cdetect(221, 209, 191);

	// 2.读取输入的图像
	cv::Mat image = cv::imread("castle.jpg");
	if (image.empty())  // 图像为空?
		return;
	cv::imshow("原图", image);

	// 3. 设置输入参数
	//cdetect.setTargetColor(221, 209, 191);	// 这里表示天空颜色
	//cdetect.setColorDistanceThreshold(100);

	// 4. 处理图像并显示结果
	//cv::imshow("Result", cdetect.process(image));
	cv::imshow("Result", cdetect(image));

	cv::waitKey();  // 等待按键，参数默认为0
}