//
//  main.cpp
//  opencv6-2
//
//  Created by one on 2018/10/24.
//  Copyright © 2018 one. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main()
{
    // 读取输入图像
    cv::Mat image = cv::imread("castle.jpg");
    cv::imshow("Original", image);
    
    // 平滑滤波
    cv::Mat result;
    //cv::blur(image, result,
    //    cv::Size(5, 5));    // 滤波器尺寸
    cv::boxFilter(image, result, CV_8U, cv::Size(5, 5));
    cv::imshow("Smoothness", result);
    
    // 高斯平滑滤波
    cv::GaussianBlur(image, result, cv::Size(5, 5), 0);
    cv::imshow("GaussSmoothness", result);
    
    cv::waitKey();
}
