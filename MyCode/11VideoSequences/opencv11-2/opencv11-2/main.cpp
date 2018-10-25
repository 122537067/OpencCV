//
//  main.cpp
//  opencv11-2
//
//  Created by one on 2018/10/24.
//  Copyright © 2018 one. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>

int main()
{
    // 打开视频文件
    cv::VideoCapture capture("bike.mp4");
    //cv::VideoCapture capture(0);
    //cv::VideoCapture capture;
    //capture.open("test.mp4");
    
    // 检查打开是否成功
    if (!capture.isOpened())
        return EXIT_FAILURE;
    
    // 取得帧速率,get始终返回double类型数值
    double rate = capture.get(CV_CAP_PROP_FPS);
    if (rate == 0) rate = 60;
    std::cout << "帧速率：" << rate << std::endl;
    // 获得总帧数
    long t = static_cast<long>(
                               capture.get(CV_CAP_PROP_FRAME_COUNT));
    std::cout << "总帧数：" << t << std::endl;
    // 获得视频图像尺寸
    int width = static_cast<int>(
                                 capture.get(CV_CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(
                                  capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    std::cout << "图像尺寸：" << width
    << " × " << height << std::endl;
    
    // 跳转到第100帧
    //double position = 100.0;
    //capture.set(CV_CAP_PROP_POS_FRAMES, position);
    
    bool stop(false); // 用于控制后面的while循环
    cv::Mat frame;      // 存储当前视频帧
    
    // 根据帧速率计算帧之间的等待时间，单位ms
    // 可通过修改此数值，让视频慢进或快进
    // 时长要保证窗口有足够的时间进行刷新
    int delay = 1000 / rate;
    
    // 循环遍历视频中的全部帧
    while (!stop) {
        // 读取下一帧(如果有)
        if (!capture.read(frame))
            break;
        
        cv::imshow("Extracted Frame", frame);
        
        // 等待一段时间，或者通过按键停止
        // 如果是按键停止，则返回按键ASCII码(>=0)
        if (cv::waitKey(delay) >= 0)
            stop = true;
    }
    
    // 关闭视频文件
    // 不是必须的，因为类的析构函数会调用
    capture.release();
    return EXIT_SUCCESS;
}
