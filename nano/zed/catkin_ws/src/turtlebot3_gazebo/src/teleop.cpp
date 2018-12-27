#include "ros/ros.h"
#include <std_msgs/Int32.h>

#include <signal.h>
#include <termios.h>
#include <stdio.h>

#define KEYCODE_1 0x31
#define KEYCODE_2 0x32

#define SIM  1
#define REAL 2
 
int kfd = 0;
struct termios cooked, raw;

class teleop_node
{
    public:
    teleop_node()
    {
	msg.data = 1;
	teleop_pub = n.advertise<std_msgs::Int32>("/teleop_topic", 100);
    }

    void keyloop();

  private:
    ros::NodeHandle n;
    ros::Publisher teleop_pub;
    std_msgs::Int32 msg;

};

void quit(int sig)
{
  ROS_INFO("quit");
  tcsetattr(kfd, TCSANOW, &cooked);
  ros::shutdown();
  exit(0);
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "teleop_node");   
    teleop_node tnode;
    signal(SIGINT,quit);
    tnode.keyloop();

  return 0;
}

void teleop_node::keyloop()
{
  char c;
  bool dirty=false;
 
  // get the console in raw mode                                                              
  tcgetattr(kfd, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag &=~ (ICANON | ECHO);
  // Setting a new line, then end of file                         
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;
  tcsetattr(kfd, TCSANOW, &raw);
 
  puts("Reading from keyboard");
  puts("---------------------------");
  puts("Press 1(Sim) or 2(Real) to switch");
 
 
  for(;;)
  {
    // get the next event from the keyboard  
    if(read(kfd, &c, 1) < 0)
    {
      perror("read():");
      exit(-1);
    }
    ROS_DEBUG("value: 0x%02X\n", c);
  
    switch(c)
    {
      case KEYCODE_1:
	ROS_INFO("switch to Zed_Sim");
        ROS_DEBUG("Simulated zed");
        msg.data = SIM;
	dirty = true;
        break;
      case KEYCODE_2:
	ROS_INFO("switch to Zed_Real");
        ROS_DEBUG("REAL Zed");
        msg.data = REAL;
	dirty = true;
        break;
    }

    if(dirty==true)
    {
	teleop_pub.publish(msg);
	dirty=false;
	ros::spinOnce();
    }
    
  }
  return;
}
