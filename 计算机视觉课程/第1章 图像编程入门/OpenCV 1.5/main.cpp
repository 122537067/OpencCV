#include <opencv2/highgui.hpp>
//#include <iostream>

void main()
{
	cv::Mat image = cv::imread("puppy.jpg");
	cv::Mat logo = cv::imread("logo.bmp");
	//cv::Mat mask = cv::imread("logo.bmp", CV_LOAD_IMAGE_GRAYSCALE);  // 掩码图像
	cv::Mat mask(logo);
	//std::cout << mask.channels() << std::endl;
	cv::namedWindow("原图");
	cv::imshow("原图", image);

	cv::namedWindow("Logo");
	cv::imshow("Logo", logo);

	// 定义ROI
	cv::Mat imageROI(image, cv::Rect(
		image.cols - logo.cols,  // ROI左上角x坐标
		image.rows - logo.rows,	 // ROI左上角y坐标								  
		logo.cols,				 // ROI的宽度
		logo.rows				 // ROI的高度
	));

	// 插入logo
	logo.copyTo(imageROI, mask);

	// 插入Logo后图像
	cv::namedWindow("新图");
	cv::imshow("新图", image);

	cv::waitKey(0);	
}