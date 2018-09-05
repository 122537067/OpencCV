#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\tracking.hpp>
#include <iostream>
#include <iomanip>

// 处理帧的接口
class FrameProcessor {
public:
	// 处理方法
	virtual void process(cv::Mat &input, cv::Mat &output) = 0;	// 纯虚函数
};

class VideoProcessor {
private:
	// OpenCV视频捕获类对象
	cv::VideoCapture capture;

	// 处理每一帧时都会调用的函数指针
	void(*process)(cv::Mat&, cv::Mat&);
	FrameProcessor* frameProcessor;	// 帧处理器对象指针

									// 布尔型变量，表示该回调函数是否会被调用
	bool callIt;
	// 输入窗口的显示名称
	std::string windowNameInput;
	// 输出窗口的显示名称
	std::string windowNameOutput;
	// 帧之间的延时
	int delay;
	// 已经处理的帧数
	long fnumber;
	// 达到这个帧数时结束
	long frameToStop;
	// 结束处理标志
	bool stop;
	// 作为输入对象的图像文件名向量
	std::vector<std::string> images;
	// 图像向量的迭代器
	std::vector<std::string>::const_iterator itImg;

	// OpenCV写视频对象
	cv::VideoWriter writer;
	// 输出文件名
	std::string outputFile;
	// 输出图像的扩展名
	std::string extension;
	// 输出图像文件名中数字的位数
	int digits;
	// 输出图像的当前序号
	int currentIndex;

	// 取得下一帧
	// 可以是：视频文件、摄像头或者图像向量
	bool readNextFrame(cv::Mat& frame) {
		if (images.size() == 0)
			return capture.read(frame);

		else {  // 读取图像向量中下一幅图像
			if (itImg != images.end()) {
				frame = cv::imread(*itImg);
				itImg++;
				return frame.data != 0;
			}
			else
				return false;
		}
	}

	// 写输出的帧
	// 可以是：视频文件或图像组
	void writeNextFrame(cv::Mat& frame) {
		if (extension.length()) {  // 写入到图像组
			std::stringstream ss;
			// 组合成输出文件名
			ss << outputFile << std::setfill('0') << std::setw(digits)
				<< currentIndex++ << extension;
			cv::imwrite(ss.str(), frame);
		}
		else {	// 写入到视频文件
			writer.write(frame);
		}
	}

public:
	// 构造函数，设置默认值
	VideoProcessor() : callIt(false), delay(-1),
		fnumber(0), stop(false), digits(0), frameToStop(-1),
		process(NULL), frameProcessor(NULL) {}

	// 设置针对每一帧调用的处理函数
	void setFrameProcessor(
		void(*frameProcessingCallback)(
			cv::Mat&, cv::Mat&)) {
		process = frameProcessingCallback;
		frameProcessor = NULL;	// 使帧处理器失效
		callProcess();
	}

	// 设置实现FrameProcessor接口的实例
	void setFrameProcessor(FrameProcessor* frameProcessorPtr) {
		// 使处理函数失效
		process = NULL;
		// 这个就是即将被调用的帧处理器接口
		frameProcessor = frameProcessorPtr;
		callProcess();
	}

	// 设置视频文件的名称并打开
	bool setInput(std::string filename) {
		fnumber = 0;
		// 防止已经有资源与VideoCapture实例关联
		capture.release();
		// 打开视频文件
		return capture.open(filename);
	}

	// 打开摄像设备
	bool setInput(int device) {
		fnumber = 0;
		// 防止已经有资源与VideoCapture实例关联
		capture.release();
		// 打开视频捕捉设备
		return capture.open(device);
	}

	// 设置输入图像的向量
	bool setInput(const std::vector<std::string>& imgs) {
		if (imgs.empty())
			return false;

		fnumber = 0;
		// 防止已经有资源与VideoCapture实例关联
		capture.release();

		// 将这个图像向量作为输入对象
		images = imgs;
		itImg = images.begin();

		return true;
	}

	// 创建用于显示输入帧的窗口
	void displayInput(std::string wn) {
		windowNameInput = wn;
		cv::namedWindow(windowNameInput);
	}

	// 创建用于显示输出帧的窗口
	void displayOutput(std::string wn) {
		windowNameOutput = wn;
		cv::namedWindow(windowNameOutput);
	}

	// 抓取并处理序列中的帧
	void run() {
		cv::Mat frame;	// 用于存储当前帧
		cv::Mat output;	// 用于存储输出帧

						// 如果没有设置捕获设备
		if (!isOpened())
			return;

		stop = false;

		while (!isStopped()) {
			// 读取下一帧(如果有)
			if (!readNextFrame(frame))
				break;

			// 显示输入帧
			if (windowNameInput.length() != 0)
				cv::imshow(windowNameInput, frame);

			// 调用处理函数或方法
			if (callIt) {
				// 处理帧
				if (process)	// 如果用的是处理函数
					process(frame, output);
				else if (frameProcessor)	// 如果用的是帧处理类的接口
					frameProcessor->process(frame, output);
				// 递增帧数
				fnumber++;
			}
			else {	// 没有处理
				output = frame;
			}

			// 写入到输出的序列
			if (outputFile.length() != 0)
				writeNextFrame(output);

			// 显示输出的帧
			if (windowNameOutput.length() != 0)
				cv::imshow(windowNameOutput, output);

			// 产生延时
			if (delay == 0) // 逐帧查看
				cv::waitKey(0);
			// 等待指定毫秒数，按键则直接退出
			else if (delay > 0 && cv::waitKey(delay) >= 0)
				stopIt();

			// 检查是否需要结束
			if (frameToStop >= 0 &&
				getFrameNumber() == frameToStop)
				stopIt();
		}
	}

