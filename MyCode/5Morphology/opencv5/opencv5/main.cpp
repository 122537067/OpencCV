//
//  main.cpp
//  opencv5
//
//  Created by one on 2018/10/23.
//  Copyright © 2018 one. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main()
{
    // 读取输入图像
    cv::Mat image = cv::imread("binary.bmp");
    //image = cv::Scalar(255, 255, 255) - image;
    cv::imshow("Original", image);
    cv::waitKey();
    
    // 腐蚀图像
    cv::Mat eroded;    // 目标图像
    // 结构元素传空矩阵表示使用默认3×3正方形结构元素
    cv::erode(image, eroded, cv::Mat());
    cv::imshow("Corrosion", eroded);
    
    // 膨胀图像
    cv::Mat dilated;    // 目标图像
    // 结构元素传空矩阵表示使用默认3×3正方形结构元素
    cv::dilate(image, dilated, cv::Mat());
    cv::imshow("Expand", dilated);
    cv::waitKey();
    
    // 使用自定义的7×7结构元素(元素值全为1)
    cv::Mat element(7, 7, CV_8U, cv::Scalar(1));
    cv::erode(image, eroded, element);
    cv::imshow("7*7Corrosion", eroded);
    cv::waitKey();
    
    // 腐蚀图像三次
    cv::erode(image, eroded, cv::Mat(), cv::Point(-1, -1), 3);
    cv::imshow("Corrosion3", eroded);
    cv::waitKey();
    
    // 闭运算
    cv::Mat element5(5, 5, CV_8U, cv::Scalar(1));
    cv::Mat closed;
    cv::morphologyEx(image, closed, cv::MORPH_CLOSE, element5);
    cv::imshow("Close", closed);
    
    // 开运算
    cv::Mat opened;
    cv::morphologyEx(image, opened, cv::MORPH_OPEN, element5);
    cv::imshow("Open", opened);
    
    cv::waitKey();
}
