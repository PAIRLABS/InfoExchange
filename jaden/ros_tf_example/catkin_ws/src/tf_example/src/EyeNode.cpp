#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <turtlesim/Pose.h>
#include "std_msgs/String.h"


std::string turtle_name;
ros::Publisher pubEye;
ros::Publisher pubBox;
ros::Publisher pubEB;
ros::Publisher pubEBdata;
tf::Vector3 vecBox;

void poseCallback(const turtlesim::PoseConstPtr& msg){
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(msg->x, msg->y, 0.0) );
  tf::Quaternion q;
  q.setRPY(0, 0, msg->theta);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", turtle_name));

  std::stringstream sEye;
  sEye << "Eye_x=" << msg->x << ",  Eye_y=" << msg->y << ",  Eye_theta=" << msg->theta*180/3.14159265359;
  std_msgs::String sEyedata;
  sEyedata.data = sEye.str();
  pubEye.publish(sEyedata);
}
void poseCallbackBox(const turtlesim::PoseConstPtr& msg)
{
  vecBox.setX(msg->x);
  vecBox.setY(msg->y);
  std::stringstream sbox;
  sbox << "Box_x=" << msg->x << ",  Box_y=" << msg->y;
  std_msgs::String sboxdata;
  sboxdata.data = sbox.str();
  pubBox.publish(sboxdata);
}

void publishBoxPosfromEye(tf::Vector3 &v) {
  std::stringstream svec;
    svec << "EB_x=" << v.x() << ",  EB_y=" << v.y()
    << ", EB_theta=" << atan2(v.y(), v.x())*180/3.14159265359
    << ", EB_dist=" << sqrt(pow(v.x(), 2) + pow(v.y(), 2));
    std_msgs::String svecdata;
    svecdata.data = svec.str();
    pubEB.publish(svecdata);
    geometry_msgs::Vector3 vecEB_msg;
    vecEB_msg.x = v.x();
    vecEB_msg.y = v.y();
    vecEB_msg.z = v.z();
    pubEBdata.publish(vecEB_msg);
}

int main(int argc, char** argv){
  ros::init(argc, argv, "base_Eye_Node");
  if (argc != 2){ROS_ERROR("need turtle name as argument"); return -1;};
  turtle_name = argv[1];

  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe(turtle_name+"/pose", 10, &poseCallback);
  ros::Subscriber subBox = node.subscribe("box/pose", 10, &poseCallbackBox);

  pubEye = node.advertise<std_msgs::String>("pubEye", 10);
  pubBox = node.advertise<std_msgs::String>("pubBox", 10);
  pubEB = node.advertise<std_msgs::String>("pubEB", 10);
  pubEBdata = node.advertise<geometry_msgs::Vector3>("pubEBdata", 10);

  tf::TransformListener listener;
  ros::Rate rate(10.0);
  while (node.ok()){
    tf::StampedTransform transform;
    try{
    listener.waitForTransform("/turtleEye", "/world", ros::Time(0), ros::Duration(5.0));
    listener.lookupTransform("/turtleEye", "/world", ros::Time(0), transform);
    tf::Transform transform_EB = transform;
    tf::Vector3 vecEB;
    vecEB = transform_EB*vecBox;

    publishBoxPosfromEye(vecEB);
    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
    }
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
};
