//
//  main.cpp
//  1-2Entry
//
//  Created by one on 2018/9/6.
//  Copyright © 2018 one. All rights reserved.
//

#include <iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

//创建一幅画
Mat createGrey()
{
    Mat ima(500,500,CV_8U,50);
    return ima;
}

int main()
{
    //////////////////////////////////////////////////////////////////1-2
//    Mat image_grey(240,320,CV_8U,100);  //创建240 * 320 的灰度图
//    imshow("ImageGrey", image_grey);
//    waitKey(500); //500毫秒后
//
//    //重新分配一幅新图像
//    image_grey.create(600,200,CV_8U);
//    image_grey = 200;  //设置所有像素值为200
//    imshow("ImageGrey", image_grey);
//    waitKey(1000);
//
//    //红色图像
//    Mat image_red (200,200,CV_8UC3,Scalar(0,0,255));
//    imshow("ImageRed", image_red);
//    waitKey(2000);
//
//    Mat image_mine = imread("mine.png");
//    Mat image_copy(image_mine);
//    image_grey = image_mine;
//
//    image_mine.copyTo(image_red);
//    Mat image_copy_copy = image_mine.clone();
//
//    flip(image_mine, image_mine, 1); //转换图像用来测试
//    imshow("ImageGrey", image_grey);
//    imshow("ImageRed", image_red);
//    imshow("ImageMine", image_mine);
//    imshow("ImageCopy", image_copy);
//    imshow("ImageCopyCopy", image_copy_copy);
//    waitKey(5000);
//
//    //从函数中获取一幅灰度图像
//    Mat image_grey2 =createGrey();
//    imshow("ImageGrey2", image_grey2);
//    waitKey(5000);
//
//    //作为灰度图像读入
//    image_grey = imread("mine.png",CV_LOAD_IMAGE_GRAYSCALE);
//    image_grey.convertTo(image_red, CV_32F,1/255.0,0.0);
//    cout<<image_red<<endl;
//    imshow("ImageGrey", image_red);
//    waitKey(0);

    
    //////////////////////////////////////////////////////////1-3
    Mat image = imread("mine.png");
    Mat logo = imread("logo.png");
    Mat mask(logo);
    cout<<mask.channels()<<endl;
    namedWindow("OriginalImage");
    imshow("OriginalImage", image);
    namedWindow("Logo");
    imshow("Logo", logo);
    
    Mat imageROI(image,Rect(
                            image.cols - logo.cols,
                            image.rows - logo.rows,
                            logo.cols,
                            logo.rows
                            ));
    
    logo.copyTo(imageROI,mask);  //插入logo
    
    namedWindow("NewImage");
    imshow("NewImage", image);
    
    waitKey(0);
}








