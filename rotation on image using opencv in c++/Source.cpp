/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
int main(int argc, char** argv) {
	Mat img = imread("./image/c30c97416dcf2ce17a92b5d0545fb2a4.png");
	if (img.empty()) {
		cout << "Error!";
		return -1;
	}
	const char* windowName = "Original Image";
	namedWindow(windowName, WINDOW_AUTOSIZE);
	imshow(windowName, img);
	const char* windowName1 = "RotatedImage";
	namedWindow(windowName, WINDOW_AUTOSIZE);
	int iAngle = 180;
	createTrackbar("Angle", windowName1, &iAngle, 360);
	int iImageHieght = img.rows / 2;
	int iImagewidth = img.cols / 2;
	while (true) {
		Mat matRotation = getRotationMatrix2D(Point(iImagewidth, iImageHieght), (iAngle - 180), 1);
		Mat imgRotated;
		warpAffine(img, imgRotated, matRotation, img.size());
		imshow(windowName1, imgRotated);
		int iRet = waitKey(30);
		if (iRet == 27) {
			break;
		}
	}
	destroyWindow(windowName);
	destroyWindow(windowName1);
	return 0;

}
#include<opencv2/opencv.hpp>

using namespace cv;

int main()
{
	// Load the image
	Mat imgOriginal = imread("./image/c30c97416dcf2ce17a92b5d0545fb2a4.png", 1);

	//show the original image
	String pzOriginalImage = "Original Image";
	namedWindow(pzOriginalImage,WINDOW_NORMAL);
	imshow(pzOriginalImage, imgOriginal);

	String pzRotatedImage = "Rotated Image";
	namedWindow(pzRotatedImage, WINDOW_NORMAL);

	int iAngle = 180;
	createTrackbar("Angle", pzRotatedImage, &iAngle, 540);

	int iImageHieght = imgOriginal.rows / 2;
	int iImageWidth = imgOriginal.cols / 2;

	while (1)
	{
		Mat matRotation = getRotationMatrix2D(Point(iImageWidth, iImageHieght), (iAngle - 180), 1);

		// Rotate the image
		Mat imgRotated;
		warpAffine(imgOriginal, imgRotated, matRotation, imgOriginal.size());

		imshow(pzRotatedImage, imgRotated);

		int iRet = waitKey(10);
		if (iRet == 7)
		{
			break;
		}
	}
	destroyWindow(pzOriginalImage);
	destroyWindow(pzRotatedImage);
	return 0;
}
*/
// CPP program to detects face in a video 

// Include required header files from OpenCV 
#include "/programfies/include/opencv2/objdetect.hpp" 
#include "/usr/local/include/opencv2/highgui.hpp" 
#include "/usr/local/include/opencv2/imgproc.hpp" 
#include <iostream> 

using namespace std;
using namespace cv;

// Function for Face Detection 
void detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade, double scale);
string cascadeName, nestedCascadeName;

int main(int argc, const char** argv)
{
	// VideoCapture class for playing video for which faces to be detected 
	VideoCapture capture;
	Mat frame, image;

	// PreDefined trained XML classifiers with facial features 
	CascadeClassifier cascade, nestedCascade;
	double scale = 1;

	// Load classifiers from "opencv/data/haarcascades" directory 
	nestedCascade.load("../../haarcascade_eye_tree_eyeglasses.xml");

	// Change path before execution 
	cascade.load("../../haarcascade_frontalcatface.xml");

	// Start Video..1) 0 for WebCam 2) "Path to Video" for a Local Video 
	capture.open(0);
	if (capture.isOpened())
	{
		// Capture frames from video and detect faces 
		cout << "Face Detection Started...." << endl;
		while (1)
		{
			capture >> frame;
			if (frame.empty())
				break;
			Mat frame1 = frame.clone();
			detectAndDraw(frame1, cascade, nestedCascade, scale);
			char c = (char)waitKey(10);

			// Press q to exit from window 
			if (c == 27 || c == 'q' || c == 'Q')
				break;
		}
	}
	else
		cout << "Could not Open Camera";
	return 0;
}

void detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade,
	double scale)
{
	vector<Rect> faces, faces2;
	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY); // Convert to Gray Scale 
	double fx = 1 / scale;

	// Resize the Grayscale Image 
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	// Detect faces of different sizes using cascade classifier 
	cascade.detectMultiScale(smallImg, faces, 1.1,
		2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	// Draw circles around the faces 
	for (size_t i = 0; i < faces.size(); i++)
	{
		Rect r = faces[i];
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = Scalar(255, 0, 0); // Color for Drawing tool 
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width * 0.5) * scale);
			center.y = cvRound((r.y + r.height * 0.5) * scale);
			radius = cvRound((r.width + r.height) * 0.25 * scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(img, cvPoint(cvRound(r.x * scale), cvRound(r.y * scale)),
				cvPoint(cvRound((r.x + r.width - 1) * scale),
					cvRound((r.y + r.height - 1) * scale)), color, 3, 8, 0);
		if (nestedCascade.empty())
			continue;
		smallImgROI = smallImg(r);

		// Detection of eyes int the input image 
		nestedCascade.detectMultiScale(smallImgROI, nestedObjects, 1.1, 2,
			0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		// Draw circles around eyes 
		for (size_t j = 0; j < nestedObjects.size(); j++)
		{
			Rect nr = nestedObjects[j];
			center.x = cvRound((r.x + nr.x + nr.width * 0.5) * scale);
			center.y = cvRound((r.y + nr.y + nr.height * 0.5) * scale);
			radius = cvRound((nr.width + nr.height) * 0.25 * scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
	}

	// Show Processed Image with detected faces 
	imshow("Face Detection", img);
}