	// 结束处理
	void stopIt() {
		stop = true;
	}

	// 处理过程是否已经停止？
	bool isStopped() {
		return stop;
	}

	// 捕获设备是否已经打开？
	bool isOpened() {
		return capture.isOpened() || !images.empty();
	}

	// 设置帧之间的延时(单位为毫秒),
	// 0表示每一帧都等待,
	// 负数表示不延时
	void setDelay(int d) {
		delay = d;
	}

	// 设置为需要调用回调函数process
	void callProcess() {
		callIt = true;
	}

	// 设置为不需要调用回调函数
	void dontCallProcess() {
		callIt = false;
	}

	// 设置在处理完指定数量的帧后结束
	void stopAtFrameNo(long frameNo) {
		frameToStop = frameNo;
	}

	// 返回下一帧的编号
	long getFrameNumber() {
		// 从捕获设备获取信息
		long fnumber = static_cast<long>(
			capture.get(CV_CAP_PROP_POS_FRAMES));
		return fnumber;
	}

	// 获取视频的帧速率
	double getFrameRate() {
		if (isOpened())
			return capture.get(CV_CAP_PROP_FPS);
		else
			return 0;
	}

	// 取得输入视频的编解码器
	int getCodec(char codec[4]) {
		// 对于图像向量，本方法无意义
		if (images.size() != 0) return -1;

		union {  // 表示四字符代码的数据结构
			int value;
			char code[4];
		} returned;

		// 取得代码
		returned.value = static_cast<int>
			(capture.get(CV_CAP_PROP_FOURCC));

		// 取得4个字符
		codec[0] = returned.code[0];
		codec[1] = returned.code[1];
		codec[2] = returned.code[2];
		codec[3] = returned.code[3];

		// 返回代码的整数值
		return returned.value;
	}

	// 获取视频中帧的尺寸
	cv::Size getFrameSize() {
		if (images.size() == 0) {
			// 从视频或捕获设备获取尺寸
			int w = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
			int h = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));

			return cv::Size(w, h);

		}
		else { // 输入是图像向量
			cv::Mat tmp = cv::imread(images[0]);
			if (!tmp.data) return cv::Size(0, 0);
			else return tmp.size();
		}
	}

	// 设置输出视频文件
	// 默认情况下会使用与输入视频相同的参数
	bool setOutput(const std::string &filename, int codec = 0,
		double framerate = 0.0, bool isColor = true) {
		outputFile = filename;
		extension.clear();

		if (framerate == 0.0)
			framerate = getFrameRate();	// 与输入相同

		char c[4];
		// 使用与输入相同的编解码器
		if (codec == 0) {
			codec = getCodec(c);
		}

		// 打开输出视频
		return writer.open(outputFile,	// 文件名
			codec,		// 编解码器
			framerate,	// 视频的帧速率
			getFrameSize(),	// 帧的尺寸
			isColor);	// 彩色视频?
	}

	// 设置输出为一系列图像文件
	// 扩展名必须是.jpg、.bmp……
	bool setOutput(const std::string &filename,  // 前缀
		const std::string &ext,	// 图像文件的扩展名
		int numberOfDigits = 3,	// 数字的位数
		int startIndex = 0) {	// 开始序号
								// 数字的位数必须是正数
		if (numberOfDigits < 0)
			return false;

		// 文件名和常用的扩展名
		outputFile = filename;
		extension = ext;

		// 文件编号方案中数字的位数
		digits = numberOfDigits;
		// 从这个序号开始编号
		currentIndex = startIndex;

		return true;
	}
};

// 用Canny算子计算输入图像的边缘
void canny(cv::Mat& img, cv::Mat& out) {
	// 如是彩色图像，则转换成灰度图像
	if (img.channels() == 3)
		cv::cvtColor(img, out, CV_BGR2GRAY);
	// 计算Canny边缘
	cv::Canny(out, out, 100, 200);
	// 反转图像
	cv::threshold(out, out, 128, 255, cv::THRESH_BINARY_INV);
}

void main()
{
	// 创建实例
	VideoProcessor processor;
	// 打开视频文件
	processor.setInput("bike.avi");
	// 设置处理帧的函数
	processor.setFrameProcessor(canny);
	// 设置输出视频文件
	processor.setOutput("bikeOut.avi", 0.0, 0.0, false);
	//processor.setOutput("bikeOut.avi", -1.0, 0.0, false); // 测试失败

	// 输出编解码器
	char codec[4];
	processor.getCodec(codec);
	std::cout << "Codec: " << codec[0] << codec[1] <<
		codec[2] << codec[3] << std::endl;

	// 开始处理
	processor.run();
}
