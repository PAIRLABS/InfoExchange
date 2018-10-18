#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencvtest/image.h>
#include <sstream>
#include <iostream>

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  ros::Publisher pub;

  int count = 0;
public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/usb_cam/image_raw", 1,
      &ImageConverter::imageCb, this);
    pub = nh_.advertise<opencvtest::image>("imageinfo", 1);

  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    //publish message
    opencvtest::image img_msg;
    img_msg.ros_image = (sensor_msgs::Image)*msg;
    std::stringstream ss;
    ss << "imagetag for test: " << count;
    img_msg.data = ss.str();
    
    count++;
    // Output modified video stream
    pub.publish(img_msg);
    
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
