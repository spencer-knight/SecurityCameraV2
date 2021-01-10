#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/video/background_segm.hpp>
#include <iostream>
#include <thread>
#include <chrono>
using namespace std::literals::chrono_literals;

cv::Ptr<cv::BackgroundSubtractor> backsub;
bool motion( cv::Mat &frame) {

    int threshold = 10;
    cv::Mat greyscale;
	cv::Size openingSize(2,2);
    cv::Size closingSize(10, 10);
    cv::Mat openingKernel = cv::getStructuringElement(cv::MORPH_RECT, openingSize);
    cv::Mat closingKernel = cv::getStructuringElement(cv::MORPH_RECT, closingSize);

	cv::cvtColor(frame, greyscale, cv::COLOR_BGR2GRAY);
	backsub->apply(greyscale, greyscale);
	cv::morphologyEx(greyscale, greyscale, cv::MORPH_OPEN, openingKernel);
	cv::morphologyEx( greyscale, greyscale, cv::MORPH_CLOSE, closingKernel);
	int sum = 0;
	for (int i = 0; i < greyscale.rows; i++) {

		for (int j = 0; j < greyscale.cols; j++) {

			sum += greyscale.data[i * greyscale.step + j];
		}
	}

    return (sum / (greyscale.rows * greyscale.cols)) > threshold ? true : false;
}
int main() {
    std::cout << "Program start" << std::endl;
    cv::Mat frame;
    backsub = cv::createBackgroundSubtractorMOG2();
	cv::VideoCapture cap;
    cap.open( 0);
    
	std::cout << "Loop begin" << std::endl; for (;;)
    {
        cap.read(frame);
		cv::imshow("Live", frame);
        if (motion(frame)) {

            std::cout << "Motion detected!" << std::endl;
        }
        if (cv::waitKey(1) >= 0)
            break;
    }
    return 0;
}