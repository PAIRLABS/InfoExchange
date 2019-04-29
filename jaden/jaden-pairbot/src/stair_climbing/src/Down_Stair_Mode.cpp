#include "ros/ros.h"

#include <stdlib.h>
#include <stdio.h>

#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#include "DownStairMode.h"

#define width 640
#define height 480

int main(int argc, char **argv)
{

	ros::init(argc, argv, "DownStairMode");

	DownStairMode DSM;

	DSM.n;

	ros::Rate r(2);

	// while(ros::ok())
	// {
	// 	if (DSM.d<1000)break;
	// 	ros::spinOnce();
	// 	r.sleep();
	// }


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
	sleep(3);
	DSM.legMA(-26000,-27000);
	sleep(3);
	// DSM.mode_5();
	DSM.mode_6();
	DSM.mode_7();
	return 0;

	return 0;
}



