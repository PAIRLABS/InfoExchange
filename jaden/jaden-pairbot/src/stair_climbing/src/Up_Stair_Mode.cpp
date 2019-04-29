#include "ros/ros.h"

#include <stdlib.h>
#include <stdio.h>
 
#include <sensor_msgs/image_encodings.h> 
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#include "UpStairMode.h"



int main(int argc, char **argv)
{

	ros::init(argc, argv, "UpStairMode");
	
	UpStairMode USM;
	
	USM.n;
	
		
	
	ros::Rate r(2);
	
	while(ros::ok())
	{
		std::cout<<"d21="<<USM.d21<<" cnt21="<<USM.counter21<<"\n";
		if(USM.d21!=0 && USM.counter21>70)break;
		//printf("d21=%d\n",USM.d21);
		ros::spinOnce();
		r.sleep();
	}
	//USM.start();	

	USM.legMA(-26000,-27000);
	sleep(3);
	printf("before mode_21\n");
	USM.mode_21();
	printf("before mode_3\n");
	USM.mode_3();
	printf("before mode_4\n");
	USM.mode_4();
	return 0;
}



