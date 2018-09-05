#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

void picture(cv::Mat& img, cv::Mat& out) {
	// ת����HSVɫ�ʿռ�
	cv::Mat hsv;
	cv::cvtColor(img, hsv, CV_BGR2HSV);

	// ������ͨ���ָ������ͼ����
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);

	// �������ص�����ͨ����Ϊ255
	channels[2] = 255;
	// ���ºϲ�ͨ��
	cv::merge(channels, hsv);
	// ת����BGR
	cv::cvtColor(hsv, out, CV_HSV2BGR);
}

// ��Canny���Ӽ�������ͼ��ı�Ե
void canny(cv::Mat& img, cv::Mat& out) {
	// ���ǲ�ɫͼ����ת���ɻҶ�ͼ��
	if (img.channels() == 3) {
		cv::cvtColor(img, out, CV_BGR2GRAY);
		// ����Canny��Ե
		cv::Canny(out, out, 100, 200);
	}
	else
		cv::Canny(img, out, 100, 200);
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

	// ȡ����һ֡
	bool readNextFrame(cv::Mat& frame) {
		return capture.read(frame);
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
};

void main()
{
	// ����ʵ��
	VideoProcessor processor;
	// ����Ƶ�ļ�
	processor.setInput("bike.avi");
	// ��Ĭ������ͷ
	//processor.setInput(0);
	// ������ʾ��Ƶ�Ĵ���
	processor.displayInput("��ǰ֡");
	processor.displayOutput("���֡");
	// ��ԭʼ֡���ʲ�����Ƶ
	processor.setDelay(1000. / processor.getFrameRate());
	//processor.setDelay(0);	// ��֡�鿴
	//processor.setDelay(1000.0 / 30);
	// ָ���ڴ�����ָ��������֡�����
	//processor.stopAtFrameNo(10);
	// ���ô���֡�ĺ���
	processor.setFrameProcessor(picture);
	// ��ʼ����
	processor.run();
}