#include "ros/ros.h"
#include <sensor_msgs/Image.h>

void DepthImage_Callback(const sensor_msgs::Image::ConstPtr &msg)
{
  //ROS_INFO("I heard: %d",msg->data[2]);
  
  int h=msg->height;
  int w=msg->width;
  
  float dis[h][w];
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
         dis[i][j]=-1;
       else
	 dis[i][j] = f;
  //  printf("%d %d %d %d \n",b0,b1,b2,b3); 
   }
  }

  for (i=0;i<h;i++)
  {
    for(j=0;j<w;j++)
    {
        printf("%.5f ", dis[i][j]);
        //cout << dis[i][j] << " ";
    }
    printf("\n");
  }

printf("-----------------------------------\n"); 

  //return f;

  return;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;

  //ros::Subscriber sub = n.subscribe("/camera_depth/depth_camera/depth_image", 100, DepthImage_Callback);
  ros::Subscriber sub = n.subscribe("/depth_camera/depth_image_raw", 100, DepthImage_Callback);
  ros::spin();

  return 0;
}
