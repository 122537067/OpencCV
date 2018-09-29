//
//  main.cpp
//  OpenCV_second
//
//  Created by one on 2018/9/10.
//  Copyright © 2018 one. All rights reserved.
//

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>

using namespace cv;
using namespace std;

//为图像image添加盐噪声，n为盐噪声的数量
void salt(Mat image,int n)
{
    int i,j;
    for(int k = 0;k<n;k++)
    {
        i = rand() % image.cols;
        j = rand() % image.rows;
//        i =  2000;
//        double randn = double(rand()%11)/10;
//        std::cout<<randn<<std::endl;
//        j = j+1;
//        if(j>=image.rows) j = image.rows-1;
//
        
//        std::cout<<image.size();
        
        //8UC1单通道图像-8UC3用于rgb三通道
        if(image.type() == CV_8UC1)   //灰度图像
        {
            
            image.at<uchar>(j,i) = 255;  //将像素值设为白色
            cout<<image.size()<<endl;
        }
        else if(image.type() == CV_8UC3) //彩色图像
        {
            //将像素值设为白色
//            image.at<Vec3b>(j,i) [0] = 255;
//            image.at<Vec3b>(j,i) [1] = 255;
//            image.at<Vec3b>(j,i) [2] = 255;
            image.at<Vec3b>(j,i) = Vec3b(222,222,222);
        }
    }
}

int main() {
    Mat image = imread("mine.png");
    
    std::cout<<image.size()<<std::endl;
    
    salt(image, 22000);
    
    imshow("addSalt", image);
    waitKey(0);
}
