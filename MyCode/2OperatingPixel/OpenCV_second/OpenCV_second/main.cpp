//
//  main.cpp
//  OpenCV_second
//
//  Created by one on 2018/9/10.
//  Copyright © 2018 one. All rights reserved.
//

#include <iostream>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

//为图像image添加盐噪声，n为盐噪声的数量
void salt(Mat image,int n)
{
    int i,j;
    for(int k = 0;k<n;k++)
    {
        i = rand() % image.cols;
        j = rand() % image.rows;
        
        if(image.type() == CV_8UC3)   //灰度图像（cv_8uc1 和 cv_8u 值相等）
        {
            image.at<uchar>(j,i) = 255;  //将像素值设为白色
        }
        else if(image.type() == CV_8UC3) //彩色图像
        {
            //将像素值设为白色
//            image.at<Vec3b>(j,i) [0] = 255;
//            image.at<Vec3b>(j,i) [1] = 255;
//            image.at<Vec3b>(j,i) [2] = 255;
            image.at<Vec3b>(j,i) = Vec3b(255,255,255);
        }
    }
}

int main() {
    Mat image = imread("mine.png");
    
    salt(image, 30000);
    
    imshow("addSalt", image);
    waitKey(0);
}
