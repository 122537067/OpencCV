#include <opencv2\highgui\highgui.hpp>
#include <iostream>

// ����ͼ��image����ɫ������ÿ��ͨ������ɫֵ����������ԭ����1/div
void colorReduce(cv::Mat image, int div = 64) {
	// ���ͼ��ÿ��ĩβû�����������أ�����һά���鴦��
	if (image.isContinuous()) {
		image.reshape(1, // �µ�ͨ����
			1);			 // �µ�����
	}

	int nr = image.rows; // ����
	// ÿ�е�Ԫ������(ע�ⲻ����������)
	int nc = image.cols * image.channels();
	
	// ���ͼ��ÿ��ĩβû�����������أ�����һά���鴦��
	// �������ǵ�ʾ��ͼ���������������
	/*if (image.isContinuous()) {
		nc = nc * nr;
		nr = 1;
	}*/

	for (int j = 0; j < nr; j++) { // ����ÿһ��
		// ȡ����j�ĵ�ַ
		uchar* data = image.ptr<uchar>(j);
		// ������ǰ��ÿһ��Ԫ��
		for (int i = 0; i < nc; i++) {
			/*if (j == 209 && i == 563 * 3){
				std::cout << (int)data[i] << std::endl;
				data[i] = data[i] / div * div + div / 2;
				std::cout << (int)data[i] << std::endl;
				}*/
			// ����ÿ��Ԫ��-------------------------
			data[i] = cv::saturate_cast<uchar>(data[i] / div * div + div / 2);
			//data[i] = data[i] - data[i] % div + div / 2;
			//*data = *data / div * div + div / 2;
			//data++;
			// Ԫ�ش������-------------------------
		} // һ�н���
	}
}

// ����ͼ��image����ɫ������ÿ��ͨ������ɫֵ����������ԭ����1/2^n
void colorReduceWithBitOperation(cv::Mat image, int n = 6) {
	int div = pow(2, n);  // ��������
	// ������ȡ����ֵ������
	uchar mask = 0xFF << n; // ���磬��div=16����mask=0xF0

	int nr = image.rows; // ����
	// ÿ�е�Ԫ������(ע�ⲻ����������)
	int nc = image.cols * image.channels();

	for (int j = 0; j < nr; j++) { // ����ÿһ��
		// ȡ����j�ĵ�ַ
		uchar* data = image.ptr<uchar>(j);
		// ������ǰ��ÿһ��Ԫ��
		for (int i = 0; i < nc; i++) {
			// ����ÿ��Ԫ��-------------------------
			*data &= mask;    		// ���룬����nλ��Ϊ0
			*data++ += div >> 1;  	// ����div/2
			// Ԫ�ش������-------------------------
		} // һ�н���
	}
}


// ��ͨ�ð汾�������û�ѡ���Ƿ�Ҫ���þ͵ش���
void colorReduce(const cv::Mat &image,	// ����ͼ��
	cv::Mat &result,		// ���ͼ��
	int div = 64) {
	// ����һ����С�����Ͷ�������ͼ����ͬ�ľ���
	result.create(image.rows, image.cols, image.type());

	int nr = image.rows; // ����
	// ÿ�е�Ԫ������(ע�ⲻ����������)
	int nc = image.cols * image.channels();

	for (int j = 0; j < nr; j++) { // ����ÿһ��
		// ȡ����j������������ַ
		const uchar* data_in = image.ptr<uchar>(j);
		uchar* data_out = result.ptr<uchar>(j);

		// ������ǰ��ÿһ��Ԫ��
		for (int i = 0; i < nc; i++) {
			// ����ÿ��Ԫ��-------------------------
			data_out[i] = data_in[i] / div * div + div / 2;
			// Ԫ�ش������-------------------------
		} // һ�н���
	}
}

void main()
{
	// ��ȡͼ��
	cv::Mat image = cv::imread("castle.jpg");

	//std::cout << image.cols << " x " << image.rows << "=" << image.total() << std::endl;

	// ����ͼ��
	colorReduce(image, 64);
	//colorReduceWithBitOperation(image, 6);

	// ��ʾͼ��
	cv::imshow("������ɫ����", image);

	// �ȴ�����
	cv::waitKey(0);
}