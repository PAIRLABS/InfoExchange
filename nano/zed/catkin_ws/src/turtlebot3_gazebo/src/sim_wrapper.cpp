#include "ros/ros.h"
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/PointCloud2.h>
#include <std_msgs/Int32.h>
//#include "std_msgs/String.h"

#define SIM  1
#define REAL 2


class Master_node
{
  public:
    Master_node()
    { 

	right_raw = n.subscribe("/Zed_sim/right/image_raw_color",100, &Master_node::right_raw_Callback,this);
	left_info = n.subscribe("/Zed_sim/left/camera_Info",100, &Master_node::left_Info_Callback,this);
	left_raw = n.subscribe("/Zed_sim/left/Image_raw_color",100, &Master_node::left_raw_Callback,this);
	
	//pub = n.advertise<sensor_msgs::Image>("/master_depth", 100);
	right_rect = n.advertise<sensor_msgs::Image>("/Zed_sim/right/image_rect_color", 100);
	
	rgb_Info = n.advertise<sensor_msgs::CameraInfo>("/Zed_sim/rgb/camera_info", 100);
	
	rgb_rect = n.advertise<sensor_msgs::Image>("/Zed_sim/rgb/image_rect_color", 100);
	rgb_raw = n.advertise<sensor_msgs::Image>("/Zed_sim/rgb/image_raw_color", 100);
	left_rect = n.advertise<sensor_msgs::Image>("/Zed_sim/left/image_rect_color", 100);
    }

    void right_raw_Callback(const sensor_msgs::Image::ConstPtr &msg)
    {
       right_rect.publish(msg);
    }

    void left_Info_Callback(const sensor_msgs::CameraInfo::ConstPtr &msg)
    {
       rgb_Info.publish(msg);
    }

    void left_raw_Callback(const sensor_msgs::Image::ConstPtr& msg)
    {
	rgb_rect.publish(msg);
	rgb_raw.publish(msg);
	left_rect.publish(msg);
    }

    
  private:
    ros::NodeHandle n;

    ros::Subscriber right_raw;
    ros::Publisher right_rect;

    ros::Subscriber left_info;
    ros::Publisher rgb_Info;

    ros::Subscriber left_raw;
    ros::Publisher  rgb_rect;
    ros::Publisher  rgb_raw;
    ros::Publisher  left_rect;
};



int main(int argc, char **argv)
{
  ros::init(argc, argv, "Zed_sim_wrapper_node");
  Master_node master; 
  ros::spin();

  return 0;
}
