/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/* Authors: Taehun Lim (Darby) */

#include "turtlebot3_gazebo/turtlebot3_depth.h"
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>


Turtlebot3Drive::Turtlebot3Drive()
  : nh_priv_("~")
{
  //Init gazebo ros turtlebot3 node
  ROS_INFO("TurtleBot3 Simulation Node Init");
  ROS_ASSERT(init());
}

Turtlebot3Drive::~Turtlebot3Drive()
{
  updatecommandVelocity(0.0, 0.0);
  ros::shutdown();
}

/*******************************************************************************
* Init function
*******************************************************************************/
bool Turtlebot3Drive::init()
{
  // initialize ROS parameter
  std::string cmd_vel_topic_name = nh_.param<std::string>("cmd_vel_topic_name", "");

  robot_width = nh_.param<float>("robot_width", 0.14);
  
	k = 2 * robot_width/0.14; //the constant of (ratio_width * obstacle's distant)

  // initialize publishers
  cmd_vel_pub_   = nh_.advertise<geometry_msgs::Twist>(cmd_vel_topic_name, 10);
  // initialize subscribers
  depth_scan_sub_  = nh_.subscribe("camera_depth/depth_camera/depth_image", 10, &Turtlebot3Drive::depthScanMsgCallBack, this);  

  return true;
}



void Turtlebot3Drive::depthScanMsgCallBack(const sensor_msgs::Image::ConstPtr &msg)
{
  //ROS_INFO("I heard: %d",msg->data[2]);
  
  int h=msg->height;
  int w=msg->width;
  int i,j;

  for(i=0;i<h;i++)
  {
    for(j=0;j<w;j++)
    {
       float f;
       uint8_t b0 = msg->data[i*w*4+j*4];
       uint8_t b1 = msg->data[i*w*4+j*4+1];
       uint8_t b2 = msg->data[i*w*4+j*4+2];
       uint8_t b3 = msg->data[i*w*4+j*4+3];
       uint8_t uc[] ={b0,b1,b2,b3};
       memcpy(&f, &uc, sizeof(f)); 
       if (f!=f)
         dis[i][j] = 0.5;
			 else if (f <0.3)
				 dis[i][j] = 0.3;
       else
	       dis[i][j] = f;
   }
  }
}

void Turtlebot3Drive::updatecommandVelocity(double linear, double angular)
{
  geometry_msgs::Twist cmd_vel;

  cmd_vel.linear.x  = linear;
  cmd_vel.angular.z = angular;

  cmd_vel_pub_.publish(cmd_vel);
}
/**
uint8_t find_distance(float x)
{
	if(x >= 0.5 && x <= 1)
		return CLOSE;
	else if(x > 1 && x <= 2 )
		return FAR;
	else if(x > 2)
		return CLEAR;
	else
		return VERY_CLOSE;
}
***/

//finally use the minimum of the flag to avoid the closest obstacle
float check_obstacle(float depth, int i, float robot_width)
{
	float dist_flag;
	float pixel_length = depth/0.5/WIDTH; //the length between per pixel
	float avoid_width = robot_width / pixel_length ;//the range that need to avoid 
	
	if(depth >= 2)
		dist_flag = 99;
	else if((float)i >= (WIDTH - avoid_width)/2 && (float)i <= (WIDTH + avoid_width)/2 )//the obastacle is on the path
		dist_flag = depth;
	else//the obsatacle is not on the path
		dist_flag = 99;
	return dist_flag;
}

