//
//  main.cpp
//  openCV2-4
//
//  Created by one on 2018/9/29.
//  Copyright © 2018 one. All rights reserved.
//

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

//削减图像image的颜色数量，每个通道的颜色值数量雪娇到原来的1/div
void colorReduce(Mat image,int div = 64){
    MatIterator_<Vec3b> it = image.begin<Vec3b>();  //在出事位置获得迭代器
    MatIterator_<Vec3b> itend = image.end<Vec3b>(); //获得结束位置
    for(; it != itend; ++it){
        //处理每个像素
        (*it)[0] = (*it)[0] / div * div + div / 2;
        (*it)[1] = (*it)[1] / div * div + div / 2;
        (*it)[2] = (*it)[2] / div * div + div / 2;
        //像素处理结束
    }
    
}


int main() {
    Mat image = imread("castle.jpg");
    colorReduce(image,64);
    imshow("ReduceColor", image);
    waitKey(0);
}
