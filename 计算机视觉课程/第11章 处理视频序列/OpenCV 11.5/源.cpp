#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\tracking.hpp>
#include <iostream>
#include <iomanip>

// ����֡�Ľӿ�
class FrameProcessor {
public:
	// ������
	virtual void process(cv::Mat &input, cv::Mat &output) = 0;	// ���麯��
};

class FeatureTracker :public FrameProcessor {
private:
	cv::Mat gray;	// ��ǰ�Ҷ�ͼ��
	cv::Mat gray_prev;	// ��һ֡�Ҷ�ͼ��
	// �����ٵ���������0��1
	std::vector<cv::Point2f> points[2];
	// ������������ĳ�ʼλ��
	std::vector<cv::Point2f> initial;
	std::vector<cv::Point2f> features;	// ��⵽������
	int max_count;	// ����������������
	double qlevel;	// ���������������ȼ�
	double minDist;	// ����������֮�����С���
	std::vector<uchar> status;	// ������������״̬
	std::vector<float> err;		// �����г��ֵ����

	// �ж��Ƿ���Ҫ����µ�������
	bool addNewPoints() {
		// �������������̫��
		return points[0].size() <= 10;
	}

	// �������ⷽ��
	void detectFeaturePoints() {
		// ���������
		cv::goodFeaturesToTrack(gray_prev,  // ͼ��
			features,	// �����⵽��������
			max_count,	// ��������������
			qlevel,		// �����ȼ�
			minDist);	// ������֮�����С���
	}

	// �ж���Ҫ������������
	bool acceptTrackedPoint(int i) {
		return status[i] &&
			// ����Ѿ��ƶ�
			(abs(points[0][i].x - points[1][i].x) + (abs(points[0][i].y -
				points[1][i].y)) > 2);
	}

	// ����ǰ���ٵ�������
	// ��������������ǵĳ�ʼλ��
	void handleTrackedPoints(cv::Mat &frame, cv::Mat &output) {
		// ��������������
		for (int i = 0; i < points[1].size(); i++) {
			// ���ߺ�Բ
			cv::line(output,	// Ŀ��ͼ��
				initial[i],		// ��ʼλ��
				points[1][i],	// ��λ��
				cv::Scalar(255, 255, 255));	// �ð�ɫ����
			cv::circle(output,	// Ŀ��ͼ��
				points[1][i],	// ���ĵ�
				3,				// �뾶
				cv::Scalar(255, 255, 255), // �ð�ɫ����
				-1);			// �߿�Ϊ������ʾ�����ģʽ
		}
	}

public:
	FeatureTracker() :max_count(500), qlevel(0.01), minDist(10.) {}

	void process(cv::Mat &frame, cv::Mat &output) {
		// ת���ɻҶ�ͼ��
		cv::cvtColor(frame, gray, CV_BGR2GRAY);
		frame.copyTo(output);

		// ���������еĵ�һ֡ͼ��
		if (gray_prev.empty())
			gray.copyTo(gray_prev);

		// 1.�����������µ�������
		if (addNewPoints()) {
			// ���������
			detectFeaturePoints();
			// �ڵ�ǰ�����б�����Ӽ�⵽��������
			points[0].insert(points[0].end(), features.begin(),
				features.end());
			initial.insert(initial.end(), features.begin(),
				features.end());
		}

		// 2.��������
		cv::calcOpticalFlowPyrLK(gray_prev, gray, // 2֡����ͼ��(Ҫ��8λ)
			points[0],	// �����һ֡ͼ���������λ��
			points[1],	// ����ڶ�֡ͼ���������λ��
			status,		// ÿһ���������״̬��־���ɹ�����1��ʧ����0
			err);		// ÿһ������������

		// 3.ѭ����鱻���ٵ������㣬�޳�����
		int k = 0;
		for (int i = 0; i < points[1].size(); i++) {
			// �Ƿ�����������㣿
			if (acceptTrackedPoint(i)) {
				// �������б������������
				initial[k] = initial[i];
				points[1][k++] = points[1][i];
			}
		}

		// �޳�����ʧ�ܵ�������
		points[1].resize(k);
		initial.resize(k);

		// 4.�����Ѿ��Ͽɵı�����������
		handleTrackedPoints(frame, output);

		// 5.��ǰ�������ͼ����ǰһ��
		std::swap(points[1], points[0]);
		cv::swap(gray_prev, gray);
	}
};

class VideoProcessor {
private:
	// OpenCV��Ƶ���������
	cv::VideoCapture capture;

	// ����ÿһ֡ʱ������õĺ���ָ��
	void(*process)(cv::Mat&, cv::Mat&);
	FrameProcessor* frameProcessor;	// ֡����������ָ��

