#include "ros/ros.h"

#include <stdlib.h>
#include <stdio.h>

#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#include "StairsDet.h"

#define width 672
#define height 376

int main(int argc, char **argv)
{

	ros::init(argc, argv, "StairDetection");
	
	StairDetection SD;
	
	SD.n;
	
		
	
	ros::Rate r(2);
	
	// while(ros::ok())
	// {
	// 	SD.test();
	// }
	while(ros::ok())
	{
		if(cv::waitKey(33) == 27)//press esc to escape
		{ 
			break;
		}
		SD.Depth_Arr();
		for(int ii=0;ii<height-20;ii+=20)
		{
			for(int jj=0;jj<width-20;jj+=20)
			{ 
				printf("%.1f ",SD.depthMD(jj,ii));
			}
			printf("%d\n", ii);
		}
		if(SD.depthMD(width/2,height/2)!=0)
		{ 
			break;
		}
		printf("READY!!!,%f\n",SD.depthMD(width/2,height/2));
		ros::spinOnce();
		r.sleep();
		
		
	}
	SD.SD_Start();
	
	
	return 0;
}



