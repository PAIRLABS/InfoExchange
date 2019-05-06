#ifndef _UPSTAIRMODE_H_
#define _UPSTAIRMODE_H_

#include "ros/ros.h"
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#include <string>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Int16MultiArray.h"
#include "std_msgs/Int16.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include <dynamixel_workbench_msgs/JointCommand.h>
#include <geometry_msgs/Twist.h>

#define PI 3.1415926
class UpStairMode
{
	public:
	    std::string NameSpace;
		UpStairMode()
		{
			pub_rs = n.advertise<std_msgs::Int32MultiArray>("robot_speed", 1);
			pub_ls = n.advertise<std_msgs::Int32MultiArray>("leg_speed", 1);
			pub_rMA = n.advertise<std_msgs::Int32MultiArray>("robot_MA", 1);
			pub_lMA = n.advertise<std_msgs::Int32MultiArray>("leg_MA", 1);
			pub_rHO = n.advertise<std_msgs::Int32MultiArray>("robot_HO", 1);
			pub_rVA = n.advertise<std_msgs::Int32MultiArray>("robot_VA", 1);
			pub_rm = n.advertise<std_msgs::Int32>("robot_motion", 1);
			pub_lm = n.advertise<std_msgs::Int32>("leg_motion", 1);
			pub_lHO = n.advertise<std_msgs::Int32MultiArray>("leg_HO", 1);
			pub_lVA = n.advertise<std_msgs::Int32MultiArray>("leg_VA", 1);
			pub_rMR = n.advertise<std_msgs::Int32MultiArray>("robot_MR", 1);
			pub_MS = n.advertise<std_msgs::Int32MultiArray>("motor_switch", 1);
			pub_vel = n.advertise<geometry_msgs::Twist>("/pairbot/cmd_vel", 1);
			//pub_front_flipper = n.advertise<std_msgs::Float64>("/pairbot/joint_left_front_flipper_controller/command", 1);
			//pub_rear_flipper = n.advertise<std_msgs::Float64>("/pairbot/joint_left_rear_flipper_controller/command", 1);
			pub_workbenchV = n.advertise<std_msgs::Float64>("/pairbot/joint_scan_joint_controller/command", 1);
			//pub_teleopKey = n.advertise<std_msgs::String>("teleopKey", 1);

			
            joint_command_client = n.serviceClient<dynamixel_workbench_msgs::JointCommand>("joint_command");
			ros::param::param<std::string>("USM_NameSpace", NameSpace, "/Up_Stair_Mode");
			// pub_ht = n.advertise<std_msgs::Float64>("head_top", 1);
			// pub_hb = n.advertise<std_msgs::Float64>("head_bottom", 1);

			sub_d21 = n.subscribe("d21", 1, &UpStairMode::d21Callback,this);
			sub_d4 = n.subscribe("d4", 1, &UpStairMode::d4Callback,this);
			sub_cnt21 = n.subscribe("cnt21", 1, &UpStairMode::cnt21Callback,this);
			sub_mid = n.subscribe("midxy", 1, &UpStairMode::midCallback,this);
			
		}
		void start();
		int mode_21();
		void mode_3();
		void mode_4();
		void legMA(int front,int back);
		void robotMA(int left,int right);
		void robotspeed(int left,int right);
		void legspeed(int front,int back);
		void robotmotion( std::string rm);
		void legmotion(std::string lm);
		void leghome(int front,int back);
		void motor_switch(int number,int serve_status);

		void d21Callback(const std_msgs::Int32::ConstPtr& msg_d21);
		void d4Callback(const std_msgs::Int32::ConstPtr& msg_d4);
		void cnt21Callback(const std_msgs::Int32::ConstPtr& msg_cnt21);
		void midCallback(const std_msgs::Int32MultiArray::ConstPtr& array_mid);

		ros::NodeHandle n;
		int d21,d4;
		int counter21;
		int mid_x,mid_y;
		geometry_msgs::Twist twist;
        
		
	private:
		ros::Subscriber sub_d21;
		ros::Subscriber sub_d4;
		ros::Subscriber sub_cnt21;
		ros::Subscriber sub_mid;

		ros::Publisher pub_rs ;
		ros::Publisher pub_ls ;
		ros::Publisher pub_rMA ;
		ros::Publisher pub_lMA ;
		ros::Publisher pub_rHO ;
		ros::Publisher pub_rVA ;
		ros::Publisher pub_MS;
		ros::Publisher pub_rm ;
		ros::Publisher pub_lm ;

		ros::Publisher pub_lHO ;
		ros::Publisher pub_lVA ;
		ros::Publisher pub_rMR ;
		ros::Publisher pub_vel ;
		ros::Publisher pub_workbenchV;
		// ros::Publisher pub_front_flipper;
		// ros::Publisher pub_rear_flipper;
		// ros::Publisher pub_teleopKey;

		// ros::Publisher pub_ht ;
		// ros::Publisher pub_hb ;
		ros::ServiceClient joint_command_client;
		int r_motion;
		int l_motion;
		int r_speed[2];
		int l_speed[2];
		int r_MA[2];
		int l_MA[2];
		int r_VA[2];
		int l_VA[2];
		int r_HO[2];
		int l_HO[2];
		int MS[2];
		

//---robot_motion---
//1=fwd
//2=back
//3=left
//4=right
//0=stop
//-------------------	 	 
//---lef_motion---
//1=front up
//2=front down 
//3=back up
//4=back down
//0=stop
//-------------------
};

#endif
