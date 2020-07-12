#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define PI 3.1415926

using namespace std;
using namespace cv;

class LineFinder {
private:
	cv::Mat img; // �� ����
	cv::Mat canvus;
	std::vector<cv::Vec4i> lines; // ���� �����ϱ� ���� ������ ���� ������ ����
	double deltaRho;
	double deltaTheta; // ����� �ػ� �Ķ����
	int minVote; // ���� ����ϱ� ���� �޾ƾ� �ϴ� �ּ� ��ǥ ����
	double minLength; // ���� ���� �ּ� ����
	double maxGap; // ���� ���� �ִ� ��� ����

public:
	LineFinder() : deltaRho(1), deltaTheta(PI / 180), minVote(10), minLength(0.), maxGap(0.) {}
	// �⺻ ���� �ػ󵵴� 1���� 1ȭ�� 
	// ������ ���� �ּ� ���̵� ����

   // �ش� ���� �޼ҵ��

   // �����⿡ �ػ� ����
	void setAccResolution(double dRho, double dTheta) {
		deltaRho = dRho;
		deltaTheta = dTheta;
	}

	// ��ǥ �ּ� ���� ����
	void setMinVote(int minv) {
		minVote = minv;
	}

	// �� ���̿� ���� ����
	void setLineLengthAndGap(double length, double gap) {
		minLength = length;
		maxGap = gap;
	}

	// ���� �� ���׸�Ʈ ������ �����ϴ� �޼ҵ�
	// Ȯ���� ���� ��ȯ ����
	std::vector<cv::Vec4i> findLines(cv::Mat& binary) {
		lines.clear();
		cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
		return lines;
	} // cv::Vec4i ���͸� ��ȯ�ϰ�, ������ �� ���׸�Ʈ�� ���۰� ������ �� ��ǥ�� ����.

	// �� �޼ҵ忡�� ������ ���� ���� �޼ҵ带 ����ؼ� �׸�
	// ���󿡼� ������ ���� �׸���
	void drawDetectedLines(cv::Mat& image, cv::Scalar color = cv::Scalar(0, 0, 255)) {

		// �� �׸���
		std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();

		while (it2 != lines.end()) {
			cv::Point pt1((*it2)[0], (*it2)[1]);
			cv::Point pt2((*it2)[2], (*it2)[3]);
			cv::line(image, pt1, pt2, color);
			++it2;
		}
	}
};

int main()
{
	VideoCapture cap("roadtest2.mp4");

	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}


	for (;;)
	{
		cv::Mat image;
		cap.read(image);

		resize(image, image, Size(640, 360));

		// ĳ�� �˰��� ����
		cv::Mat contours;
		cv::Canny(image, contours, 125, 350);

		LineFinder ld; // �ν��Ͻ� ����

		// Ȯ���� ������ȯ �Ķ���� �����ϱ�
		ld.setLineLengthAndGap(100, 20);
		ld.setMinVote(80);

		// ���� �����ϰ� �׸���
		std::vector<cv::Vec4i> li = ld.findLines(contours);
		ld.drawDetectedLines(image);
		cv::namedWindow("Detected Lines with HoughP");
		cv::imshow("Detected Lines with HoughP", image);

		cv::waitKey(1);
	}

	return 0;
}