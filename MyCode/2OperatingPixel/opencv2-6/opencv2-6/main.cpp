//
//  main.cpp
//  opencv2-6
//
//  Created by one on 2018/9/29.
//  Copyright © 2018 one. All rights reserved.
//

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// 锐化函数
void sharpen(const cv::Mat &image, cv::Mat &result)
{
    // 判断是否要重新分配图像数据。如果需要，就分配
    result.create(image.size(), image.type());
    int nchannels = image.channels();    // 获得通道数
    
    // 处理所有行(除了第一行和最后一行)
    for (int j = 1; j < image.rows - 1; j++) {
        const uchar* previous =
        image.ptr<const uchar>(j - 1);    // 上一行
        const uchar* current =
        image.ptr<const uchar>(j);        // 当前行
        const uchar* next =
        image.ptr<const uchar>(j + 1);    // 下一行
        
        uchar* output = result.ptr<uchar>(j);    // 输出行
        
        // 遍历当前行的每个元素(除去第一列和最后一列像素对应的元素)
        for (int i = nchannels; i < (image.cols - 1) * nchannels; i++) {
            // 锐化处理，satureate_cast使得到的结果一定在uchar范围内
            *output++ = cv::saturate_cast<uchar>(
                                                 5 * current[i] - current[i - nchannels] -
                                                 current[i + nchannels] - previous[i] - next[i]
                                                 );
        }
    }
    
    // 把未处理的像素设为0
    result.row(0).setTo(cv::Scalar(0));
    result.row(result.rows - 1).setTo(cv::Scalar(0));
    result.col(0).setTo(cv::Scalar(0));
    result.col(result.cols - 1).setTo(cv::Scalar(0));
}

// 锐化函数(使用cv::filter2D)
void sharpen2D(const cv::Mat &image, cv::Mat &result)
{
    // 构造内核(3×3大小，元素为浮点型，所有元素都初始化为0)
    cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
    
    // 对内核赋值
    kernel.at<float>(1, 1) = 5.0;
    kernel.at<float>(0, 1) = -1.0;
    kernel.at<float>(2, 1) = -1.0;
    kernel.at<float>(1, 0) = -1.0;
    kernel.at<float>(1, 2) = -1.0;
    
    // 对图像滤波
    cv::filter2D(image, result, image.depth(), kernel);
    //std::cout << image.depth() << "==" << CV_8U << std::endl;
}

int main()
{
    cv::Mat image = cv::imread("castle.jpg"); // 读取图像
    cv::imshow("original", image);
    
    cv::Mat result;        // 结果图像
    sharpen(image, result); // 处理图像
    cv::imshow("sharpen", result);
    
    sharpen2D(image, result); // 用filter2D处理
    cv::imshow("sharpenFilter2D", result);
    
    cv::waitKey(0); // 等待按键
}
