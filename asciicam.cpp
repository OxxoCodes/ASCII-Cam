//#include <opencv2/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>

#include "asciicam.h"

using namespace cv;

//Initialize camera
void AsciiCam::initCam()
{
    VideoCapture cap(0);
    if (!cap.isOpened()) { std::cout << "Error obtaining video feed!" << std::endl; std::exit(0); }

    capture = cap;
}

//Get camera footage. Return true if footage exists, false otherwise
bool AsciiCam::getFootage()
{
    capture >> frame;
    if (frame.empty()) { return false; }
    else { return true; }
}

//Release camera and destroy windows
void AsciiCam::terminate()
{
    capture.release();
    destroyAllWindows();
}

//Show the grayscale camera footage in a seperate window
void AsciiCam::showFootage()
{
    imshow("Frame", frame);
    waitKey(25);
}

//Boost the contrast of the image.
//Any pixel whose value is greater than the threshold has their
//brightness boosted, while any pixel whose value is lower than
//the threshold has their brightness decreased by the same boost value
int AsciiCam::boostContrast(int pixel, int threshold, int boost)
{
    if (pixel >= threshold) { return pixel + boost; }
    else { return pixel - boost; }
}

//Return the average grayscale value of a cell
double AsciiCam::average(int row, int col)
{
    double c = 0.0;
    int sum = 0;
    double avg;
    int boost = 0;
    int threshold = 127;

    for (int celrow = 0; celrow < cell_width; ++celrow)
    {
        for (int celcol = 0; celcol < cell_height; ++celcol)
        {
            sum += boostContrast(frame.at<uchar>(row + celrow, col + celcol), threshold, boost); //Adjust contrast and add value to sum to be averaged
            c += 1;
        }
    }
    avg = sum / c;
    return avg;
}

//Convert a cell's grayscale value to an ASCII char
char AsciiCam::toChar(double *gray)
{
    const int numOfChars = 12;
    char validChars[numOfChars] = { ' ', ' ', '.', ':', '-', '=', '+', '*', 'W', '#', '%', '@' }; //I found that an extra ' ' allowed for deeper and better-looking dark areas.
                                                                                                  //Obviously the extra ' ' is not necessary, and can be removed if you so wish.
    //Convert grayscale value
    return validChars[
        std::min(static_cast<int>(*gray*numOfChars / 255), numOfChars-1)
    ];
}

//Driver function which 
std::string AsciiCam::toAscii()
{
    cvtColor(frame, frame, COLOR_BGR2GRAY); //Convert camera footage to grayscale

    int width = frame.cols;
    int height = frame.rows;
    cell_width = width / rows;
    cell_height = height / cols;

    //Ensure number of rows or columns does not exceed camera resolution
    if (rows > height or cols > width) { return "Too many columns or rows; Ensure the number of columns and rows is lower than the respective width and height of your camera resolution.\n"; }

    double gray;
    std::string output = "";

    //Iterate through cells, convert cell to ASCII character, and return the entire ASCII frame
    for (int row=0; row < height-cell_width; row+=cell_width)
    {
        for (int col=0; col < width-cell_height; col+=cell_height)
        {
            gray = average(row, col);
            output += toChar(&gray);
        }
        output += "\n";
    }
    return output;
}

