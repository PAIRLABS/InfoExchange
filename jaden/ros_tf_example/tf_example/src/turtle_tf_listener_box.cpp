#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include "std_msgs/String.h"
#include <string>
#include <turtlesim/Pose.h>
#include <iostream>

geometry_msgs::Twist twi;
geometry_msgs::Twist boxTwi;
tf::Vector3 vecBox;

void poseCallback3(const turtlesim::PoseConstPtr& msg)
{
  //ROS_INFO("I heard: [%s]", "hi");
  twi.linear.x = msg->x;
  twi.linear.y = msg->y;
  twi.linear.z = msg->theta;
}
void poseCallback4(const turtlesim::PoseConstPtr& msg)
{
  twi.angular.x = msg->x;
  twi.angular.y = msg->y;
  twi.angular.z = msg->theta;
}
void poseCallbackBox(const turtlesim::PoseConstPtr& msg)
{
  //boxTwi.linear.x = msg->x;
  //boxTwi.linear.y = msg->y;
  //boxTwi.linear.z = msg->theta;
  vecBox.setX(msg->x);
  vecBox.setY(msg->y);
}

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_listener");
  ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug);
  ros::NodeHandle node;

  ros::Publisher turtle_vel =
    node.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 10);
  ros::Publisher pubBody =
    node.advertise<std_msgs::String>("pubBody", 10);
  ros::Publisher pubEye =
    node.advertise<std_msgs::String>("pubEye", 10);
  ros::Publisher pubBox =
    node.advertise<std_msgs::String>("pubBox", 10);
  // ros::Publisher pubBB =
  //   node.advertise<std_msgs::String>("pubBB", 10);
  // ros::Publisher pubEB =
  //   node.advertise<std_msgs::String>("pubEB", 10);
  ros::Publisher pubvec =
    node.advertise<std_msgs::String>("pubvec", 10);
  ros::Publisher turtle_trans =
    node.advertise<geometry_msgs::Twist>("turtle2/trans", 10);

  tf::TransformListener listener;
  
  ros::Subscriber sub3 = node.subscribe("turtleBody/pose", 10, &poseCallback3);
  ros::Subscriber sub4 = node.subscribe("turtleEye/pose", 10, &poseCallback4);
  ros::Subscriber subBox = node.subscribe("box/pose", 10, &poseCallbackBox);
  

  ros::Rate rate(10.0);
  while (node.ok()){
    //tf::StampedTransform transform_BB;
    tf::StampedTransform transform_BE;
    

    try{
    // listener.waitForTransform("/turtleEye", "/box",
    //                          ros::Time(0), ros::Duration(1.0));
    // listener.lookupTransform("/turtleEye", "/box",
    //                          ros::Time(0), transform_EB);

    listener.lookupTransform("/turtleBody", "/turtleEye",
                             ros::Time(0), transform_BE);
    
                            
    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }

    

    tf::Vector3 vecBB;
    tf::Transform tfTransform;
    tfTransform = transform_BE;
    vecBB = tfTransform*vecBox;

    geometry_msgs::Twist vel_msg_BE;
    vel_msg_BE.angular.z = atan2(vecBB.y(),
                                    vecBB.x());
    vel_msg_BE.linear.x = sqrt(pow(transform_BE.getOrigin().x(), 2) +
                                  pow(transform_BE.getOrigin().y(), 2));


    std::stringstream svec;
    svec << "vecBB_x=" << vecBB.x() << ",  vecBB_y=" << vecBB.y()
    << ", BB_theta=" << vel_msg_BE.angular.z*180/3.14159265359
    << ", BB_dist=" << vel_msg_BE.linear.x;
    std_msgs::String svecdata;
    svecdata.data = svec.str();
    pubvec.publish(svecdata);


    // geometry_msgs::Twist vel_msg;
    // vel_msg.angular.z = atan2(transform_BB.getOrigin().y(),
    //                                 transform_BB.getOrigin().x());
    // vel_msg.linear.x = sqrt(pow(transform_BB.getOrigin().x(), 2) +
    //                               pow(transform_BB.getOrigin().y(), 2));

    // geometry_msgs::Twist vel_msg_EB;
    // vel_msg_EB.angular.z = atan2(transform_EB.getOrigin().y(),
    //                                 transform_EB.getOrigin().x());
    // vel_msg_EB.linear.x = sqrt(pow(transform_EB.getOrigin().x(), 2) +
    //                               pow(transform_EB.getOrigin().y(), 2));
    
    std::stringstream sBody;
    sBody << "Body_x=" << twi.linear.x << ",  Body_y=" << twi.linear.y << ",  Body_theta=" << twi.linear.z*180/3.14159265359;
    std_msgs::String sBodydata;
    sBodydata.data = sBody.str();
    pubBody.publish(sBodydata);

    std::stringstream sEye;
    sEye << "Eye_x=" << twi.angular.x << ",  Eye_y=" << twi.angular.y << ",  Eye_theta=" << twi.angular.z*180/3.14159265359;
    std_msgs::String sEyedata;
    sEyedata.data = sEye.str();
    pubEye.publish(sEyedata);

    std::stringstream sbox;
    sbox << "Box_x=" << vecBox.x() << ",  Box_y=" << vecBox.y();
    std_msgs::String sboxdata;
    sboxdata.data = sbox.str();
    pubBox.publish(sboxdata);

    // std::stringstream sBB;
    // sBB << "BB_x=" << transform_BB.getOrigin().x() << ", BB_y=" << transform_BB.getOrigin().y() 
    // << ", BB_theta=" << vel_msg.angular.z*180/3.14159265359
    // << ", BB_dist=" << vel_msg.linear.x;
    // std_msgs::String sBBdata;
    // sBBdata.data = sBB.str();
    // pubBB.publish(sBBdata);

    // std::stringstream sEb;
    // sEb << "EB_x=" << transform_EB.getOrigin().x() << ", EB_y=" << transform_EB.getOrigin().y() 
    // << ", EB_theta=" << vel_msg_EB.angular.z*180/3.14159265359
    // << ", EB_dist=" << vel_msg_EB.linear.x;
    // std_msgs::String sEbdata;
    // sEbdata.data = sEb.str();
    // pubEB.publish(sEbdata);

    ros::spinOnce();
    rate.sleep();
  }
  return 0;
};


