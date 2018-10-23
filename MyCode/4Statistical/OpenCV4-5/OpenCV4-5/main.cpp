//
//  main.cpp
//  OpenCV4-5
//
//  Created by one on 2018/10/23.
//  Copyright © 2018 one. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
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

// 创建灰度图像的直方图
class Histogram1D {
private:
    int histSize[1];        // 直方图中条目的数量
    float hranges[2];        // 值范围
    const float* ranges[1];    // 值范围的指针
    int channels[1];        // 要检查的通道
    
public:
    Histogram1D() {
        // 准备一维直方图的默认参数
        histSize[0] = 256;    // 256个条目
        hranges[0] = 0.0;    // 从0开始(含)
        hranges[1] = 256.0;    // 到256(不含)
        ranges[0] = hranges;// 值范围的指针
        channels[0] = 0;    // 先关注通道0
    }
    
    //计算一维直方图
    cv::Mat getHistogram(const cv::Mat &image) {
        cv::Mat hist;
        
        // 计算直方图
        cv::calcHist(&image,
                     1,            // 仅为一个图像的直方图
                     channels,    // 使用的通道
                     cv::Mat(),    // 不使用掩码
                     hist,        // 作为结果的直方图
                     1,            // 这是一维的直方图
                     histSize,    // 条目数量
                     ranges        // 像素值的范围
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
                                       const cv::Mat &hist, int zoom) {    // zoom决定了图像的大小
        // 取得条目值的最大值和最小值
        double maxVal = 0;
        double minVal = 0;
        cv::minMaxLoc(hist,
                      &minVal,    // 存最小值的指针
                      &maxVal,    // 存最大值的指针
                      NULL,        // 最小值位置指针
                      NULL        // 最大值位置指针
                      );
        
        // 取得直方图的大小
        int histSize = hist.rows; // 直方图是n行1列的图像
        
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
    
    // 反向投影直方图
    void backProject(const cv::Mat& image, const cv::Mat& hist, cv::Mat& result) {
        cv::calcBackProject(&image,
                            1,            // 一个图像
                            channels,    // 用到的通道，取决于直方图的维度
                            hist,        // 需要反向投影的直方图
                            result,        // 反向投影得到的结果
                            ranges,        // 值的范围
                            255.0        // 选用的换算系数，把概率值1映射到255
                            );
    }
};

// 反向映射颜色直方图
class ContentFinder {
private:
    // 直方图参数
    float hranges[2];
    const float* ranges[3];
    int channels[3];
    
    float threshold;            // 判断阈值
    cv::SparseMat histogram;    // 输入直方图
    
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
    void setHistogram(const cv::SparseMat& h) {
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
        for (int i = 0; i < histogram.dims(); i++)
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

// 计算彩色BGR图像的直方图
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
};

int main()
{
    // 读取输入的图像
    /*cv::Mat image = cv::imread("cloud.jpg",
     CV_LOAD_IMAGE_GRAYSCALE);  // 作为灰度图导入
     // 设置鼠标回调函数
     cv::setMouseCallback("原图", onMouse, reinterpret_cast<void*>(&image));
     // 定义兴趣区域(ROI)
     cv::Rect rect(556, 159, 40, 40);
     cv::Mat imageROI = image(rect);    // 云彩区域
     // 显示带ROI的原图
     cv::Mat imageShown = image.clone(); // 为避免修改image，进行克隆
     cv::rectangle(imageShown, rect, cv::Scalar(0));
     cv::imshow("原图", imageShown);
     // 提取ROI的直方图
     Histogram1D h;
     cv::Mat hist = h.getHistogram(imageROI);
     // 归一化直方图，得到ROI中各像素属于云彩的概率
     cv::normalize(
     hist,    // 输入数组
     hist,    // 输出数组
     1.0,    // 归一化，这里就填1
     0.0,    // 此参数针对range归一化，这里无用
     cv::NORM_L1);    // 表示结果各元素之和为1.0
     // 反向投影直方图
     cv::Mat result;
     h.backProject(image, hist, result);
     cv::imshow("反向投影结果", result);
     //cv::imshow("反向投影结果直方图", h.getHistogramImage(result));
     // 阈值化
     cv::threshold(result, result, 2, 255, cv::THRESH_BINARY);
     cv::imshow("阈值化结果", result);*/
    
    /*反向映射彩色直方图*/
    ColorHistogram hc;
    cv::Mat color = cv::imread("cloud.jpg");  // 装载彩色图像
    
    // 定义兴趣区域
    cv::Rect rect(556, 159, 40, 40);
    cv::Mat imageROI = color(rect);    // 云彩区域
    // 显示带ROI的原图
    cv::Mat imageShown = color.clone(); // 为避免修改image，进行克隆
    cv::rectangle(imageShown, rect, cv::Scalar(0, 0, 255));
    cv::imshow("Original", imageShown);
    
    // 取得3D颜色直方图(每个通道含8个条目)
    hc.setSize(8); // 8×8×8
    cv::SparseMat shist = hc.getHistogram(imageROI);
    
    // 创建内容搜寻器
    ContentFinder finder;
    // 设置用来反向投影的直方图
    finder.setHistogram(shist);
    finder.setThreshold(0.05f);
    
    // 取得颜色直方图的反向投影
    cv::Mat result = finder.find(color);
    cv::imshow("Result", result);
    
    cv::waitKey();    // 等待按键
}
