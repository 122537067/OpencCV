//
//  main.cpp
//  opencv11-6
//
//  Created by one on 2018/10/25.
//  Copyright © 2018 one. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <iomanip>

// 处理帧的接口
class FrameProcessor {
public:
    // 处理方法
    virtual void process(cv::Mat &input, cv::Mat &output) = 0;    // 纯虚函数
};

class BGFGSegmentor : public FrameProcessor {
private:
    cv::Mat gray;        // 当前灰度图像
    cv::Mat background;    // 累积的背景
    cv::Mat backImage;    // 当前背景图像
    cv::Mat foreground;    // 前景图像
    // 累计背景时使用的学习速率
    double learningRate;
    int threshold;        // 提取前景的阈值
    
public:
    BGFGSegmentor() : threshold(10), learningRate(0.01) {}
    
    // 处理方法
    void process(cv::Mat &frame, cv::Mat &output) {
        // 转换成灰度图像
        cv::cvtColor(frame, gray, CV_BGR2GRAY);
        
        // 采用第一帧初始化背景
        if (background.empty())
            gray.convertTo(background, CV_32F);
        
        // 背景转换成8U类型
        background.convertTo(backImage, CV_8U);
        
        // 计算图像与背景之间的差异
        cv::absdiff(backImage, gray, foreground);
        
        // 在前景图像上应用阈值
        cv::threshold(foreground, output, threshold, 255,
                      cv::THRESH_BINARY_INV);
        
        // 累积背景
        // background(x,y) = learningRate * gray(x,y) + (1-learningRate)*background(x,y), if(output(x,y)≠0)
        cv::accumulateWeighted(gray, background,
                               learningRate,    // 学习速率
                               output);        // 掩码
    }
    
    // 设置前景阈值
    void setThreshold(int t) {
        threshold = t;
    }
};

class VideoProcessor {
private:
    // OpenCV视频捕获类对象
    cv::VideoCapture capture;
    
    // 处理每一帧时都会调用的函数指针
    void(*process)(cv::Mat&, cv::Mat&);
    FrameProcessor* frameProcessor;    // 帧处理器对象指针
    
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
        else {    // 写入到视频文件
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
        cv::Mat frame;    // 用于存储当前帧
        cv::Mat output;    // 用于存储输出帧
        
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
                if (process)    // 如果用的是处理函数
                    process(frame, output);
                else if (frameProcessor)    // 如果用的是帧处理类的接口
                    frameProcessor->process(frame, output);
                // 递增帧数
                fnumber++;
            }
            else {    // 没有处理
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
            framerate = getFrameRate();    // 与输入相同
        
        char c[4];
        // 使用与输入相同的编解码器
        if (codec == 0) {
            codec = getCodec(c);
        }
        
        // 打开输出视频
        return writer.open(outputFile,    // 文件名
                           codec,        // 编解码器
                           framerate,    // 视频的帧速率
                           getFrameSize(),    // 帧的尺寸
                           isColor);    // 彩色视频?
    }
    
    // 设置输出为一系列图像文件
    // 扩展名必须是.jpg、.bmp……
    bool setOutput(const std::string &filename,  // 前缀
                   const std::string &ext,    // 图像文件的扩展名
                   int numberOfDigits = 3,    // 数字的位数
                   int startIndex = 0) {    // 开始序号
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

int main()
{
    // 创建视频处理类的实例
    VideoProcessor processor;
    
    // 创建背景/前景的分割器
    BGFGSegmentor segmentor;
    segmentor.setThreshold(25);
    
    // 打开视频文件
    processor.setInput("bike.mp4");
    
    // 设置帧处理对象
    processor.setFrameProcessor(&segmentor);
    
    // 声明显示视频的窗口
    processor.displayInput("original");
    processor.displayOutput("output");
    
    // 用原始帧速率播放视频
    processor.setDelay(1000. / processor.getFrameRate());
    
    // 开始处理
    processor.run();
    
    //// 打开视频文件
    //cv::VideoCapture capture("bike.avi");
    //// 检查打开视频是否成功
    //if (!capture.isOpened())
    //    return;
    //// 当前视频帧
    //cv::Mat frame;
    //// 前景的二值图像
    //cv::Mat foreground;
    //cv::namedWindow("Extracted Foreground");
    //// 混合高斯模型类的对象，全部采用默认参数
    //cv::BackgroundSubtractorMOG mog;
    //bool stop(false);
    //// 遍历视频中的所有帧
    //while (!stop) {
    //    // 读取下一帧(如有)
    //    if (!capture.read(frame))
    //        break;
    //    // 更新背景并返回前景
    //    mog(frame, foreground, 0.01);
    //    // 阈值化
    //    cv::threshold(foreground, foreground,
    //        128, 255, cv::THRESH_BINARY_INV);
    //    // 显示前景
    //    cv::imshow("Extracted Foreground", foreground);
    
    //    // 产生延时，或者按键结束
    //    if (cv::waitKey(1000. / capture.get(CV_CAP_PROP_FPS)) >= 0)
    //        stop = true;
    //}
}
