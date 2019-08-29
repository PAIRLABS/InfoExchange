#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_broadcaster");
  ros::NodeHandle node;

  tf::TransformBroadcaster br;

  tf::Transform transform_left;
  tf::Transform transform_right;

  ros::Rate rate(10.0);
  while (node.ok()){
    transform_left.setOrigin( tf::Vector3(0.0, 2.0, 0.0) );
    transform_left.setRotation( tf::Quaternion(0, 0, 0, 1) );
    br.sendTransform(tf::StampedTransform(transform_left, ros::Time::now(), "/turtle1", "/turtle1/left_hand"));
    
    transform_right.setOrigin( tf::Vector3(0.0, -2.0, 0.0) );
    transform_right.setRotation( tf::Quaternion(0, 0, 0, 1) );
    br.sendTransform(tf::StampedTransform(transform_right, ros::Time::now(), "/turtle1", "/turtle1/right_hand"));
  /*  
    br.sendTransform(tf::StampedTransform(transform_right, ros::Time::now(), "world", "/robot1/base_footprint"));
    br.sendTransform(tf::StampedTransform(transform_right, ros::Time::now(), "/robot1/base_footprint", "/robot1/base_link"));
    br.sendTransform(tf::StampedTransform(transform_right, ros::Time::now(), "/robot1/base_link", "/robot1/base_scan"));
    br.sendTransform(tf::StampedTransform(transform_right, ros::Time::now(), "/robot1/base_link", "/robot1/another_sensor"));
  
  
    br.sendTransform(tf::StampedTransform(transform_right, ros::Time::now(), "world", "/turtle1"));
    br.sendTransform(tf::StampedTransform(transform_right, ros::Time::now(), "world", "/turtle2"));
*/
    rate.sleep();
  }


  return 0;
};
