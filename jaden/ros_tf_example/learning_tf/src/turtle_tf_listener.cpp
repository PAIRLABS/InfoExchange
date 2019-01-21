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

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_listener");
  ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug);
  ros::NodeHandle node;

  ros::service::waitForService("spawn");
  ros::ServiceClient add_turtle =
    node.serviceClient<turtlesim::Spawn>("spawn");
  turtlesim::Spawn srv;
  //add_turtle.call(srv);       //disable spawn turtle2

  ros::Publisher turtle_vel =
    node.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 10);
  ros::Publisher turtleT3 =
    node.advertise<std_msgs::String>("turtleT3", 10);
  ros::Publisher turtleT4 =
    node.advertise<std_msgs::String>("turtleT4", 10);
  ros::Publisher turtletf =
    node.advertise<std_msgs::String>("turtletf", 10);
  ros::Publisher turtle_trans =
    node.advertise<geometry_msgs::Twist>("turtle2/trans", 10);

  tf::TransformListener listener;
  
  ros::Subscriber sub3 = node.subscribe("turtle3/pose", 10, &poseCallback3);
  ros::Subscriber sub4 = node.subscribe("turtle4/pose", 10, &poseCallback4);
  

  ros::Rate rate(10.0);
  while (node.ok()){
    tf::StampedTransform transform;
    

    try{
    //***disable track 5 seconds ago
    //ros::Time now = ros::Time::now();
    //ros::Time past = ros::Time::now() - ros::Duration(5.0);
    //listener.waitForTransform("/turtle2", now, "/turtle1",
    //                          past, "/world", ros::Duration(1.0));
    //listener.lookupTransform("/turtle2", now, "/turtle1",
    //                         past, "/world", transform);
    //***
    listener.lookupTransform("/turtle4", "/turtle3",
                             ros::Time(0), transform);

                            
    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }

    geometry_msgs::Twist vel_msg;
    vel_msg.angular.z = atan2(transform.getOrigin().y(),
                                    transform.getOrigin().x());
    vel_msg.linear.x = sqrt(pow(transform.getOrigin().x(), 2) +
                                  pow(transform.getOrigin().y(), 2));
    //turtle_vel.publish(vel_msg);


    std::stringstream s3;
    s3 << "t3_x=" << twi.linear.x << ",  t3_y=" << twi.linear.y << ",  t3_theta=" << twi.linear.z*180/3.14159265359;
    std_msgs::String s3data;
    s3data.data = s3.str();
    turtleT3.publish(s3data);

    std::stringstream s4;
    s4 << "t4_x=" << twi.angular.x << ",  t4_y=" << twi.angular.y << ",  t4_theta=" << twi.angular.z*180/3.14159265359;
    std_msgs::String s4data;
    s4data.data = s4.str();
    turtleT4.publish(s4data);

    std::stringstream stf;
    stf << "tf_x=" << transform.getOrigin().x() << ", tf_y=" << transform.getOrigin().y() 
    << ", tf_theta=" << vel_msg.angular.z*180/3.14159265359
    << ", tf_dist=" << vel_msg.linear.x;
    std_msgs::String sdata;
    sdata.data = stf.str();
    turtletf.publish(sdata);

    ros::spinOnce();
    rate.sleep();
  }
  return 0;
};


