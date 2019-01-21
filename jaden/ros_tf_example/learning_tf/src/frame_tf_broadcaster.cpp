#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_broadcaster");
  ros::NodeHandle node;

  tf::TransformBroadcaster br;
  tf::Transform transform;

  ros::Rate rate(10.0);
  while (node.ok()){
    transform.setOrigin( tf::Vector3(4, 3, 0.0) );
    transform.setRotation( tf::Quaternion(0, 0, 0, 0.1) );
    
    //transform.setOrigin( tf::Vector3(2.0*sin(ros::Time::now().toSec()), 2.0*cos(ros::Time::now().toSec()), 0.0) );
    //transform.setRotation( tf::Quaternion(0, 0, 0, 1) );
    //br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "turtleBody", "turtleEye"));
    rate.sleep();
  }
  return 0;
};

//#include <ros/ros.h>
// #include <tf/transform_broadcaster.h>
// #include <turtlesim/Pose.h>

// std::string turtle_name;



// void poseCallback(const turtlesim::PoseConstPtr& msg){
//   static tf::TransformBroadcaster br;
//   tf::Transform transform;
//   transform.setOrigin( tf::Vector3(msg->x, msg->y, 0.0) );
//   tf::Quaternion q;
//   q.setRPY(0, 0, msg->theta);
//   transform.setRotation(q);
//   br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "turtleBody", turtle_name));
// }

// int main(int argc, char** argv){
//   ros::init(argc, argv, "my_tf_broadcaster");
//   if (argc != 2){ROS_ERROR("need turtle name as argument"); return -1;};
//   turtle_name = argv[1];

//   ros::NodeHandle node;
//   ros::Subscriber sub = node.subscribe(turtle_name+"/pose", 10, &poseCallback);

//   ros::spin();
//   return 0;
// };