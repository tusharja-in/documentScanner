#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat imgOriginal, imgCanny, imgGray,imgBlur,imgDilate;

vector<Point> getContours(Mat imgDilate) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<Point> biggest;
	int maxArea = 0;

	findContours(imgDilate, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());


	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);
		
		if (area > 1000) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			if (area > maxArea && conPoly[i].size() == 4) {
				drawContours(imgOriginal, contours, i, Scalar(255, 0, 255), 2);
				maxArea = area;
				biggest = {conPoly[i][0],conPoly[i][1] ,conPoly[i][2] ,conPoly[i][3] };
			}

		}

	}
	return biggest;
}

Mat preProcess(Mat img) {
	Mat kernal = getStructuringElement(MORPH_RECT, Size(2, 2));
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 50, 100);
	dilate(imgCanny, imgDilate, kernal);
	return imgDilate;
}

void drawPoints(vector<Point> points, Scalar color) {
	for (int i = 0; i < points.size(); i++) {
		circle(imgOriginal, points[i], 3, color, 3, FILLED);
		putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN,2, color, 2);
	}
}

void main() {
	string path = "Resources/paper.jpg";
	imgOriginal = imread(path);
	vector<Point> initialPoints,docPoints;
	resize(imgOriginal, imgOriginal, Size(), 0.5, 0.5);

	//pre-processing
	Mat dilatedImage = preProcess(imgOriginal);

	//getting edges(points) of document
	initialPoints=getContours(dilatedImage);
	drawPoints(initialPoints, Scalar(0, 0, 255));

	//reorder the doc points 



	imshow("img dilae", dilatedImage);
	imshow("img", imgOriginal);
	waitKey(0);

}