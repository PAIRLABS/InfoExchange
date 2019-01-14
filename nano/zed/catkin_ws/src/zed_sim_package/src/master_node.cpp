#include "ros/ros.h"
#include <sensor_msgs/Image.h>
#include <std_msgs/Int32.h>
//#include "std_msgs/String.h"

#define SIM  1
#define REAL 2


class Master_node
{
  public:
    Master_node()
    { 
	zed_switcher=SIM;

	sub_sim = n.subscribe("/Zed_sim/depth/depth_registered",100, &Master_node::Sim_DepthImage_Callback,this);
	sub_real = n.subscribe("/zed/depth/depth_registered",100, &Master_node::Real_DepthImage_Callback,this);
	sub_teleop = n.subscribe("/teleop_topic",100, &Master_node::teleop_Callback,this);
	pub = n.advertise<sensor_msgs::Image>("/master_depth", 100);
    }

    void Sim_DepthImage_Callback(const sensor_msgs::Image::ConstPtr &msg)
    {
       //ROS_INFO("Sim ing...");
       if(zed_switcher==SIM)
	{
	  //ROS_INFO("I heard: sim hight= %d", msg->height);
	  pub.publish(msg);
	}
         
       else return;
       //ros::spinOnce();
    }

    void Real_DepthImage_Callback(const sensor_msgs::Image::ConstPtr &msg)
    {
       //ROS_INFO("real ing");
       if(zed_switcher==REAL)
	{
	  //ROS_INFO("I heard: real hight= %d", msg->height);
	  pub.publish(msg);
	}
       else return;
       //ros::spinOnce();
    }

    void teleop_Callback(const std_msgs::Int32::ConstPtr& msg)
    {
	zed_switcher = msg->data;
	ROS_INFO("switch to %d",zed_switcher);	
	return;
    }

    
  private:
    ros::NodeHandle n;
    ros::Publisher pub;
    ros::Subscriber sub_sim;
    ros::Subscriber sub_real;
    ros::Subscriber sub_teleop;
    int zed_switcher;

};



int main(int argc, char **argv)
{
  ros::init(argc, argv, "master_node");
  Master_node master; 
  ros::spin();

  return 0;
}
