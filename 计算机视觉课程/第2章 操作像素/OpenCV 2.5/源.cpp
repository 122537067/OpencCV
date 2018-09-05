#include <opencv2\highgui\highgui.hpp>
#include <iostream>

// ʹ����������
void colorReduce(cv::Mat image, int div = 64) {
	int nr = image.rows; // ����
	// ÿ�е�Ԫ������(ע�ⲻ����������)
	int nc = image.cols * image.channels();

	for (int j = 0; j < nr; j++) { // ����ÿһ��
		// ȡ����j�ĵ�ַ
		uchar* data = image.ptr<uchar>(j);
		// ������ǰ��ÿһ��Ԫ��
		for (int i = 0; i < nc; i++) {
			// ����ÿ��Ԫ��-------------------------
			//data[i] = data[i] / div * div + div / 2;
			*data++ = *data / div * div + div / 2;
			// Ԫ�ش������-------------------------
		} // һ�н���
	}
}

// ʹ��ȡģ�����
void colorReduce1(cv::Mat image, int div = 64) {
	int nr = image.rows; // ����
						 // ÿ�е�Ԫ������(ע�ⲻ����������)
	int nc = image.cols * image.channels();

	for (int j = 0; j < nr; j++) { // ����ÿһ��
								   // ȡ����j�ĵ�ַ
		uchar* data = image.ptr<uchar>(j);
		// ������ǰ��ÿһ��Ԫ��
		for (int i = 0; i < nc; i++) {
			// ����ÿ��Ԫ��-------------------------
			data[i] = data[i] - data[i] % div + div / 2;
			// Ԫ�ش������-------------------------
		} // һ�н���
	}
}

// ʹ��λ�������ÿ��ͨ������ɫֵ����������ԭ����1/2^n
void colorReduceWithBitOperation(cv::Mat image, int n = 6) {
	int div = pow(2, n);  // ��������
	uchar div2 = div >> 1; // div��һ��
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
			*data++ += div2;  		// ����div/2
			// Ԫ�ش������-------------------------
		} // һ�н���
	}
}

// �����û�ѡ���Ƿ�Ҫ���þ͵ش���
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

// ʹ��λ���������ѭ���ڴ�������
void colorReduceWithBitOperation1(cv::Mat image, int n = 6) {
	int nr = image.rows; // ����

	for (int j = 0; j < nr; j++) { // ����ÿһ��
		// ȡ����j�ĵ�ַ
		uchar* data = image.ptr<uchar>(j);
		// ������ǰ��ÿһ��Ԫ��
		for (int i = 0; i < image.cols * image.channels(); i++) {
			int div = pow(2, n);  // ��������
			uchar div2 = div >> 1; // div��һ��
			// ������ȡ����ֵ������
			uchar mask = 0xFF << n; // ���磬��div=16����mask=0xF0
			// ����ÿ��Ԫ��-------------------------
			*data &= mask;    		// ���룬����nλ��Ϊ0
			*data++ += div2;  		// ����div/2
									// Ԫ�ش������-------------------------
		} // һ�н���
	}
}

// ʹ�õ���������
void colorReduce2(cv::Mat image, int div = 64) {
	// �ڳ�ʼλ�û�õ�����
	cv::MatIterator_<cv::Vec3b> it =
		image.begin<cv::Vec3b>();
	// ��ý���λ��
	cv::MatIterator_<cv::Vec3b> itend =
		image.end<cv::Vec3b>();

	// ������������
	for (; it != itend; ++it) {
		// ����ÿ��Ԫ��-------------------------
		(*it)[0] = (*it)[0] / div * div + div / 2;
		(*it)[1] = (*it)[1] / div * div + div / 2;
		(*it)[2] = (*it)[2] / div * div + div / 2;
		// Ԫ�ش������-------------------------
	}
}

// ʹ��at����
void colorReduce3(cv::Mat image, int div = 64) {
	int nr = image.rows; // ����
	// ÿ�е���������
	int nc = image.cols;

	for (int j = 0; j < nr; j++) { // ����ÿһ��
		// ������ǰ��ÿһ��Ԫ��
		for (int i = 0; i < nc; i++) {
			// ����ÿ������-------------------------
			image.at<cv::Vec3b>(j, i)[0] = 
				image.at<cv::Vec3b>(j, i)[0] / div * div + div / 2;
			image.at<cv::Vec3b>(j, i)[1] =
				image.at<cv::Vec3b>(j, i)[1] / div * div + div / 2;
			image.at<cv::Vec3b>(j, i)[2] =
				image.at<cv::Vec3b>(j, i)[2] / div * div + div / 2;
			// Ԫ�ش������-------------------------
		} // һ�н���
	}
}