/*******************************************************************************
* Control Loop function
*******************************************************************************/
bool Turtlebot3Drive::controlLoop()
{
//static uint8_t turtlebot3_state_num = 0;

//set the lookup tf
	tf::StampedTransform transform;
	tf::TransformListener listener;
//pass the param from launch file
	std::string target_base_frame = nh_.param<std::string>("taget_base_frame", "");
  std::string base_footprint = nh_.param<std::string>("base_footprint", "");

	listener.waitForTransform(base_footprint, target_base_frame, ros::Time(0), ros::Duration(10));
	listener.lookupTransform(base_footprint, target_base_frame,	ros::Time(0), transform);
	double dist = sqrt(pow(transform.getOrigin().x(), 2) +
										 pow(transform.getOrigin().y(), 2));
//	ROS_INFO("distance %lf", dist);
	static double ang;
	static double vel;
	float dist_flag[WIDTH];
	
	int h = HEIGHT/2;
/****
	for(int i = 0; i < WIDTH ;i++)
	{
		printf("%.1f", dis[h][i]);
	}
		printf("-------------------------------------\n");
****/

	if (dist <= 0.3)
	{
		ang = 0;
		vel = 0;
		ROS_INFO("ARRIVED");
		updatecommandVelocity(vel, ang);
		return true;
	}
	for (int i = 1; i < WIDTH ; i++)
	{
		dist_flag[i] = check_obstacle(dis[h][i], i, robot_width);
	}
	float min_flag = 99;
	int min_index;
	for (int i = 1; i < WIDTH ; i++)
	{
		if(dist_flag[i] < min_flag)
		{
			min_flag = dist_flag[i];
			min_index = i;
		}
	}
//use the depth be the ratio of speed
	printf("dist:%f min:%f\n", dist, min_flag);



	if (min_flag == 99||dist < min_flag)//go straight and find target
	{	
  	ang = 0.7 * ANGULAR_VELOCITY * atan2(transform.getOrigin().y(),
	  																		transform.getOrigin().x());
		vel = LINEAR_VELOCITY * 3;
		updatecommandVelocity(vel, ang);
	}

	else if (min_index <= WIDTH/2)//obstacle is on the left side
	{
		ang = -1 * ANGULAR_VELOCITY / dis[h][min_index];//turn right
		vel = LINEAR_VELOCITY * dis[h][min_index];
		updatecommandVelocity(vel, ang);
	}
	else if (min_index > WIDTH/2)//right side
	{	
		ang = 1 * ANGULAR_VELOCITY / dis[h][min_index];//turn left
		vel = LINEAR_VELOCITY * dis[h][min_index];
		updatecommandVelocity(vel, ang);
	}
	printf("vel:%f ang:%f\n", vel, ang);



/***************************
  for (int i = (WIDTH/32)*12; i < WIDTH/2; i++)//left side
 	{
 		if(find_distance(dis[h][i]) == VERY_CLOSE)
 		{
    	ang = -1 * ANGULAR_VELOCITY; //turn right
    	vel = 0;
			//ROS_INFO("????");
			goto update;
 		}
 		if(i >= (WIDTH/32)*14)
 		{
 			if(find_distance(dis[h][i]) == CLOSE)
			{
				ang = -1 * ANGULAR_VELOCITY * 0.8;
				vel = LINEAR_VELOCITY * 0.5;
				goto update;
			}
			if(i >= ((WIDTH/32)*15))
			{
				if(find_distance(dis[h][i]) == FAR)
				{
					ang = -1 * ANGULAR_VELOCITY * 0.4;
					vel = LINEAR_VELOCITY;
					goto update;
				}
			}
 		}
 	}	
    	
  for (int i = (WIDTH/32)*20; i > WIDTH/2; i--)//right side
 	{
 		if(find_distance(dis[h][i]) == VERY_CLOSE)
 		{
    	ang = ANGULAR_VELOCITY; //turn left
    	vel = 0;
			goto update;
 		}
 		if(i <= (WIDTH/32)*18)
 		{
 			if(find_distance(dis[h][i]) == CLOSE)
			{
				ang = ANGULAR_VELOCITY * 0.8;
				vel = LINEAR_VELOCITY * 0.5;
				goto update;
			}
			if(i <= ((WIDTH/32)*17))
			{
				if(find_distance(dis[h][i]) == FAR)
				{
					ang = ANGULAR_VELOCITY * 0.4;
					vel = LINEAR_VELOCITY;
					goto update;
				}
			}
  	}
	}
	
//go forward and trace target
  ang = 0.7 * ANGULAR_VELOCITY * atan2(transform.getOrigin().y(),
	  																		transform.getOrigin().x());
	//ang = 0;
	vel = LINEAR_VELOCITY;	
 

update:
		updatecommandVelocity(vel,ang);
************************/
	return true;
}

/*******************************************************************************
* Main function
*******************************************************************************/
int main(int argc, char* argv[])
{
  ros::init(argc, argv, "turtlebot3_follow");
  Turtlebot3Drive turtlebot3_drive;

  ros::Rate loop_rate(120);

  while (ros::ok())
  {
    turtlebot3_drive.controlLoop();
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
