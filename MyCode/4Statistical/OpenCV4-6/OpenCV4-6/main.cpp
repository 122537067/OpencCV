//
//  main.cpp
//  OpenCV4-6
//
//  Created by one on 2018/10/23.
//  Copyright © 2018 one. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>

// 鼠标事件回调函数
void onMouse(int event, int x, int y, int flags, void* param) {
    cv::Mat *im = reinterpret_cast<cv::Mat*>(param);
    
    switch (event) { // 什么鼠标事件？
        case CV_EVENT_LBUTTONDOWN:    // 鼠标左键按下事件
            // 输出像素值(x, y)
            std::cout << "坐标 (" << x << "," << y << ") 处像素值为: "
            << static_cast<int>(im->at <uchar>(cv::Point(x, y))) << std::endl;
            break;
    }
}

// 计算彩色图像的直方图
class ColorHistogram {
private:
    int histSize[3];        // 每个维度的大小
    float hranges[2];        // 值的范围
    const float* ranges[3];    // 每个维度的范围
    int channels[3];        // 需要处理的通道
    
public:
    ColorHistogram() {
        // 准备用于彩色图像的默认参数
        // 每个维度的大小和范围是相等的
        histSize[0] = histSize[1] = histSize[2] = 256;
        hranges[0] = 0.0;    // BGR范围为0~256
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
    cv::SparseMat getHistogram(const cv::Mat &image) {
        cv::SparseMat hist(3,            // 维数
                           histSize,            // 每个维度的大小
                           CV_32F);
        
        // 计算直方图
        cv::calcHist(&image,
                     1,            // 仅为一个图像的直方图
                     channels,    // 使用的通道
                     cv::Mat(),    // 不使用掩码
                     hist,        // 得到的直方图
                     3,            // 这是一个三维的直方图
                     histSize,    // 条目数量
                     ranges        // 像素值的范围
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
                     1,            // 仅为一个图像的直方图
                     channels,    // 使用的通道
                     mask,        // 使用的掩码
                     hist,        // 得到的直方图
                     1,            // 这是一维直方图
                     histSize,    // 条目数量
                     ranges        // 像素值的范围
                     );
        
        return hist;
    }
};

// 反向映射直方图
class ContentFinder {
private:
    // 直方图参数
    float hranges[2];
    const float* ranges[3];
    int channels[3];
    
    float threshold;            // 判断阈值
    cv::Mat histogram;    // 输入直方图
    
public:
    ContentFinder() :threshold(0.1f) {
        // 本类中，所有通道的范围相同
        ranges[0] = hranges;
        ranges[1] = hranges;
        ranges[2] = hranges;
    }
    
    // 设置直方图的阈值[0,1]
    void setThreshold(float t) {
        threshold = t;
    }
    
    // 取得阈值
    float getThreshold() {
        return threshold;
    }
    
    // 设置引用的直方图并归一化
    void setHistogram(const cv::Mat& h) {
        histogram = h;
        cv::normalize(histogram, histogram, 1.0, cv::NORM_L1);
    }
    
    // 使用全部通道，范围[0,256]
    cv::Mat find(const cv::Mat& image) {
        cv::Mat result;
        
        // 默认范围[0,256]
        hranges[0] = 0.0;
        hranges[1] = 256.0;
        // 三个通道
        channels[0] = 0;
        channels[1] = 1;
        channels[2] = 2;
        
        return find(image, hranges[0], hranges[1], channels);
    }
    
    // 查找属于直方图的像素
    cv::Mat find(const cv::Mat& image,
                 float minValue, float maxValue,
                 int *channels) {
        cv::Mat result;
        
        hranges[0] = minValue;
        hranges[1] = maxValue;
        
        // 直方图的维度数与通道列表一致
        for (int i = 0; i < histogram.dims; i++)
            this->channels[i] = channels[i];
        
        cv::calcBackProject(&image,
                            1,            // 一次只使用一个图像
                            channels,    // 向量表示哪个直方图维度属于哪个图像通道
                            histogram,    // 用到的直方图
                            result,        // 反向投影的图像
                            ranges,        // 每个维度的值范围
                            255.0        // 选用的换算系数，把概率值1映射到255
                            );
        
        // 对反向投影结果做阈值化，得到二值图像
        if (threshold > 0.0)
            cv::threshold(result, result,
                          255.0 * threshold, 255.0, cv::THRESH_BINARY);
        
        return result;
    }
};

int main()
{
    //cv::Mat image = cv::imread("Me1.jpg");
    //cv::namedWindow("Me1");
    //// 设置鼠标回调函数
    //cv::setMouseCallback("Me1", onMouse, reinterpret_cast<void*>(&image));
    //cv::imshow("Me1", image);
    //cv::waitKey();
    
    
    // 读取参考图像
    cv::Mat image = cv::imread("Me1.jpg");
    cv::namedWindow("Me1");
    
    // 设置鼠标回调函数
    cv::setMouseCallback("Me1", onMouse, reinterpret_cast<void*>(&image));
    
    // 定义ROI
    cv::Rect rect(227, 118, 110, 160);
    cv::Mat imageROI = image(rect);
    cv::rectangle(image, rect, cv::Scalar(0, 0, 255));
    
    cv::imshow("Me1", image);
    
    // 得到色调直方图
    int minSat = 65;    // 最小饱和度
    ColorHistogram hc;
    cv::Mat colorhist = hc.getHueHistogram(imageROI, minSat);
    
    ContentFinder finder;
    finder.setHistogram(colorhist);
    
    // 打开第2幅图像
    image = cv::imread("Me2.jpg");
    // 转换成HSV色彩空间
    cv::Mat hsv;
    cv::cvtColor(image, hsv, CV_BGR2HSV);
    // 得到色调直方图的反向投影
    int ch[1] = { 0 };
    finder.setThreshold(-1.0f);    // 不做阈值化
    cv::Mat result = finder.find(hsv, 0.0f, 180.0f, ch);
    /*std::vector<cv::Mat> hsvs;
     cv::split(hsv, hsvs);
     cv::imshow("h", hsvs[0]);
     cv::imshow("s", hsvs[1]);
     cv::imshow("v", hsvs[2]);*/
    cv::imshow("result", result);
    
    // 用均值平移法搜索物体
    cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER,
                              10, 0.01);
    cv::CamShift(result, rect, criteria);
    // 显示检测矩形
    cv::rectangle(image, rect, cv::Scalar(0, 0, 255));
    cv::imshow("Me2", image);
    
    cv::waitKey();
}
