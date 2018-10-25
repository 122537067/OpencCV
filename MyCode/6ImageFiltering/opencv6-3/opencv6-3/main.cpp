//
//  main.cpp
//  opencv6-3
//
//  Created by one on 2018/10/24.
//  Copyright © 2018 one. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main()
{
    // 读取输入图像
    cv::Mat image = cv::imread("castleWithNoises.jpg");
    cv::imshow("SaltNoise", image);
    
    cv::Mat result;
    cv::blur(image, result, cv::Size(5, 5));
    cv::imshow("MeanFilter", result);
    
    cv::GaussianBlur(image, result, cv::Size(7, 7), 0);
    cv::imshow("GaussFilter", result);
    
    // 中值滤波
    cv::medianBlur(image, result,
                   3);  // 滤波器尺寸，必须>1且为奇数
    cv::medianBlur(result, result,
                   3);  // 滤波器尺寸，必须>1且为奇数
    cv::imshow("MedianFilter", result);
    
    cv::waitKey();
}
