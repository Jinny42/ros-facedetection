#include "ros/ros.h"                          // ROS Default Header File
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include "facedetectcnn.h"


#define DETECT_BUFFER_SIZE 0x20000
using namespace cv;

int facedetection(Mat image)
{
	//load an image and convert it to gray (single-channel)
#if 0
	Mat image = imread(image_source); 
	if(image.empty())
	{
		fprintf(stderr, "Can not load the image file %s.\n", image_source);
		return -1;
	}
#endif

	int * pResults = NULL; 
    //pBuffer is used in the detection functions.
    //If you call functions in multiple threads, please create one buffer for each thread!
    unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
    if(!pBuffer)
    {
        fprintf(stderr, "Can not alloc buffer.\n");
        return -1;
    }
	

	///////////////////////////////////////////
	// CNN face detection 
	// Best detection rate
	//////////////////////////////////////////
	//!!! The input image must be a RGB one (three-channel)
	//!!! DO NOT RELEASE pResults !!!
	pResults = facedetect_cnn(pBuffer, (unsigned char*)(image.ptr(0)), image.cols, image.rows, (int)image.step);

    printf("%d faces detected.\n", (pResults ? *pResults : 0));

	Mat result_cnn = image.clone();
//	Mat result_cnn = image;
	//print the detection results
	for(int i = 0; i < (pResults ? *pResults : 0); i++)
	{
        short * p = ((short*)(pResults+1))+142*i;
		int x = p[0];
		int y = p[1];
		int w = p[2];
		int h = p[3];
		int confidence = p[4];
		int angle = p[5];

		printf("face_rect=[%d, %d, %d, %d], confidence=%d, angle=%d\n", x,y,w,h,confidence, angle);
		rectangle(result_cnn, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
	}
//	imshow("result_cnn", result_cnn);
	imshow("view", result_cnn);

//	waitKey();

    //release the buffer
    free(pBuffer);

	return 0;
}


void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
//    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
    facedetection(cv_bridge::toCvShare(msg, "bgr8")->image);
    cv::waitKey(30);
  }
  catch (cv_bridge::Exception e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
    
}


int main(int argc, char **argv)                         // Node Main Function
{
  ros::init(argc, argv, "topic_subscriber");            // Initializes Node Name

  ros::NodeHandle nh;                                   // Node handle declaration for communication with ROS system

  cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("/image_raw", 1, imageCallback);
  ros::spin();
  cv::destroyWindow("view");


  return 0;
}
