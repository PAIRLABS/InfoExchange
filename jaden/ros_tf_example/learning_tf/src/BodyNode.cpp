#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <turtlesim/Pose.h>
#include "std_msgs/String.h"


std::string turtle_name;

ros::Publisher pubBody;
ros::Publisher pubBB;
tf::Vector3 vecEB;

void poseCallback(const turtlesim::PoseConstPtr& msg){
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(msg->x, msg->y, 0.0) );
  tf::Quaternion q;
  q.setRPY(0, 0, msg->theta);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", turtle_name));

  std::stringstream sBody;
  sBody << "Body_x=" << msg->x << ",  Body_y=" << msg->y << ",  Body_theta=" << msg->theta*180/3.14159265359;
  std_msgs::String sBodydata;
  sBodydata.data = sBody.str();
  pubBody.publish(sBodydata);
}
void poseCallback_EBdata(const geometry_msgs::Vector3& msg){
  vecEB.setX(msg.x);
  vecEB.setY(msg.y);
}

void publishBoxPosfromBody(tf::Vector3 &v){
  std::stringstream svec;
  svec << "BB_x=" << v.x() << ",  BB_y=" << v.y()
  << ", BB_theta=" << atan2(v.y(), v.x())*180/3.14159265359
  << ", BB_dist=" << sqrt(pow(v.x(), 2) + pow(v.y(), 2));
  std_msgs::String svecdata;
  svecdata.data = svec.str();
  pubBB.publish(svecdata);
}

int main(int argc, char** argv){
  ros::init(argc, argv, "base_Body_Node");
  if (argc != 2){ROS_ERROR("need turtle name as argument"); return -1;};
  turtle_name = argv[1];

  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe(turtle_name+"/pose", 10, &poseCallback);
  ros::Subscriber subEB = node.subscribe("/pubEBdata", 10, &poseCallback_EBdata);

  pubBody = node.advertise<std_msgs::String>("pubBody", 10);
  pubBB = node.advertise<std_msgs::String>("pubBB", 10);

  tf::TransformListener listener;
  ros::Rate rate(10.0);
  while (node.ok()){
    tf::StampedTransform transform;
    try{
    listener.waitForTransform("/turtleBody", "/turtleEye", ros::Time(0), ros::Duration(5.0));
    listener.lookupTransform("/turtleBody", "/turtleEye", ros::Time(0), transform);
    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::spinOnce();
      rate.sleep();
      continue;
    }
    tf::Transform transform_BE = transform;

    tf::Vector3 vecBB;
    vecBB = transform_BE*vecEB;

    publishBoxPosfromBody(vecBB);

    


    ros::spinOnce();
    rate.sleep();
  }
  return 0;
};
