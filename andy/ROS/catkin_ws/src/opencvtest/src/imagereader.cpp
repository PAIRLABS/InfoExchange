#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencvtest/image.h>
#include <sstream>
#include <iostream>

static const std::string OPENCV_WINDOW = "Image window";

void imageCallback(const opencvtest::image& msg)
{
  //cv::namedWindow(OPENCV_WINDOW);
  cv_bridge::CvImagePtr cv_ptr;
  cv_ptr = cv_bridge::toCvCopy(msg.ros_image, sensor_msgs::image_encodings::BGR8);

  std::cout << msg.data << std::endl;

  cv::imshow(OPENCV_WINDOW, cv_ptr->image);
  cv::waitKey(1);
  


  //cv::destroyWindow(OPENCV_WINDOW);
}
int main(int argc, char** argv)
{
  cv::namedWindow(OPENCV_WINDOW);
  ros::init(argc, argv, "image_reader");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("imageinfo",1000,imageCallback);
  ros::spin();
  cv::destroyWindow(OPENCV_WINDOW);
  return 0;
}