	// �����ͱ�������ʾ�ûص������Ƿ�ᱻ����
	bool callIt;
	// ���봰�ڵ���ʾ����
	std::string windowNameInput;
	// ������ڵ���ʾ����
	std::string windowNameOutput;
	// ֮֡�����ʱ
	int delay;
	// �Ѿ������֡��
	long fnumber;
	// �ﵽ���֡��ʱ����
	long frameToStop;
	// ���������־
	bool stop;
	// ��Ϊ��������ͼ���ļ�������
	std::vector<std::string> images;
	// ͼ�������ĵ�����
	std::vector<std::string>::const_iterator itImg;

	// OpenCVд��Ƶ����
	cv::VideoWriter writer;
	// ����ļ���
	std::string outputFile;
	// ���ͼ�����չ��
	std::string extension;
	// ���ͼ���ļ��������ֵ�λ��
	int digits;
	// ���ͼ��ĵ�ǰ���
	int currentIndex;

	// ȡ����һ֡
	// �����ǣ���Ƶ�ļ�������ͷ����ͼ������
	bool readNextFrame(cv::Mat& frame) {
		if (images.size() == 0)
			return capture.read(frame);

		else {  // ��ȡͼ����������һ��ͼ��
			if (itImg != images.end()) {
				frame = cv::imread(*itImg);
				itImg++;
				return frame.data != 0;
			}
			else
				return false;
		}
	}

	// д�����֡
	// �����ǣ���Ƶ�ļ���ͼ����
	void writeNextFrame(cv::Mat& frame) {
		if (extension.length()) {  // д�뵽ͼ����
			std::stringstream ss;
			// ��ϳ�����ļ���
			ss << outputFile << std::setfill('0') << std::setw(digits)
				<< currentIndex++ << extension;
			cv::imwrite(ss.str(), frame);
		}
		else {	// д�뵽��Ƶ�ļ�
			writer.write(frame);
		}
	}

public:
	// ���캯��������Ĭ��ֵ
	VideoProcessor() : callIt(false), delay(-1),
		fnumber(0), stop(false), digits(0), frameToStop(-1),
		process(NULL), frameProcessor(NULL) {}

	// �������ÿһ֡���õĴ�����
	void setFrameProcessor(
		void(*frameProcessingCallback)(
			cv::Mat&, cv::Mat&)) {
		process = frameProcessingCallback;
		frameProcessor = NULL;	// ʹ֡������ʧЧ
		callProcess();
	}

	// ����ʵ��FrameProcessor�ӿڵ�ʵ��
	void setFrameProcessor(FrameProcessor* frameProcessorPtr) {
		// ʹ������ʧЧ
		process = NULL;
		// ������Ǽ��������õ�֡�������ӿ�
		frameProcessor = frameProcessorPtr;
		callProcess();
	}

	// ������Ƶ�ļ������Ʋ���
	bool setInput(std::string filename) {
		fnumber = 0;
		// ��ֹ�Ѿ�����Դ��VideoCaptureʵ������
		capture.release();
		// ����Ƶ�ļ�
		return capture.open(filename);
	}

	// �������豸
	bool setInput(int device) {
		fnumber = 0;
		// ��ֹ�Ѿ�����Դ��VideoCaptureʵ������
		capture.release();
		// ����Ƶ��׽�豸
		return capture.open(device);
	}

	// ��������ͼ�������
	bool setInput(const std::vector<std::string>& imgs) {
		if (imgs.empty())
			return false;

		fnumber = 0;
		// ��ֹ�Ѿ�����Դ��VideoCaptureʵ������
		capture.release();

		// �����ͼ��������Ϊ�������
		images = imgs;
		itImg = images.begin();

		return true;
	}

	// ����������ʾ����֡�Ĵ���
	void displayInput(std::string wn) {
		windowNameInput = wn;
		cv::namedWindow(windowNameInput);
	}

	// ����������ʾ���֡�Ĵ���
	void displayOutput(std::string wn) {
		windowNameOutput = wn;
		cv::namedWindow(windowNameOutput);
	}

	// ץȡ�����������е�֡
	void run() {
		cv::Mat frame;	// ���ڴ洢��ǰ֡
		cv::Mat output;	// ���ڴ洢���֡

		// ���û�����ò����豸
		if (!isOpened())
			return;

		stop = false;

		while (!isStopped()) {
			// ��ȡ��һ֡(�����)
			if (!readNextFrame(frame))
				break;

			// ��ʾ����֡
			if (windowNameInput.length() != 0)
				cv::imshow(windowNameInput, frame);

			// ���ô������򷽷�
			if (callIt) {
				// ����֡
				if (process)	// ����õ��Ǵ�����
					process(frame, output);
				else if (frameProcessor)	// ����õ���֡������Ľӿ�
					frameProcessor->process(frame, output);
				// ����֡��
				fnumber++;
			}
			else {	// û�д���
				output = frame;
			}

			// д�뵽���������
			if (outputFile.length() != 0)
				writeNextFrame(output);

			// ��ʾ�����֡
			if (windowNameOutput.length() != 0)
				cv::imshow(windowNameOutput, output);

			// ������ʱ
			if (delay == 0) // ��֡�鿴
				cv::waitKey(0);
			// �ȴ�ָ����������������ֱ���˳�
			else if (delay > 0 && cv::waitKey(delay) >= 0)
				stopIt();

			// ����Ƿ���Ҫ����
			if (frameToStop >= 0 &&
				getFrameNumber() == frameToStop)
				stopIt();
		}
	}

