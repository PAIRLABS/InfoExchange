#include "ros/ros.h"
#include <stdlib.h>
#include <stdio.h>

#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#include "UpStairMode.h"
#include "DownStairMode.h"

// Upstairmode + turn right 90 degree +Downstairmode version


int main(int argc, char **argv)
{
	int count=0;
	ros::init(argc, argv, "Demo_stairclimbing");

	UpStairMode USM;
	DownStairMode DSM;
	USM.n;
	DSM.n;

	printf("Demo start\n");
	ros::Rate r(2);

	//===Start climbing===
	while(ros::ok())
	{
		std::cout<<"d21="<<USM.d21<<" cnt21="<<USM.counter21<<"\n";
		if(USM.d21!=0 && USM.counter21>70)break;
		ros::spinOnce();
		r.sleep();
	}
	USM.start();
	sleep(3);
	USM.legMA(-26000,-27000);
	sleep(3);
	USM.mode_21();
	USM.mode_3();
	USM.mode_4();
	printf("speed\n");
	USM.robotspeed(350,350);
	sleep(1);
	USM.robotmotion("Right");
	sleep(1);
	for (int i=1;i<10;i++)
	{
		sleep(1);
		std::cout<<"sleep for "<<i<<"sec\n";
	}
	USM.robotmotion("Stop");
	sleep(1);
	USM.legMA(5000,5000);
	sleep(1);
	USM.leghome(0,0);
	sleep(1);

	std::cout<<"Prepare to go Downstairs!!\n";

	while(ros::ok())
	{
		printf("\n");
		printf("us=%d\n",DSM.us);
		//printf("d21=%d\n",USM.d21);
		if(DSM.us!=0)
			break;
		ros::spinOnce();
		r.sleep();
	}
	// sleep(3);
	// USM.legMA(-26000,-27000);
	// sleep(3);
	// DSM.mode_5();
	DSM.mode_6();
	DSM.mode_7();
	//===End climbing===
	//===Start obstacle avoiding
	return 0;
}



