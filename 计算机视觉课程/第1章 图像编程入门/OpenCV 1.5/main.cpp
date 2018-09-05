#include <opencv2/highgui.hpp>
//#include <iostream>

void main()
{
	cv::Mat image = cv::imread("puppy.jpg");
	cv::Mat logo = cv::imread("logo.bmp");
	//cv::Mat mask = cv::imread("logo.bmp", CV_LOAD_IMAGE_GRAYSCALE);  // ����ͼ��
	cv::Mat mask(logo);
	//std::cout << mask.channels() << std::endl;
	cv::namedWindow("ԭͼ");
	cv::imshow("ԭͼ", image);

	cv::namedWindow("Logo");
	cv::imshow("Logo", logo);

	// ����ROI
	cv::Mat imageROI(image, cv::Rect(
		image.cols - logo.cols,  // ROI���Ͻ�x����
		image.rows - logo.rows,	 // ROI���Ͻ�y����								  
		logo.cols,				 // ROI�Ŀ��
		logo.rows				 // ROI�ĸ߶�
	));

	// ����logo
	logo.copyTo(imageROI, mask);

	// ����Logo��ͼ��
	cv::namedWindow("��ͼ");
	cv::imshow("��ͼ", image);

	cv::waitKey(0);	
}