	// ��������
	void stopIt() {
		stop = true;
	}

	// ��������Ƿ��Ѿ�ֹͣ��
	bool isStopped() {
		return stop;
	}

	// �����豸�Ƿ��Ѿ��򿪣�
	bool isOpened() {
		return capture.isOpened() || !images.empty();
	}

	// ����֮֡�����ʱ(��λΪ����),
	// 0��ʾÿһ֡���ȴ�,
	// ������ʾ����ʱ
	void setDelay(int d) {
		delay = d;
	}

	// ����Ϊ��Ҫ���ûص�����process
	void callProcess() {
		callIt = true;
	}

	// ����Ϊ����Ҫ���ûص�����
	void dontCallProcess() {
		callIt = false;
	}

	// �����ڴ�����ָ��������֡�����
	void stopAtFrameNo(long frameNo) {
		frameToStop = frameNo;
	}

	// ������һ֡�ı��
	long getFrameNumber() {
		// �Ӳ����豸��ȡ��Ϣ
		long fnumber = static_cast<long>(
			capture.get(CV_CAP_PROP_POS_FRAMES));
		return fnumber;
	}

	// ��ȡ��Ƶ��֡����
	double getFrameRate() {
		if (isOpened())
			return capture.get(CV_CAP_PROP_FPS);
		else
			return 0;
	}

	// ȡ��������Ƶ�ı������
	int getCodec(char codec[4]) {
		// ����ͼ��������������������
		if (images.size() != 0) return -1;

		union {  // ��ʾ���ַ���������ݽṹ
			int value;
			char code[4];
		} returned;

		// ȡ�ô���
		returned.value = static_cast<int>
			(capture.get(CV_CAP_PROP_FOURCC));

		// ȡ��4���ַ�
		codec[0] = returned.code[0];
		codec[1] = returned.code[1];
		codec[2] = returned.code[2];
		codec[3] = returned.code[3];

		// ���ش��������ֵ
		return returned.value;
	}

	// ��ȡ��Ƶ��֡�ĳߴ�
	cv::Size getFrameSize() {
		if (images.size() == 0) {
			// ����Ƶ�򲶻��豸��ȡ�ߴ�
			int w = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
			int h = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));

			return cv::Size(w, h);

		}
		else { // ������ͼ������
			cv::Mat tmp = cv::imread(images[0]);
			if (!tmp.data) return cv::Size(0, 0);
			else return tmp.size();
		}
	}

	// ���������Ƶ�ļ�
	// Ĭ������»�ʹ����������Ƶ��ͬ�Ĳ���
	bool setOutput(const std::string &filename, int codec = 0,
		double framerate = 0.0, bool isColor = true) {
		outputFile = filename;
		extension.clear();

		if (framerate == 0.0)
			framerate = getFrameRate();	// ��������ͬ

		char c[4];
		// ʹ����������ͬ�ı������
		if (codec == 0) {
			codec = getCodec(c);
		}

		// �������Ƶ
		return writer.open(outputFile,	// �ļ���
			codec,		// �������
			framerate,	// ��Ƶ��֡����
			getFrameSize(),	// ֡�ĳߴ�
			isColor);	// ��ɫ��Ƶ?
	}

	// �������Ϊһϵ��ͼ���ļ�
	// ��չ��������.jpg��.bmp����
	bool setOutput(const std::string &filename,  // ǰ׺
		const std::string &ext,	// ͼ���ļ�����չ��
		int numberOfDigits = 3,	// ���ֵ�λ��
		int startIndex = 0) {	// ��ʼ���
								// ���ֵ�λ������������
		if (numberOfDigits < 0)
			return false;

		// �ļ����ͳ��õ���չ��
		outputFile = filename;
		extension = ext;

		// �ļ���ŷ��������ֵ�λ��
		digits = numberOfDigits;
		// �������ſ�ʼ���
		currentIndex = startIndex;

		return true;
	}
};

void main()
{
	// ����ʵ��
	VideoProcessor processor;

	// ���������������ʵ��
	FeatureTracker tracker;

	// ����Ƶ�ļ�
	processor.setInput("bike.avi");

	// ����֡������
	processor.setFrameProcessor(&tracker);

	// ������ʾ��Ƶ�Ĵ���
	processor.displayOutput("Tracked Features");

	// ��ԭʼ֡���ʲ�����Ƶ
	processor.setDelay(1000. / processor.getFrameRate());

	// ��ʼ����
	processor.run();
}