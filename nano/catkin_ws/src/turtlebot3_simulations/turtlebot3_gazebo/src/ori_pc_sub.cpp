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
	 dis[i][j] = f;
   	
	if(f==1.0/0.0 || f==-1.0/0.0 || f!=f)
	{
		printf("%f = %d %d %d %d",f,b0,b1,b2,b3);
		break;
	}
	
    }
    
  }
  int flag =0;
  int cnt=0;
  for (i=0;i<h;i++)
  {
    for(j=0;j<w;j++)
    {
	//if(dis[i][j]==dis[i][j])
	//if(dis[i][j]==1.0/0.0 || dis[i][j]==-1.0/0.0)        
	//{   
	   // printf("hahahahaha"); 
	 //   printf("%.5lf ", dis[i][j]);
            //flag=1;
	    //cnt++;
	//}
    }
    //if(flag==1) {printf("\n cnt = %d \n",cnt); break; }
  }

//printf("-----------------------------------\n"); 

  //return f;

  return;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;

  //ros::Subscriber sub = n.subscribe("/camera_depth/depth_camera/depth_image", 100, DepthImage_Callback);
  ros::Subscriber sub = n.subscribe("/zed/depth/depth_registered", 100, DepthImage_Callback);
  ros::spin();

  return 0;
}

