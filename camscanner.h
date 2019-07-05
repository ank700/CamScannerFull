#pragma once
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace cv;
using namespace std;

class CamScanner
{
public:
	// constructor requires image
	CamScanner(Mat &img);
	~CamScanner() {};

	void detectEdges();
	void displayImg();

private:
	// container to store grayscale image
	Mat img2scan;

	// warped image
	Mat warpedimg;

	// largest contour id
	double maxareaid = 0;

	// polygon extreme points (size should be 4)
	vector<Point> corners;

	// warp image
	void warpImage();
};