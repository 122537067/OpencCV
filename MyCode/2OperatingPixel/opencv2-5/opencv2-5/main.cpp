//
//  main.cpp
//  opencv2-5
//
//  Created by one on 2018/9/29.
//  Copyright © 2018 one. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <iostream>

// 使用整数除法
void colorReduce(cv::Mat image, int div = 64) {
    int nr = image.rows; // 行数
    // 每行的元素数量(注意不是像素数量)
    int nc = image.cols * image.channels();
    
    for (int j = 0; j < nr; j++) { // 遍历每一行
        // 取得行j的地址
        uchar* data = image.ptr<uchar>(j);
        // 遍历当前行每一个元素
        for (int i = 0; i < nc; i++) {
            // 处理每个元素-------------------------
            //data[i] = data[i] / div * div + div / 2;
            *data++ = *data / div * div + div / 2;
            // 元素处理结束-------------------------
        } // 一行结束
    }
}

// 使用取模运算符
void colorReduce1(cv::Mat image, int div = 64) {
    int nr = image.rows; // 行数
    // 每行的元素数量(注意不是像素数量)
    int nc = image.cols * image.channels();
    
    for (int j = 0; j < nr; j++) { // 遍历每一行
        // 取得行j的地址
        uchar* data = image.ptr<uchar>(j);
        // 遍历当前行每一个元素
        for (int i = 0; i < nc; i++) {
            // 处理每个元素-------------------------
            data[i] = data[i] - data[i] % div + div / 2;
            // 元素处理结束-------------------------
        } // 一行结束
    }
}

// 使用位运算符，每个通道的颜色值数量削减到原来的1/2^n
void colorReduceWithBitOperation(cv::Mat image, int n = 6) {
    int div = pow(2, n);  // 削减因子
    uchar div2 = div >> 1; // div的一半
    // 用来截取像素值的掩码
    uchar mask = 0xFF << n; // 例如，如div=16，则mask=0xF0
    
    int nr = image.rows; // 行数
    // 每行的元素数量(注意不是像素数量)
    int nc = image.cols * image.channels();
    
    for (int j = 0; j < nr; j++) { // 遍历每一行
        // 取得行j的地址
        uchar* data = image.ptr<uchar>(j);
        // 遍历当前行每一个元素
        for (int i = 0; i < nc; i++) {
            // 处理每个元素-------------------------
            *data &= mask;            // 掩码，将后n位设为0
            *data++ += div2;          // 加上div/2
            // 元素处理结束-------------------------
        } // 一行结束
    }
}

// 允许用户选择是否要采用就地处理
void colorReduce(const cv::Mat &image,    // 输入图像
                 cv::Mat &result,        // 输出图像
                 int div = 64) {
    // 构建一个大小和类型都与输入图像相同的矩阵
    result.create(image.rows, image.cols, image.type());
    
    int nr = image.rows; // 行数
    // 每行的元素数量(注意不是像素数量)
    int nc = image.cols * image.channels();
    
    for (int j = 0; j < nr; j++) { // 遍历每一行
        // 取得行j的输入和输出地址
        const uchar* data_in = image.ptr<uchar>(j);
        uchar* data_out = result.ptr<uchar>(j);
        
        // 遍历当前行每一个元素
        for (int i = 0; i < nc; i++) {
            // 处理每个元素-------------------------
            data_out[i] = data_in[i] / div * div + div / 2;
            // 元素处理结束-------------------------
        } // 一行结束
    }
}

// 使用位运算符，但循环内代码增加
void colorReduceWithBitOperation1(cv::Mat image, int n = 6) {
    int nr = image.rows; // 行数
    
    for (int j = 0; j < nr; j++) { // 遍历每一行
        // 取得行j的地址
        uchar* data = image.ptr<uchar>(j);
        // 遍历当前行每一个元素
        for (int i = 0; i < image.cols * image.channels(); i++) {
            int div = pow(2, n);  // 削减因子
            uchar div2 = div >> 1; // div的一半
            // 用来截取像素值的掩码
            uchar mask = 0xFF << n; // 例如，如div=16，则mask=0xF0
            // 处理每个元素-------------------------
            *data &= mask;            // 掩码，将后n位设为0
            *data++ += div2;          // 加上div/2
            // 元素处理结束-------------------------
        } // 一行结束
    }
}

// 使用迭代器方法
void colorReduce2(cv::Mat image, int div = 64) {
    // 在初始位置获得迭代器
    cv::MatIterator_<cv::Vec3b> it =
    image.begin<cv::Vec3b>();
    // 获得结束位置
    cv::MatIterator_<cv::Vec3b> itend =
    image.end<cv::Vec3b>();
    
    // 遍历所有像素
    for (; it != itend; ++it) {
        // 处理每个元素-------------------------
        (*it)[0] = (*it)[0] / div * div + div / 2;
        (*it)[1] = (*it)[1] / div * div + div / 2;
        (*it)[2] = (*it)[2] / div * div + div / 2;
        // 元素处理结束-------------------------
    }
}

// 使用at方法
void colorReduce3(cv::Mat image, int div = 64) {
    int nr = image.rows; // 行数
    // 每行的像素数量
    int nc = image.cols;
    
    for (int j = 0; j < nr; j++) { // 遍历每一行
        // 遍历当前行每一个元素
        for (int i = 0; i < nc; i++) {
            // 处理每个像素-------------------------
            image.at<cv::Vec3b>(j, i)[0] =
            image.at<cv::Vec3b>(j, i)[0] / div * div + div / 2;
            image.at<cv::Vec3b>(j, i)[1] =
            image.at<cv::Vec3b>(j, i)[1] / div * div + div / 2;
            image.at<cv::Vec3b>(j, i)[2] =
            image.at<cv::Vec3b>(j, i)[2] / div * div + div / 2;
            // 元素处理结束-------------------------
        } // 一行结束
    }
}

