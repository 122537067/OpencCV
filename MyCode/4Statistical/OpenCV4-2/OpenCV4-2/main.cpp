//
//  main.cpp
//  OpenCV4-2
//
//  Created by one on 2018/10/23.
//  Copyright © 2018 one. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;

// 创建灰度图像的直方图
class Histogram1D {
private:
    int histSize[1];        // 直方图中条目的数量
    float hranges[2];        // 值范围
    const float* ranges[1];    // 值范围的指针
    int channels[1];        // 要检查的通道数量
    
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
        //cout << hist.rows << "X" << hist.cols;
        
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
        cv::SparseMat hist(3,    // 维数
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
};

int main()
{
    // 读取输入的图像
    cv::Mat image = cv::imread("group.jpg",
                               CV_LOAD_IMAGE_GRAYSCALE);  // 作为灰度图导入
    cv::imshow("original", image);
    
    // 直方图对象
    Histogram1D h;
    
    // 计算直方图
    cv::Mat histo = h.getHistogram(image);
    
    // 循环遍历并输出每个条目
    for (int i = 0; i < 256; i++)
        cout << "Value " << i << " = " <<
        histo.at<float>(i) << endl;
    
    // 显示直方图图像
    cv::imshow("Histogram", h.getHistogramImage(image));
    
    // 基于直方图特性进行阈值分割
    cv::Mat thresholded; // 输出二值图像
    cv::threshold(image, thresholded,
                  17,        // 阈值
                  255,    // 对超过阈值的像素赋值
                  cv::THRESH_BINARY    // 阈值化类型
                  );
    cv::imshow("CutResult", thresholded); // 显示分割结果
    //cv::imwrite("binary.bmp", thresholded);
    cv::waitKey();
}
