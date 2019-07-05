#include "camscanner.h"

int main()
{
	Mat loadimg = imread("sample1.png", 1);

	CamScanner scanner(loadimg);

	scanner.detectEdges();
	scanner.displayImg();

	return 0;
}