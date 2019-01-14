#include "ros/ros.h"
#include <sensor_msgs/Image.h>

void DepthImage_Callback(const sensor_msgs::Image::ConstPtr &msg)
{
  //ROS_INFO("I heard: %d",msg->data[2]);
  
  int h=msg->height;
  int w=msg->width;

  float* f=(float*)(&msg->data[0]);
  float dis;

  int i,j;

  for (i=0;i<h;i++)
  {
    for(j=0;j<w;j++)
    {
	dis = f[i*w+j];
//	printf("%.5f",dis);
	
	uint8_t b0 = msg->data[i*w*4+j*4];
        uint8_t b1 = msg->data[i*w*4+j*4+1];
        uint8_t b2 = msg->data[i*w*4+j*4+2];
        uint8_t b3 = msg->data[i*w*4+j*4+3];
	
	if(dis==1.0/0.0 || dis==-1.0/0.0 )
        {
                printf("%f = %d %d %d %d",dis,b0,b1,b2,b3);
                break;
        }

    }
//    printf("\n");
  }

  return;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/master_depth", 100, DepthImage_Callback);
  //ros::Subscriber sub = n.subscribe("/Zed_sim/depth/depth_registered", 100, DepthImage_Callback);
  //ros::Subscriber sub = n.subscribe("/zed/depth/depth_registered", 100, DepthImage_Callback);
  ros::spin();

  return 0;
}