// 按一维数组处理方法
void colorReduce4(cv::Mat image, int div = 64) {
    // 如果图像每行末尾没有填充额外像素，则当作一维数组处理
    if (image.isContinuous()) {
        image.reshape(3, // 新的通道数
                      1);             // 新的行数
    }
    
    int nr = image.rows; // 行数
    // 每行的元素数量(注意不是像素数量)
    int nc = image.cols * image.channels();
    
    // 如果图像每行末尾没有填充额外像素，则当作一维数组处理
    // 对于我们的示例图像正好是这种情况
    /*if (image.isContinuous()) {
     nc = nc * nr;
     nr = 1;
     }*/
    
    for (int j = 0; j < nr; j++) { // 遍历每一行
        // 取得行j的地址
        uchar* data = image.ptr<uchar>(j);
        // 遍历当前行每一个元素
        for (int i = 0; i < nc; i++) {
            // 处理每个元素-------------------------
            *data++ = *data / div * div + div / 2;
            // 元素处理结束-------------------------
        } // 一行结束
    }
}

int main()
{
    // 读取图像
    cv::Mat image = cv::imread("test.jpg");
    cv::Mat img = image.clone();   // 克隆一份
    
    /*测试整数除法方法的耗时*/
    int64 start = cv::getTickCount(); // 获得起始时钟周期数
    // 处理图像(使用整数除法)
    colorReduce(img, 64);
    // 计算耗时(单位：秒)
    double duration = (cv::getTickCount() - start)
    / cv::getTickFrequency();
    // 输出耗时
    std::cout << "方法1：使用整数除法   耗时："
    << duration << "秒" << std::endl;
    
    /*测试取模运算符方法的耗时*/
    img = image.clone();   // 再克隆一份
    start = cv::getTickCount(); // 获得起始时钟周期数
    // 处理图像(使用取模运算符方法)
    colorReduce1(img, 64);
    // 计算耗时(单位：秒)
    duration = (cv::getTickCount() - start)
    / cv::getTickFrequency();
    // 输出耗时
    std::cout << "方法2：取模运算符方法   耗时："
    << duration << "秒" << std::endl;
    
    /*测试位运算符方法的耗时*/
    img = image.clone();   // 再克隆一份
    start = cv::getTickCount(); // 获得起始时钟周期数
    // 处理图像(使用位运算符方法)
    colorReduceWithBitOperation(img, 6);
    // 计算耗时(单位：秒)
    duration = (cv::getTickCount() - start)
    / cv::getTickFrequency();
    // 输出耗时
    std::cout << "方法3：位运算符方法   耗时："
    << duration << "秒" << std::endl;
    
    /*测试非就地处理方法的耗时*/
    cv::Mat newImg;   // 新图像
    start = cv::getTickCount(); // 获得起始时钟周期数
    // 处理图像(非就地处理)
    colorReduce(image, newImg, 64);
    // 计算耗时(单位：秒)
    duration = (cv::getTickCount() - start)
    / cv::getTickFrequency();
    // 输出耗时
    std::cout << "方法4：非就地处理方法   耗时："
    << duration << "秒" << std::endl;
    
    /*测试内循环代码增加后的位运算方法的耗时*/
    img = image.clone();   // 再克隆一份
    start = cv::getTickCount(); // 获得起始时钟周期数
    // 处理图像(使用修改后的位运算符方法)
    colorReduceWithBitOperation1(img, 6);
    // 计算耗时(单位：秒)
    duration = (cv::getTickCount() - start)
    / cv::getTickFrequency();
    // 输出耗时
    std::cout << "方法5：内循环代码增加后的位运算方法   耗时："
    << duration << "秒" << std::endl;
    
    /*测试迭代器方法的耗时*/
    img = image.clone();   // 再克隆一份
    start = cv::getTickCount(); // 获得起始时钟周期数
    // 处理图像(使用修改后的位运算符方法)
    colorReduce2(img, 64);
    // 计算耗时(单位：秒)
    duration = (cv::getTickCount() - start)
    / cv::getTickFrequency();
    // 输出耗时
    std::cout << "方法6：迭代器方法   耗时："
    << duration << "秒" << std::endl;
    
    /*测试at方法的耗时*/
    img = image.clone();   // 再克隆一份
    start = cv::getTickCount(); // 获得起始时钟周期数
    // 处理图像(使用at方法)
    colorReduce3(img, 64);
    // 计算耗时(单位：秒)
    duration = (cv::getTickCount() - start)
    / cv::getTickFrequency();
    // 输出耗时
    std::cout << "方法7：使用at方法   耗时："
    << duration << "秒" << std::endl;
    
    /*测试连续图像按一维数组处理方法的耗时*/
    img = image.clone();   // 再克隆一份
    start = cv::getTickCount(); // 获得起始时钟周期数
    // 处理图像(连续图像按一维数组处理方法)
    colorReduce4(img, 64);
    // 计算耗时(单位：秒)
    duration = (cv::getTickCount() - start)
    / cv::getTickFrequency();
    // 输出耗时
    std::cout << "方法8：连续图像按一维数组处理方法   耗时："
    << duration << "秒" << std::endl;
}
