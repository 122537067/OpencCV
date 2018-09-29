//
//  main.cpp
//  opencv2-7
//
//  Created by one on 2018/9/29.
//  Copyright © 2018 one. All rights reserved.
//

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main()
{
    cv::Mat image1 = cv::imread("castle.jpg"); // 读取图像
    cv::imshow("image1", image1);
    
    cv::Mat image2 = cv::imread("rain.jpg"); // 读取另一图像(大小得和image1一致)
    cv::imshow("image2", image2);
    
    cv::resize(image2, image2, image1.size());
    
    cv::Mat result;    // 结果图像
    // result[i] = image1[i] + image2[i]
    cv::add(image1, image2, result);    // 图像相加
    
    // result[i] = image1[i] + cv::Scalar(-50, -50, -50)
    cv::add(image1, cv::Scalar(-50, -50, -50), result); // 调节图像亮度
    
    //result[i] = image1[i] * 0.4 + image2[i] * 0.6 + 0;
    cv::addWeighted(image1, 0.4, image2, 0.6, 0., result); // 加权和
    
    cv::imshow("Result1", result);
    
    result = image1 * 0.4 + image2 * 0.6;
    
    /*uchar mask = 0xFF << 6;
     int div = pow(2, 6);
     result = (image1 & cv::Scalar(mask, mask, mask))
     + cv::Scalar(div / 2, div / 2, div / 2);*/
    
    // 创建包含三幅图像的向量
    std::vector<cv::Mat> planes;
    // 分割一个三通道图像为三个单通道图像
    cv::split(image1, planes);
    // 红色通道亮度值增加
    planes[2] += 50;
    // 合并三个单通道图像为一个三通道图像
    cv::merge(planes, result);
    
    cv::imshow("Result2", result);
    
    cv::waitKey(0);    // 等待按键
}
