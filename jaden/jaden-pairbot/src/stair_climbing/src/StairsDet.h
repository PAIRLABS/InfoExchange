#ifndef _STAIRSDET_H_
#define _STAIRSDET_H_
#include "ros/ros.h"

#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Float64.h"


#define row 672
#define col 376

static const std::string OPENCV_WINDOW = "RGB window";
static const std::string OPENCV_WINDOW2 = "Depth window";


class StairDetection
{
	public:
		std::string NameSpace;
		StairDetection():it(n)
		{
			d_sub = it.subscribe("/Zed_sim/depth/depth_registered", 10, &StairDetection::imageCallback,this);
			rgb_sub=it.subscribe("/zed/rgb/image_rect_color", 1, &StairDetection::rgbCallback,this);
			pub_d21 = n.advertise<std_msgs::Int32>("d21", 1);
			pub_cnt21 = n.advertise<std_msgs::Float64>("cnt21", 1);
			pub_d4 = n.advertise<std_msgs::Int32>("d4", 1);
			pub_mid = n.advertise<std_msgs::Int32MultiArray>("midxy", 1);

			pub_d5 = n.advertise<std_msgs::Int32>("d5", 1);
			pub_d7 = n.advertise<std_msgs::Int32>("d7", 1);
			pub_d = n.advertise<std_msgs::Int32>("d", 1);
			pub_mid5 = n.advertise<std_msgs::Int32MultiArray>("mid5", 1);
			pub_mid7 = n.advertise<std_msgs::Int32MultiArray>("mid7", 1);
			ros::param::param<std::string>("StairDet_NameSpace", NameSpace, "/Stair_Det");
		}
		void imageCallback(const sensor_msgs::ImageConstPtr& msg);
		void rgbCallback(const sensor_msgs::ImageConstPtr& msg);

		void test();
		void Depth_Arr();
		float depthMD(int x,int y);
		void depth_find(int dc,int *mid_x2,int *mid_y2,int *midx,int *midy,int *di,int *dj,int *dx,int *dsw);
		void depth_find_4(int dc,int *midx,int *midy,int *di,int *dj,int *dx,int *dsw);
		void depth_find_5(int dc,int *midx,int *midy,int *di,int *dj,int *dx,int *dsw);
		void depth_find_7(int dc,int *midx,int *midy,int *di,int *dj,int *dx,int *dsw);
		void SD_Start();

		ros::NodeHandle n;

	private:

		image_transport::ImageTransport it;
		image_transport::Subscriber d_sub;
		image_transport::Subscriber rgb_sub;

		ros::Publisher pub_d21;
		ros::Publisher pub_d4;
		ros::Publisher pub_cnt21;
		ros::Publisher pub_mid;

		ros::Publisher pub_d5;
		ros::Publisher pub_d7;
		ros::Publisher pub_d;
		ros::Publisher pub_mid5;
		ros::Publisher pub_mid7;

		float Arr[row*col];
		int depth[row*col];

};



#endif







