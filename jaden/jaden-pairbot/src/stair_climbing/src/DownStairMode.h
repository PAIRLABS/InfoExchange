#ifndef PI 
#define PI 3.14
#endif

#ifndef _DOWNSTAIRMODE_H_
#define _DOWNSTAIRMODE_H_

#include "ros/ros.h"
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Int16MultiArray.h"
#include "std_msgs/Int16.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include <dynamixel_workbench_msgs/JointCommand.h>
#include "sensor_msgs/Range.h"


class DownStairMode
{
	public:
		std::string NameSpace;
		DownStairMode()
		{
			//pub_rs = n.advertise<std_msgs::Int32MultiArray>("robot_speed", 1);
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
			// pub_ht = n.advertise<std_msgs::Float64>("head_top", 1);
			// pub_hb = n.advertise<std_msgs::Float64>("head_bottom", 1);
			joint_command_client = n.serviceClient<dynamixel_workbench_msgs::JointCommand>("joint_command");
			ros::param::param<std::string>("DSM_NameSpace", NameSpace, "/Down_Stair_Mode");

			sub_d = n.subscribe("d", 1, &DownStairMode::dCallback,this);
			sub_d5 = n.subscribe("d5", 1, &DownStairMode::d5Callback,this);
			sub_d7 = n.subscribe("d7", 1, &DownStairMode::d7Callback,this);
			// sub_d7d = n.subscribe("d7d", 1, &DownStairMode::d7dCallback,this);
			sub_mid5 = n.subscribe("midxy5", 1, &DownStairMode::mid5Callback,this);
			sub_mid7 = n.subscribe("midxy7", 1, &DownStairMode::mid7Callback,this);
			sub_us = n.subscribe("/pairbot/sonar", 1, &DownStairMode::ultrasonicCallback,this);
			//sub_us = n.subscribe("ultrasonic", 1, &DownStairMode::ultrasonicCallback,this);
		}
		
		void mode_5();
		void mode_6();
		void mode_7();
		void dynamixel_motor(int X,int Y);
		void legMA(int front,int back);
		void robotMA(int left,int right);
		void robotspeed(int left,int right);
		void legspeed(int front,int back);
		void robotmotion( std::string rm);
		void legmotion(std::string lm);
		void leghome(int front,int back);
		void motor_switch(int number,int serve_status);

		
		void ultrasonicCallback(const sensor_msgs::Range::ConstPtr& msg_us);
		//void ultrasonicCallback(const std_msgs::Float64::ConstPtr& msg_us);
		void d5Callback(const std_msgs::Int32::ConstPtr& msg_d5);
		void dCallback(const std_msgs::Int32::ConstPtr& msg_d);
		void d7Callback(const std_msgs::Int32::ConstPtr& msg_d7);
		// void d7dCallback(const std_msgs::Int32::ConstPtr& msg_d7d);
		void mid5Callback(const std_msgs::Int32MultiArray::ConstPtr& array_mid);
		void mid7Callback(const std_msgs::Int32MultiArray::ConstPtr& array_mid);
		ros::NodeHandle n;
		int d5;
		int d,d7;
		int us;
		int mid_x5,mid_y5;
		int mid_x7,mid_y7;
		
	private:
		
		ros::Subscriber sub_d;
		// ros::Subscriber sub_mid;
		ros::Subscriber sub_d5;
		ros::Subscriber sub_d7;
		// ros::Subscriber sub_d7d;
		ros::Subscriber sub_us;
		ros::Subscriber sub_mid5;
		ros::Subscriber sub_mid7;

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

// #ifndef Demo_stairclimbing_CPP
// #define Demo_stairclimbing_CPP

// #endif