//
//  main.cpp
//  opencv2-3
//
//  Created by one on 2018/9/10.
//  Copyright © 2018 one. All rights reserved.
//

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

//削减图像image的颜色数量，每个通道的颜色值数量削减到原来的1/div
void colorReduce(Mat image , int div = 64)
{
    //如果图像每行末尾没有填充额外像素，则当作一维数组处理
    if(image.isContinuous())
    {
        image.reshape(1,1); //参数：新的通道数，新的行数
    }
    int nr = image.rows; //行数
    int nc = image.cols * image.channels();  //每行的元素数量（注意不是像素数量）
    
    for (int j = 0; j< nr; j++)
    {
        uchar* data = image.ptr<uchar>(j);
        for (int i = 0; i < nr ; i++)
        {
            data[i] = saturate_cast<uchar>(data[i] / div * div + div /2);
        }
    }
}

//削减图像image的颜色数量，每个通道的颜色值数量削减到原来的1/2^n
void colorReduceWithBitOperation(cv::Mat image, int n = 6) {
    int div = pow(2, n);  // 削减因子
    // 用来截取像素值的掩码
    uchar mask = 0xFF << n; // div=16，则mask=0xF0
    
    int nr = image.rows; //行数
    // 每行的元素数量
    int nc = image.cols * image.channels();
    
    for (int j = 0; j < nr; j++) { // 遍历每一行
        //取得行j的地址
        uchar* data = image.ptr<uchar>(j);
        // 遍历当前行每一个元素
        for (int i = 0; i < nc; i++) {
            // 处理每个元素
            *data &= mask;            // 掩码，将后n位设置为0
            *data++ += div >> 1;      // 加上div/2
            // 元素处理结束
        } // 一行结束
    }
}


//通用版本，允许用户选择是否要采用就地处理
void colorReduce(const cv::Mat &image,    //  输入图像
                 cv::Mat &result,        //  输出图像
                 int div = 64) {
    // 构建一个大小和类型都与输入图像相同的矩阵
    result.create(image.rows, image.cols, image.type());
    
    int nr = image.rows; // 行数
    // 每行的元素数量
    int nc = image.cols * image.channels();
    
    for (int j = 0; j < nr; j++) { // 遍历
        // 取得行j的输入和输出地址
        const uchar* data_in = image.ptr<uchar>(j);
        uchar* data_out = result.ptr<uchar>(j);
        
        // 遍历当前行每一个元素
        for (int i = 0; i < nc; i++) {
            // 处理每个元素
            data_out[i] = data_in[i] / div * div + div / 2;
            // 元素处理结束
        } // 一行结束
    }
}

int main()
{
    // 读取
    cv::Mat image = cv::imread("mine.png");
    
    cout << image.cols << " x " << image.rows << "=" << image.total() << endl;
    
    // 处理图像
    colorReduce(image, 64);
    //colorReduceWithBitOperation(image, 6);
    
    // 显示图像
    cv::imshow("reduceColor", image);
    
    cv::waitKey(0);
}
