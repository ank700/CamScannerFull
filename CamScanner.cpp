#include "camscanner.h"

CamScanner::CamScanner(Mat &img)
{
	// check if image is grayscale or not
	if (img.type() != 0)
		cvtColor(img, img2scan, COLOR_BGR2GRAY);
	else
		img2scan = img;
}

void CamScanner::detectEdges()
{
	// smooth image
	Mat blurimg;
	GaussianBlur(img2scan, blurimg, Size(5, 5), 1);

	// convert to binary (since image is already "b&w")
	Mat threshimg;
	threshold(img2scan, threshimg, 128, 255, THRESH_BINARY);

	/*Mat imgscaled;
	resize(threshimg, imgscaled, Size(640, 480));
	imshow("Thresh Image", imgscaled);
	waitKey(0);/**/

	// find contours
	vector<vector<Point>> contours;
	vector<Vec4i> hier;
	findContours(threshimg, contours, hier, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	// Draw contours
	/*Mat drawing = Mat::zeros(threshimg.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(0, 0, 255);
		drawContours(drawing, contours, i, color, 2, 8, hier, 0, Point());
	}

	Mat outscaled;
	resize(drawing, outscaled, Size(640, 480));
	imshow("Contours", outscaled);
	waitKey(0);/**/

	// find largest contour id from area
	double maxarea = 0;

	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours.at(i));

		if (area > maxarea)
		{
			maxarea = area;
			maxareaid = i;
		}
	}

	// Find the convex hull object for each contour
	//vector<Point> hull;
	/*vector<vector<Point>> hull(contours.size());
	convexHull(Mat(contours[maxareaid]), hull[maxareaid], false);

	Scalar color = Scalar(0, 0, 255);
	Mat drawing = Mat::zeros(threshimg.size(), CV_8UC3);
	//drawContours(drawing, contours, maxareaid, color, 1, 8, vector<Vec4i>(), 0, Point());
	drawContours(drawing, hull, maxareaid, color, 1, 8, vector<Vec4i>(), 0, Point());

	Mat outscaled;
	resize(drawing, outscaled, Size(640, 480));
	imshow("Contours", outscaled);
	waitKey(0);*/

	// src - https://stackoverflow.com/questions/47326901/how-to-find-extreme-point-of-the-contour-opencv-c?rq=1
	// compare x axis (this uses lambda operator)
	auto valx = std::minmax_element(contours[maxareaid].begin(), contours[maxareaid].end(), [](Point const& a, Point const& b) {
		return a.x < b.x;
		});

	std::cout << " leftMost [ " << valx.first->x << ", " << valx.first->y << " ]" << std::endl;
	std::cout << " RightMost [ " << valx.second->x << ", " << valx.second->y << " ]" << std::endl;

	// compare y axis
	auto valy = std::minmax_element(contours[maxareaid].begin(), contours[maxareaid].end(), [](Point const& a, Point const& b) {
		return a.y < b.y;
		});

	std::cout << " TopMost [ " << valy.first->x << ", " << valy.first->y << " ]" << std::endl;
	std::cout << " BottomMost [ " << valy.second->x << ", " << valy.second->y << " ]" << std::endl;
	corners.push_back(Point(valx.first->x, valx.first->y)); //left
	corners.push_back(Point(valy.first->x, valy.first->y)); //top
	corners.push_back(Point(valx.second->x, valx.second->y)); //right
	corners.push_back(Point(valy.second->x, valy.second->y)); //bottom

	warpImage();
}

void CamScanner::warpImage()
{
	Mat h;

	vector<Point> rectpts = { {0,0}, {200, 0}, {200,200}, {0,200} };

	h = findHomography(corners, rectpts, RANSAC);
	
	warpPerspective(img2scan, warpedimg, h, Size(320, 240));
}

void CamScanner::displayImg()
{
	imshow("Warped", warpedimg);
	waitKey(0);
}