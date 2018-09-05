//
//  main.cpp
//  1Entry
//
//  Created by one on 2018/9/5.
//  Copyright © 2018 one. All rights reserved.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

//鼠标事件回调函数
void onMouse(int event,int x,int y,int flags,void *param)
{
    Mat *im = reinterpret_cast<Mat*>(param);
    switch (event) {
            //鼠标左键按下事件
        case CV_EVENT_LBUTTONDOWN:
            cout<<"坐标("<<x<<","<<y<<")处像素值为"<<static_cast<int>(im->at<uchar>(Point(x,y)))<<endl;
            break;
    }
}

int main()
{
    Mat image = imread("puppy.jpg", CV_LOAD_IMAGE_UNCHANGED);  //找到图片（相对路径需要放在跟exe文件同一目录）
    
    //如果图片为空
    if(image.empty())
    {
        cout<<"readding picture fail!"<<endl;
        exit(EXIT_FAILURE);  //退出程序
    }
    cout<<"该图像有"<<image.channels()<<"通道"<<endl;
    
    namedWindow("OriginalImage",WINDOW_NORMAL);  //创建窗口
    imshow("OriginalImage",image);   //显示图像
    
    Mat result; //创建另一个空的图像
    flip(image,result,1);   //参数：1.输入图像 2.输出图像 3.正数表示水平翻转，0表示垂直翻转，附属表示水平和垂直同时翻转
    
    //在另一个窗口显示结果
    namedWindow("AfterImage");
    imshow("AfterImage", result);
    
    setMouseCallback("OriginalImage", onMouse,reinterpret_cast<void*>(&image));  //设置鼠标回调函数
    
    circle(image, Point(386,325), 300, 0,3);   //在图片中画圆
    //目标图像。 中心点坐标。 半径。颜色（黑）。线的粗细
    
    putText(image, "puppy dog", Point(600,592), FONT_HERSHEY_PLAIN, 2.0, 255,2);
    //目标图像。 文本。文本位置。字体类型。字体大小。字体颜色。文本粗细。
    
    imshow("OriginalImage", image);
    
    waitKey(0);    //0表示永远地等待按键，正数表示等待指定的毫秒数
    imwrite("output.bmp", result);  //将处理后的图像保存起来
    
    return 0;
}







