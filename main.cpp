#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>

#include <thread>
#include <chrono>

#include "asciicam.h"

using namespace cv;

//Main program entry point
int main(int argc, char** argv)
{
	const int resx = 140; //Number of columns to be printed
	const int resy = 100; //Number of rows to be printed

	//Initialize camera
	AsciiCam ascii(resy,resx);
	ascii.initCam();
	
	while (ascii.getFootage()) //Continue to run for as long as camera output is present
	{
		printf(ascii.toAscii().c_str()); //printf is faster than cout, but can result in some visual glitches with ' ' chars
		
										 
		//std::cout << ascii.toAscii() << std::endl; //cout is slower, but doesn't have these visual glitches
		//ascii.showFootage(); //Shows the actual camera footage in grayscal prior to being converted to ASCII
		//std::this_thread::sleep_for(std::chrono::milliseconds(33)); //Wait 33 milliseconds before continuing execution
	}

	ascii.terminate(); //Release camera and destroy window
	
	return 0;
}
