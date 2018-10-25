//
//  main.cpp
//  opencv11-3
//
//  Created by one on 2018/10/25.
//  Copyright © 2018 one. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void picture(cv::Mat& img, cv::Mat& out) {
    // 转换成HSV色彩空间
    cv::Mat hsv;
    cv::cvtColor(img, hsv, CV_BGR2HSV);
    
    // 把三个通道分割进三个图像中
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    
    // 所有像素的亮度通道设为255
    channels[2] = 255;
    // 重新合并通道
    cv::merge(channels, hsv);
    // 转换回BGR
    cv::cvtColor(hsv, out, CV_HSV2BGR);
}

// 用Canny算子计算输入图像的边缘
void canny(cv::Mat& img, cv::Mat& out) {
    // 如是彩色图像，则转换成灰度图像
    if (img.channels() == 3) {
        cv::cvtColor(img, out, CV_BGR2GRAY);
        // 计算Canny边缘
        cv::Canny(out, out, 100, 200);
    }
    else
        cv::Canny(img, out, 100, 200);
    // 反转图像
    cv::threshold(out, out, 128, 255, cv::THRESH_BINARY_INV);
}

// 处理帧的接口
class FrameProcessor {
public:
    // 处理方法
    virtual void process(cv::Mat &input, cv::Mat &output) = 0;    // 纯虚函数
};

class VideoProcessor {
private:
    // OpenCV视频捕获类对象
    cv::VideoCapture capture;
    // 处理每一帧时都会调用的函数指针
    void(*process)(cv::Mat&, cv::Mat&);
    FrameProcessor* frameProcessor;    // 帧处理器对象指针
    // 布尔型变量，表示该处理函数是否会被调用
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
    
    // 取得下一帧
    bool readNextFrame(cv::Mat& frame) {
        return capture.read(frame);
    }
    
public:
    // 构造函数，设置默认值
    VideoProcessor() : callIt(false), delay(-1),
    fnumber(0), stop(false), frameToStop(-1),
    process(NULL), frameProcessor(NULL) {}
    
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
    
    // 设置针对每一帧调用的处理函数
    void setFrameProcessor(
                           void(*frameProcessingCallback)(
                                                          cv::Mat&, cv::Mat&)) {
                               process = frameProcessingCallback;
                               frameProcessor = NULL;    // 使帧处理器失效
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
    
    // 设置在处理完指定数量的帧后结束
    void stopAtFrameNo(long frameNo) {
        frameToStop = frameNo;
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
        return capture.isOpened();
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
    
    // 抓取并处理序列中的帧
    void run() {
        cv::Mat frame;    // 用于存储当前帧
        cv::Mat output;    // 用于存储输出帧
        
        // 如果没有打开视频文件或捕获设备
        if (!isOpened())
            return;
        
        stop = false;
        
        while (!isStopped()) {
            // 读取下一帧(如果有)
            if (!readNextFrame(frame))
                break;
            
            // 显示输入的帧
            if (windowNameInput.length() != 0)
                cv::imshow(windowNameInput, frame);
            
            // 调用处理函数
            if (callIt) {
                // 处理帧
                if (process)    // 如果处理函数不为空
                    process(frame, output);
                else if (frameProcessor)    // 如果用的是帧处理类的接口
                    frameProcessor->process(frame, output);
                // 递增已处理帧数
                fnumber++;
            }
            else {    // 没有处理
                output = frame;
            }
            
            // 显示输出的帧
            if (windowNameOutput.length() != 0)
                cv::imshow(windowNameOutput, output);
            
            // 帧间延时
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
};

int main()
{
    // 创建实例
    VideoProcessor processor;
    // 打开视频文件
    processor.setInput("bike.mp4");
    // 打开默认摄像头
    //processor.setInput(0);
    // 创建显示视频的窗口
    processor.displayInput("now");
    processor.displayOutput("output");
    // 用原始帧速率播放视频
    processor.setDelay(1000. / processor.getFrameRate());
    //processor.setDelay(0);    // 逐帧查看
    //processor.setDelay(1000.0 / 30);
    // 指定在处理完指定数量的帧后结束
    //processor.stopAtFrameNo(10);
    // 设置处理帧的函数
    processor.setFrameProcessor(picture);
    // 开始处理
    processor.run();
}
