#pragma once

#include <string>
#include "opencv2/opencv.hpp"

using namespace cv;

//Class containing all necessary functions necessary for ASCII-Cam to function
class AsciiCam {
public:
	AsciiCam(int r, int c) { rows = r; cols = c; cell_width = 0; cell_height = 0; } //Initialize values
	void initCam();
	bool getFootage();
	void terminate();
	std::string toAscii();
	void showFootage();
	double average(int, int);
	char toChar(double*);
	int boostContrast(int, int, int);
private:
	VideoCapture capture;
	Mat frame;
	int rows, cols, cell_width, cell_height;
};