// ��һά���鴦����
void colorReduce4(cv::Mat image, int div = 64) {
	// ���ͼ��ÿ��ĩβû�����������أ�����һά���鴦��
	if (image.isContinuous()) {
		image.reshape(3, // �µ�ͨ����
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
			// ����ÿ��Ԫ��-------------------------
			*data++ = *data / div * div + div / 2;
			// Ԫ�ش������-------------------------
		} // һ�н���
	}
}

void main()
{
	// ��ȡͼ��
	cv::Mat image = cv::imread("test.jpg");
	cv::Mat img = image.clone();   // ��¡һ��

	/*�����������������ĺ�ʱ*/
	int64 start = cv::getTickCount(); // �����ʼʱ��������
	// ����ͼ��(ʹ����������)
	colorReduce(img, 64);
	// �����ʱ(��λ����)
	double duration = (cv::getTickCount() - start) 
		/ cv::getTickFrequency();
	// �����ʱ
	std::cout << "����1��ʹ����������   ��ʱ��" 
		<< duration << "��" << std::endl;

	/*����ȡģ����������ĺ�ʱ*/
	img = image.clone();   // �ٿ�¡һ��
	start = cv::getTickCount(); // �����ʼʱ��������
	// ����ͼ��(ʹ��ȡģ���������)
	colorReduce1(img, 64);
	// �����ʱ(��λ����)
	duration = (cv::getTickCount() - start)
		/ cv::getTickFrequency();
	// �����ʱ
	std::cout << "����2��ȡģ���������   ��ʱ��"
		<< duration << "��" << std::endl;

	/*����λ����������ĺ�ʱ*/
	img = image.clone();   // �ٿ�¡һ��
	start = cv::getTickCount(); // �����ʼʱ��������
	// ����ͼ��(ʹ��λ���������)
	colorReduceWithBitOperation(img, 6);
	// �����ʱ(��λ����)
	duration = (cv::getTickCount() - start)
		/ cv::getTickFrequency();
	// �����ʱ
	std::cout << "����3��λ���������   ��ʱ��"
		<< duration << "��" << std::endl;

	/*���ԷǾ͵ش������ĺ�ʱ*/
	cv::Mat newImg;   // ��ͼ��
	start = cv::getTickCount(); // �����ʼʱ��������
	// ����ͼ��(�Ǿ͵ش���)
	colorReduce(image, newImg, 64);
	// �����ʱ(��λ����)
	duration = (cv::getTickCount() - start)
		/ cv::getTickFrequency();
	// �����ʱ
	std::cout << "����4���Ǿ͵ش�����   ��ʱ��"
		<< duration << "��" << std::endl;

	/*������ѭ���������Ӻ��λ���㷽���ĺ�ʱ*/
	img = image.clone();   // �ٿ�¡һ��
	start = cv::getTickCount(); // �����ʼʱ��������
	// ����ͼ��(ʹ���޸ĺ��λ���������)
	colorReduceWithBitOperation1(img, 6);
	// �����ʱ(��λ����)
	duration = (cv::getTickCount() - start)
		/ cv::getTickFrequency();
	// �����ʱ
	std::cout << "����5����ѭ���������Ӻ��λ���㷽��   ��ʱ��"
		<< duration << "��" << std::endl;

	/*���Ե����������ĺ�ʱ*/
	img = image.clone();   // �ٿ�¡һ��
	start = cv::getTickCount(); // �����ʼʱ��������
	// ����ͼ��(ʹ���޸ĺ��λ���������)
	colorReduce2(img, 64);
	// �����ʱ(��λ����)
	duration = (cv::getTickCount() - start)
		/ cv::getTickFrequency();
	// �����ʱ
	std::cout << "����6������������   ��ʱ��"
		<< duration << "��" << std::endl;

	/*����at�����ĺ�ʱ*/
	img = image.clone();   // �ٿ�¡һ��
	start = cv::getTickCount(); // �����ʼʱ��������
	// ����ͼ��(ʹ��at����)
	colorReduce3(img, 64);
	// �����ʱ(��λ����)
	duration = (cv::getTickCount() - start)
		/ cv::getTickFrequency();
	// �����ʱ
	std::cout << "����7��ʹ��at����   ��ʱ��"
		<< duration << "��" << std::endl;

	/*��������ͼ��һά���鴦�����ĺ�ʱ*/
	img = image.clone();   // �ٿ�¡һ��
	start = cv::getTickCount(); // �����ʼʱ��������
	// ����ͼ��(����ͼ��һά���鴦����)
	colorReduce4(img, 64);
	// �����ʱ(��λ����)
	duration = (cv::getTickCount() - start)
		/ cv::getTickFrequency();
	// �����ʱ
	std::cout << "����8������ͼ��һά���鴦����   ��ʱ��"
		<< duration << "��" << std::endl;
}