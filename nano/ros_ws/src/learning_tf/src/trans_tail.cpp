 #include <ros/ros.h>
 #include <geometry_msgs/PointStamped.h>
 #include <geometry_msgs/QuaternionStamped.h>
 #include <geometry_msgs/Vector3Stamped.h>
 #include <tf/transform_listener.h>
 #include <tf/transform_datatypes.h>
  

 void quaternion_to_rpt(geometry_msgs::Quaternion geo_qua,geometry_msgs::Quaternion geo_qua2){


 	tf::Quaternion tf_qua;
    quaternionMsgToTF(geo_qua , tf_qua);
    tf::Matrix3x3 m(tf_qua);
	double roll, pitch, yaw;
	m.getRPY(roll, pitch, yaw);

	tf::Quaternion tf_qua2;
    quaternionMsgToTF(geo_qua2 , tf_qua2);
    tf::Matrix3x3 m2(tf_qua2);
	double roll2, pitch2, yaw2;
	m2.getRPY(roll2, pitch2, yaw2);
	
	ROS_INFO("t1_RPY:(%.2f,%.2f,%.2f) -> t2_RPY(%.2f,%.2f,%.2f)",roll,pitch,yaw,roll2,pitch2,yaw2);

 }

 void transformPoint(const tf::TransformListener& listener){
     
/*
      geometry_msgs::PointStamped t1_tail;
      
      t1_tail.header.frame_id = "/turtle1";
      t1_tail.header.stamp = ros::Time();
      t1_tail.point.x = -1.0;
      t1_tail.point.y = 0.0;
      t1_tail.point.z = 0.0;
      try{
          geometry_msgs::PointStamped t2_vision;
          listener.transformPoint("/turtle2", t1_tail, t2_vision);
 
          ROS_INFO("t1_tail: (%.2f, %.2f. %.2f) -----> t2_vision: (%.2f, %.2f, %.2f) at time %.2f",
          t1_tail.point.x, t1_tail.point.y, t1_tail.point.z,
          t2_vision.point.x, t2_vision.point.y, t2_vision.point.z, t2_vision.header.stamp.toSec());
      }
      catch(tf::TransformException& ex){
          ROS_ERROR("Received an exception trying to transform a point from \"t1_tail\" to \"t2_vision\": %s", ex.what());
      }
*/

      geometry_msgs::QuaternionStamped t1_quaternion;
      
      t1_quaternion.header.frame_id = "/turtle1";
      t1_quaternion.header.stamp = ros::Time();
      t1_quaternion.quaternion =  tf::createQuaternionMsgFromRollPitchYaw(0,0,1.047); //60 degree

      try{
          geometry_msgs::QuaternionStamped t2_vision;
          listener.transformQuaternion("/turtle2", t1_quaternion, t2_vision);
 
          ROS_INFO("t1_quaternion: (%.2f, %.2f, %.2f, %.2f) -----> t2_vision: (%.2f, %.2f, %.2f, %.2f) at time %.2f",
          t1_quaternion.quaternion.x, t1_quaternion.quaternion.y, t1_quaternion.quaternion.z,t1_quaternion.quaternion.w,
          t2_vision.quaternion.x, t2_vision.quaternion.y, t2_vision.quaternion.z, t2_vision.quaternion.w, t2_vision.header.stamp.toSec());
      	
      	  quaternion_to_rpt(t1_quaternion.quaternion,t2_vision.quaternion);
      }
      catch(tf::TransformException& ex){
          ROS_ERROR("Received an exception trying to transform a point from \"t1_quaternion\" to \"t2_vision\": %s", ex.what());
      }

      
	
	/*
	    geometry_msgs::Vector3Stamped t1_vector;
    
      t1_vector.header.frame_id = "/turtle1";
      t1_vector.header.stamp = ros::Time();
 
      t1_vector.vector.x = 2.0;
      t1_vector.vector.y = 0.0;
      t1_vector.vector.z = 0.0;
      try{
          geometry_msgs::Vector3Stamped t2_vision;
          listener.transformVector("/turtle2", t1_vector, t2_vision);
 
          ROS_INFO("t1_vector: (%.2f, %.2f. %.2f) -----> t2_vision: (%.2f, %.2f, %.2f) at time %.2f",
          t1_vector.vector.x, t1_vector.vector.y, t1_vector.vector.z,
          t2_vision.vector.x, t2_vision.vector.y, t2_vision.vector.z, t2_vision.header.stamp.toSec());
      }
      catch(tf::TransformException& ex){
          ROS_ERROR("Received an exception trying to transform a vector from \"t1_vector\" to \"t2_vision\": %s", ex.what());
      }
     
*/


 /*	
 	  tf::Stamped<tf::Point> t2_vision;
	  
	  tf::Point t1_point;
	  t1_point.setValue(1,0,0);
	  tf::Stamped<tf::Point> t1_stamp = tf::Stamped<tf::Point>(t1_point,ros::Time::now(),"/turtle1");
	  
	  listener.transformPoint("/turtle2",t1_stamp,t2_vision );  //strange

	  ROS_INFO("t1_tail: (%.2f, %.2f. %.2f) -----> t2_vision: (%.2f, %.2f, %.2f)",
          t1_stamp.getX(), t1_point.getY(), t1_point.getZ(),
          t1_point.getX(), t1_point.getY(), t1_point.getZ());
*/

  }
 
 int main(int argc, char** argv){
      ros::init(argc, argv, "point_transformer");
      ros::NodeHandle n;

      tf::TransformListener listener(ros::Duration(10));

      ros::Timer timer = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener)));
 
      ros::spin();
 
 }