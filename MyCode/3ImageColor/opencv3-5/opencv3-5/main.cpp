//
//  main.cpp
//  opencv3-5
//
//  Created by one on 2018/10/8.
//  Copyright © 2018 one. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void detectHScolor(const cv::Mat &image,    // 输入图像
                   double minHue, double maxHue,            // 色调区间
                   double minSat, double maxSat,            // 饱和度区间
                   cv::Mat &mask) {                        // 输出掩码
    // 转换到HSV空间
    cv::Mat hsv;
    cv::cvtColor(image, hsv, CV_BGR2HSV);
    
    // 分割3个通道，并存进3幅图像
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    // channels[0]是色调
    // channels[1]是饱和度
    // channels[2]是亮度
    
    // 色调掩码
    cv::Mat mask1;    // 小于maxHue
    // channels[0]中像素值小于maxHue则将mask1中对应像素值置255
    cv::threshold(channels[0], mask1, maxHue, 255,
                  cv::THRESH_BINARY_INV);
    cv::Mat mask2;    // 大于minHue
    // channels[0]中像素值小于minHue则将mask2中对应像素值置255
    cv::threshold(channels[0], mask2, minHue, 255,
                  cv::THRESH_BINARY);
    
    cv::Mat hueMask;    // 色调掩码
    if (minHue < maxHue)
        hueMask = mask1 & mask2; // 按位与
    else // 如果区间穿越0度中轴线
        hueMask = mask1 | mask2; // 按位或
    
    // 饱和度掩码
    // channels[1]中像素值小于maxSat则将mask1中对应像素值置255
    cv::threshold(channels[1], mask1, maxSat, 255,
                  cv::THRESH_BINARY_INV);
    // channels[1]中像素值大于minSat则将mask2中对应像素值置255
    cv::threshold(channels[1], mask2, minSat, 255,
                  cv::THRESH_BINARY);
    
    cv::Mat satMask;    // 饱和度掩码
    satMask = mask1 & mask2;
    
    // 组合掩码
    mask = hueMask & satMask;
}


// 在YCrCb空间用椭圆皮肤模型进行肤色检测
void detectSkinColor(const cv::Mat &image,    // 输入图像
                     cv::Mat &mask) {                        // 输出掩码
    
    /*初始化椭圆皮肤模型*/
    // 用于肤色检测的掩码图像，256×256大小，初始为全0
    cv::Mat skinCrCb(cv::Size(256, 256), CV_8UC1, cv::Scalar(0));
    // 在掩码图像中绘制一白色椭圆
    cv::ellipse(skinCrCb, // 目标图像
                cv::Point(113, 155.6), // 椭圆中心坐标
                cv::Size(23.4, 15.2),  // 椭圆长短轴的长度
                43.0,                   // 椭圆长轴与水平方向夹角
                0.0,                   // 起始角度
                360.0,                 // 终止角度
                cv::Scalar(255),       // 椭圆为白色
                -1                       // 线框宽度，为负数表示绘制填充椭圆
                );
    cv::imshow("Elliptical Model", skinCrCb);
    
    // 转换到YCrCb空间
    cv::Mat matYCrCb;
    cv::cvtColor(image, matYCrCb, CV_BGR2YCrCb);
    
    // 有必要的话重新分配mask的数据空间
    mask.create(image.size(), CV_8UC1);
    
    // 遍历图像，检测每个像素颜色是否位于椭圆内部
    for (int j = 0; j < matYCrCb.rows; j++) {
        cv::Vec3b *input = matYCrCb.ptr<cv::Vec3b>(j);
        uchar *output = mask.ptr<uchar>(j);
        for (int i = 0; i < matYCrCb.cols; i++) {
            // 将像素的CrCb值作为坐标访问skinCrCb图像，在椭圆内部则表面是肤色
            if (skinCrCb.at<uchar>(input[i][1], input[i][2]) > 0)
                output[i] = 255;
            else
                output[i] = 0;
        }
    }
    cv::imshow("mask", mask);
}

void AdjustSaturation(const cv::Mat &image, cv::Mat &result, float s)
{
    result.create(image.size(), image.type());
    
    for (int row = 0; row < image.rows; row++) {
        const cv::Vec3b *input = image.ptr<cv::Vec3b>(row);
        cv::Vec3b *output = result.ptr<cv::Vec3b>(row);
        for (int col = 0; col < image.cols; col++) {
            uchar Y = input[col][0] * 0.114 +
            input[col][1] * 0.587 +
            input[col][2] * 0.299;
            output[col] = cv::Vec3b(cv::saturate_cast<uchar>(Y + s * (input[col][0] - Y)),
                                    cv::saturate_cast<uchar>(Y + s * (input[col][1] - Y)),
                                    cv::saturate_cast<uchar>(Y + s * (input[col][2] - Y)));
        }
    }
}

int main()
{
    cv::Mat image = cv::imread("castle.jpg");
    if (image.empty())  // 图像为空?
        return 0;
    cv::imshow("original", image);
    
    // 转换成HSV色彩空间
    cv::Mat hsv;
    cv::cvtColor(image, hsv, CV_BGR2HSV);
    
    // 把三个通道分割进三个图像中
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    
    // channels[0]是色调
    cv::imshow("hue", channels[0]);
    // channels[1]是饱和度
    cv::imshow("saturation", channels[1]);
    // channels[2]是亮度
    cv::imshow("brightness", channels[2]);
    
    // 所有像素的亮度通道设为255
    channels[2] = 255;
    // 重新合并通道
    cv::merge(channels, hsv);
    // 转换回BGR
    cv::Mat newImage;
    cv::cvtColor(hsv, newImage, CV_HSV2BGR);
    cv::imshow("brightness - 255", newImage);
    
    //cv::Mat image = cv::imread("girl.jpg");
    //cv::imshow("原图", image);
    
    //// 检测肤色
    //cv::Mat mask;
    ////detectHScolor(image,
    ////    160, 10, // 色调从320度到20度
    ////    25, 166, // 饱和度从0.1到0.65
    ////    mask);
    //detectSkinColor(image, mask);
    
    //// 显示使用掩码后的图像
    //// 像素值初始化为全黑
    //cv::Mat detected(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
    //image.copyTo(detected, mask); // 仅复制mask为白色的部分
    //cv::imshow("肤色检测结果", detected);
    
    //cv::Mat result;
    //AdjustSaturation(image, result, 0.5);
    //imshow("饱和度调节结果", result);
    
    cv::waitKey(); // 等待按键
}
