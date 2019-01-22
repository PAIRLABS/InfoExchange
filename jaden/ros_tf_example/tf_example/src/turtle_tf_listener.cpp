#include <ros/ros.h>
#include <tf/transform_listener.h>
#include "std_msgs/String.h"
#include <turtlesim/Pose.h>

ros::Publisher turtleT3;
ros::Publisher turtleT4;
ros::Publisher turtletf;

void poseCallback3(const turtlesim::PoseConstPtr& msg)
{
  std::stringstream s3;
  s3 << "t3_x=" << msg->x << ",  t3_y=" << msg->y << ",  t3_theta=" << msg->theta*180/3.14159265359;
  std_msgs::String s3data;
  s3data.data = s3.str();
  turtleT3.publish(s3data);
}
void poseCallback4(const turtlesim::PoseConstPtr& msg)
{
  std::stringstream s4;
  s4 << "t4_x=" << msg->x << ",  t4_y=" << msg->y << ",  t4_theta=" << msg->theta*180/3.14159265359;
  std_msgs::String s4data;
  s4data.data = s4.str();
  turtleT4.publish(s4data);
}
void publishT3PosfromT4(tf::StampedTransform &tr){
  std::stringstream stf;
  stf << "tf_x=" << tr.getOrigin().x() << ", tf_y=" << tr.getOrigin().y() 
  << ", tf_theta=" << atan2(tr.getOrigin().y(), tr.getOrigin().x())*180/3.14159265359
  << ", tf_dist=" << sqrt(pow(tr.getOrigin().x(), 2) + pow(tr.getOrigin().y(), 2));
  std_msgs::String sdata;
  sdata.data = stf.str();
  turtletf.publish(sdata);
}
int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_listener");
  ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug);
  ros::NodeHandle node;

  turtleT3 = node.advertise<std_msgs::String>("turtleT3", 10);
  turtleT4 = node.advertise<std_msgs::String>("turtleT4", 10);
  turtletf = node.advertise<std_msgs::String>("turtletf", 10);

  tf::TransformListener listener;
  
  ros::Subscriber sub3 = node.subscribe("turtle3/pose", 10, &poseCallback3);
  ros::Subscriber sub4 = node.subscribe("turtle4/pose", 10, &poseCallback4);
  

  ros::Rate rate(10.0);
  while (node.ok()){
    tf::StampedTransform transform;
    try{
    listener.lookupTransform("/turtle4", "/turtle3", ros::Time(0), transform);
    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }

    publishT3PosfromT4(transform);


    ros::spinOnce();
    rate.sleep();
  }
  return 0;
};


