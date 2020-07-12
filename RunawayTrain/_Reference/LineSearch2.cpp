#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define PI 3.1415926

using namespace std;
using namespace cv;

class LineFinder {
private:
	cv::Mat img; // 원 영상
	cv::Mat canvus;
	std::vector<cv::Vec4i> lines; // 선을 감지하기 위한 마지막 점을 포함한 벡터
	double deltaRho;
	double deltaTheta; // 누산기 해상도 파라미터
	int minVote; // 선을 고려하기 전에 받아야 하는 최소 투표 개수
	double minLength; // 선에 대한 최소 길이
	double maxGap; // 선에 따른 최대 허용 간격

public:
	LineFinder() : deltaRho(1), deltaTheta(PI / 180), minVote(10), minLength(0.), maxGap(0.) {}
	// 기본 누적 해상도는 1각도 1화소 
	// 간격이 없고 최소 길이도 없음

   // 해당 세터 메소드들

   // 누적기에 해상도 설정
	void setAccResolution(double dRho, double dTheta) {
		deltaRho = dRho;
		deltaTheta = dTheta;
	}

	// 투표 최소 개수 설정
	void setMinVote(int minv) {
		minVote = minv;
	}

	// 선 길이와 간격 설정
	void setLineLengthAndGap(double length, double gap) {
		minLength = length;
		maxGap = gap;
	}

	// 허프 선 세그먼트 감지를 수행하는 메소드
	// 확률적 허프 변환 적용
	std::vector<cv::Vec4i> findLines(cv::Mat& binary) {
		lines.clear();
		cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
		return lines;
	} // cv::Vec4i 벡터를 반환하고, 감지된 각 세그먼트의 시작과 마지막 점 좌표를 포함.

	// 위 메소드에서 감지한 선을 다음 메소드를 사용해서 그림
	// 영상에서 감지된 선을 그리기
	void drawDetectedLines(cv::Mat& image, cv::Scalar color = cv::Scalar(0, 0, 255)) {

		// 선 그리기
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

		// 캐니 알고리즘 적용
		cv::Mat contours;
		cv::Canny(image, contours, 125, 350);

		LineFinder ld; // 인스턴스 생성

		// 확률적 허프변환 파라미터 설정하기
		ld.setLineLengthAndGap(100, 20);
		ld.setMinVote(80);

		// 선을 감지하고 그리기
		std::vector<cv::Vec4i> li = ld.findLines(contours);
		ld.drawDetectedLines(image);
		cv::namedWindow("Detected Lines with HoughP");
		cv::imshow("Detected Lines with HoughP", image);

		cv::waitKey(1);
	}

	return 0;
}