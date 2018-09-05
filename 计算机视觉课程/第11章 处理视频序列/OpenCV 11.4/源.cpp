#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

// ��Canny���Ӽ�������ͼ��ı�Ե
void canny(cv::Mat& img, cv::Mat& out) {
	// ���ǲ�ɫͼ����ת���ɻҶ�ͼ��
	if (img.channels() == 3)
		cv::cvtColor(img, out, CV_BGR2GRAY);
	// ����Canny��Ե
	cv::Canny(out, out, 100, 200);
	// ��תͼ��
	cv::threshold(out, out, 128, 255, cv::THRESH_BINARY_INV);
}

// ����֡�Ľӿ�
class FrameProcessor {
public:
	// ������
	virtual void process(cv::Mat &input, cv::Mat &output) = 0;	// ���麯��
};

class VideoProcessor {
private:
	// OpenCV��Ƶ���������
	cv::VideoCapture capture;
	// ����ÿһ֡ʱ������õĺ���ָ��
	void(*process)(cv::Mat&, cv::Mat&);
	FrameProcessor* frameProcessor;	// ֡����������ָ��
									// �����ͱ�������ʾ�ô������Ƿ�ᱻ����
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

	// OpenCVд��Ƶ����
	cv::VideoWriter writer;
	// ����ļ���
	std::string outputFile;

	// ȡ����һ֡
	bool readNextFrame(cv::Mat& frame) {
		return capture.read(frame);
	}

	// д�����֡
	void writeNextFrame(cv::Mat& frame) {
		// д�뵽��Ƶ�ļ�
		writer.write(frame);
	}

public:
	// ���캯��������Ĭ��ֵ
	VideoProcessor() : callIt(false), delay(-1),
		fnumber(0), stop(false), frameToStop(-1),
		process(NULL), frameProcessor(NULL) {}

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

	// �����ڴ�����ָ��������֡�����
	void stopAtFrameNo(long frameNo) {
		frameToStop = frameNo;
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
		return capture.isOpened();
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

	// ץȡ�����������е�֡
	void run() {
		cv::Mat frame;	// ���ڴ洢��ǰ֡
		cv::Mat output;	// ���ڴ洢���֡

						// ���û�д���Ƶ�ļ��򲶻��豸
		if (!isOpened())
			return;

		stop = false;

		while (!isStopped()) {
			// ��ȡ��һ֡(�����)
			if (!readNextFrame(frame))
				break;

			// ��ʾ�����֡
			if (windowNameInput.length() != 0)
				cv::imshow(windowNameInput, frame);

			// ���ô�����
			if (callIt) {
				// ����֡
				if (process)	// �����������Ϊ��
					process(frame, output);
				else if (frameProcessor)	// ����õ���֡������Ľӿ�
					frameProcessor->process(frame, output);
				// �����Ѵ���֡��
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

			// ֡����ʱ
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

	// ��ȡ������Ƶ�ı������
	int getCodec(char codec[4]) {
		union {  // ��ʾ���ַ���������ݽṹ
			int value;
			char code[4];
		} returned;

		// ȡ����Ƶ���ñ����������
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
		int w = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
		int h = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));

		return cv::Size(w, h);
	}

	// ���������Ƶ�ļ�
	// Ĭ������»�ʹ����������Ƶ��ͬ�Ĳ���
	bool setOutput(const std::string &filename, int codec = 0,
		double framerate = 0.0, bool isColor = true) {
		outputFile = filename;

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
};

void main()
{
	// ����ʵ��
	VideoProcessor processor;
	// ����Ƶ�ļ�
	processor.setInput("bike.avi");
	// ���ô���֡�ĺ���
	processor.setFrameProcessor(canny);
	// ���������Ƶ�ļ�
	processor.setOutput("bikeOut.avi", 0.0, 0.0, false);
	//processor.setOutput("bikeOut.avi", -1.0, 0.0, false); // ����ʧ��

	// ����������
	char codec[4];
	processor.getCodec(codec);
	std::cout << "Codec: " << codec[0] << codec[1] <<
		codec[2] << codec[3] << std::endl;

	// ��ʼ����
	processor.run();
}