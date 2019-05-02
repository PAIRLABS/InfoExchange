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

#include "DownStairMode.h"

 
//  extern float top,bottom;
//  extern float topp,bottomm;
//  extern float angg; //34
//  extern float ang;
static float top=134,bottom=45;
static float topp=134,bottomm=45;
static float angg=30; //34
static float ang=0;


void DownStairMode::ultrasonicCallback(const std_msgs::Int32::ConstPtr& msg_us)
{
	us=msg_us->data;
	// printf("us=%d",us);
}
void DownStairMode::d5Callback(const std_msgs::Int32::ConstPtr& msg_d5)
{
	d5=msg_d5->data;
}
void DownStairMode::dCallback(const std_msgs::Int32::ConstPtr& msg_d)
{
	d=msg_d->data;
}
void DownStairMode::d7Callback(const std_msgs::Int32::ConstPtr& msg_d7)
{
	d7=msg_d7->data;
}

// void DownStairMode::midCallback(const std_msgs::Int32MultiArray::ConstPtr& array_mid)
// {
// 	mid_x=*(array_mid->data.begin());
// 	mid_y=*(array_mid->data.begin())+1;
// 	//ROS_INFO("mid:(%d,%d)",mid_x,mid_y);
// }
void DownStairMode::mid5Callback(const std_msgs::Int32MultiArray::ConstPtr& array_mid)
{
	mid_x5=*(array_mid->data.begin());
	mid_y5=*(array_mid->data.begin())+1;
	//ROS_INFO("mid:(%d,%d)",mid_x,mid_y);
}
void DownStairMode::mid7Callback(const std_msgs::Int32MultiArray::ConstPtr& array_mid)
{
	mid_x7=*(array_mid->data.begin());
	mid_y7=*(array_mid->data.begin())+1;
	//ROS_INFO("mid:(%d,%d)",mid_x,mid_y);
}

void DownStairMode::mode_5()	//---Before 'down' stairs alignment
{

	std_msgs::Int32MultiArray robot_speed,leg_speed,robot_MA,leg_MA,robot_HO,robot_VA,robot_MR,leg_HO,leg_VA;

	std_msgs::Int32 robot_motion,leg_motion;

	dynamixel_workbench_msgs::JointCommand joint_command;

	// std_msgs::Float64 head_top,head_bottom;

	robot_speed.data.clear();
	leg_speed.data.clear();
	robot_MA.data.clear();
	leg_MA.data.clear();
	robot_HO.data.clear();
	robot_VA.data.clear();
	robot_MR.data.clear();

	/*---For Down Stairs Alignment(Mode 5) Luck---*/

	/*---Out While---*/
	int d=1000;

    /*---Vel - For Record---*/
	int vel_L=0,vel_R=0;

    //XtionMotor(550,0); //Put in mode 9
	DownStairMode::dynamixel_motor(0,30);
	sleep(1);
	printf("Mode 5 , Before down stairs Alignment \n");
//======Xtion Turn Down====
//	head_bottom.data=bottom;
//	head_top.data=topp+15;
//	pub_hb.publish(head_bottom);
//	pub_ht.publish(head_top);
//	printf("xtion turn down\n");
//	ROS_INFO("bottom:%f,top:%f", head_bottom.data,head_top.data);
//	ros::spinOnce();
//
//    usleep(2000000);
//======Xtion Turn Down====


    vel_L=350;
    vel_R=350;
    //bodyJOGvel(vel_L,vel_R);

	r_speed[0]=vel_L;
	r_speed[1]=vel_R;
	for (int i = 0; i < 2; i++)
	{
		robot_speed.data.push_back(r_speed[i]);
	}
	pub_rs.publish(robot_speed);
	printf("robot_speed publish\n");
   ros::spinOnce();
   robot_speed.data.clear();
   usleep(50000);


   //bodyact("fwd");
	r_motion=1;
	robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	printf("robot_motion publish\n");
	ros::spinOnce();

    usleep(500000);

    while(d>85)
    {


        //legact("stop","stop");
		r_motion=1;
		robot_motion.data=r_motion;
		pub_rm.publish(robot_motion);
		printf("robot_motion publish\n");
		ros::spinOnce();

        usleep(50000);



        if(mid_x5!=0 && mid_y5!=0)
        {

            if(abs(d5)<=22) //25
            {
                printf("mode5 , Alignment OK!\n");

                //bodyact("fwd");
					r_motion=1;
					robot_motion.data=r_motion;
					pub_rm.publish(robot_motion);
					printf("robot_motion publish\n");
					ros::spinOnce();

                usleep(300000);
                break;
            }
            else if(d5<=-23) //26
            {
                printf("d5=%d , Need Left\n",d5);
                //key=1;
                //bodyact("stop");

					r_motion=0;
					robot_motion.data=r_motion;
					pub_rm.publish(robot_motion);
					printf("robot_motion stop\n");
					ros::spinOnce();

                usleep(50000);


                vel_L=200;
                vel_R=200;
                //bodyJOGvel(vel_L,vel_R);
               r_speed[0]=vel_L;
					r_speed[1]=vel_R;
					for (int i = 0; i < 2; i++)
					{
						robot_speed.data.push_back(r_speed[i]);
					}
					pub_rs.publish(robot_speed);
					printf("robot_speed publish\n");
					ros::spinOnce();
               robot_speed.data.clear();
               usleep(50000);


               //bodyact("left");
					r_motion=3;
					robot_motion.data=r_motion;
					pub_rm.publish(robot_motion);
					printf("robot_motion LEFT\n");
					ros::spinOnce();

               usleep(300000);

            }
            else if(d5>=23) //26
            {
                printf("d5=%d , Need Right\n",d5);
                //key=2;
                //bodyact("stop");
					r_motion=0;
					robot_motion.data=r_motion;
					pub_rm.publish(robot_motion);
					printf("robot_motion stop\n");
					ros::spinOnce();


                usleep(50000);


                vel_L=200;
                vel_R=200;
                //bodyJOGvel(vel_L,vel_R);

               r_speed[0]=vel_L;
					r_speed[1]=vel_R;
					for (int i = 0; i < 2; i++)
					{
						robot_speed.data.push_back(r_speed[i]);
					}
					pub_rs.publish(robot_speed);
					printf("robot_speed publish\n");
               ros::spinOnce();
               robot_speed.data.clear();
               usleep(50000);


                //bodyact("right");
					r_motion=4;
					robot_motion.data=r_motion;
					pub_rm.publish(robot_motion);
					printf("robot_motion RIGHT\n");
					ros::spinOnce();

               usleep(300000);

            }
            printf("d:%d\n",d);
            //d=depthMD(mid_x,mid_y+5);
        }//if end

        else //can't find stairs
        {
            printf("Can't Find Stairs! 'Patorl Mode'\n");

            r_motion=0;
				robot_motion.data=r_motion;
				pub_rm.publish(robot_motion);
				printf("robot_motion RIGHT\n");
				ros::spinOnce();
            usleep(300000);

            d=1000;
        }

    }

}//mode5 end

void DownStairMode::mode_6()
{
   std_msgs::Int32MultiArray robot_speed,leg_speed,robot_MA,leg_MA,robot_HO,robot_VA,robot_MR,leg_HO,leg_VA;

	std_msgs::Int32 robot_motion,leg_motion;

	dynamixel_workbench_msgs::JointCommand joint_command;

	robot_speed.data.clear();
	leg_speed.data.clear();
	robot_MA.data.clear();
	leg_MA.data.clear();
	robot_HO.data.clear();
	robot_VA.data.clear();
	robot_MR.data.clear();

   ros::Rate r(2);

   ros::spinOnce();
	r.sleep();

   printf("Mode 6 , Down stairs prepare \n");
    //bodyJOGvel(350,350);

   r_speed[0]=540;
   r_speed[1]=540;
   for (int i = 0; i < 2; i++)
	{
	   robot_speed.data.push_back(r_speed[i]);
   }
	pub_rs.publish(robot_speed);
	printf("robot_speed publish\n");
	ros::spinOnce();
	r.sleep();
   robot_speed.data.clear();
   usleep(50000);

    /*---Check Sensor Connection---*/


	if(us<1)
	{
        printf("*** Sensor Disconnection ***\n\n");

        //bodyact("stop");
        r_motion=0;
	     robot_motion.data=r_motion;
	     pub_rm.publish(robot_motion);
	     printf("robot_motion publish\n");
	     ros::spinOnce();
	     r.sleep();
        usleep(50000);

   }
	else
	{
	    printf("Ready to Close Stairs......\n");
	    //bodyact("fwd");
	    r_motion=1;
	    robot_motion.data=r_motion;
	    pub_rm.publish(robot_motion);
	    printf("robot_motion publish\n");
	    ros::spinOnce();
	    r.sleep();
       usleep(50000);
	}

    /*---If sensor have connection, Do---*/

    while(ros::ok())
    {
        printf("HC sensor data = %d\n",us);

        //bodyact("fwd");
        r_motion=1;
	     robot_motion.data=r_motion;
	     pub_rm.publish(robot_motion);
	     printf("robot_motion publish\n");
	     ros::spinOnce();
	     r.sleep();
        usleep(50000);

        if(us>30)

        {
            usleep(50000);

            //bodyact("stop");
            r_motion=0;
	         robot_motion.data=r_motion;
	         pub_rm.publish(robot_motion);
	         printf("robot_motion publish\n");
	         ros::spinOnce();
	         r.sleep();
            usleep(50000);

            break;
        }

    }

    /*---When close stairs---*/
   printf("Down Stairs Prepare.\n\n");
	//---Down Prepare
	//bodyJOGvel(350,350);
	r_speed[0]=600;
   r_speed[1]=600;
   for (int i = 0; i < 2; i++)
	{
	   robot_speed.data.push_back(r_speed[i]);
   }
	pub_rs.publish(robot_speed);
	printf("robot_speed publish\n");
	ros::spinOnce();
	r.sleep();
   robot_speed.data.clear();
   usleep(30000);//0.5


	//legMAvel(200,200); //80,50
	l_speed[0]=1000;
	l_speed[1]=1000;
	for (int i = 0; i < 2; i++)
	{
		leg_speed.data.push_back(l_speed[i]);
	}
	pub_ls.publish(leg_speed);
	printf("speed publish\n");
	ros::spinOnce();
	r.sleep();
	leg_speed.data.clear();
	usleep(30000);//0.5

	//legcs(0,0);
	l_HO[0]=0;
	l_HO[1]=0;
	for (int i = 0; i < 2; i++)
	{
		robot_HO.data.push_back(l_HO[i]);
	}
	pub_lHO.publish(robot_HO);
	printf("robot_HO publish\n");
	ros::spinOnce();
	r.sleep();
	leg_HO.data.clear();
	usleep(30000);
	usleep(100000);
	//legma(100*300,(90+angg)*300);
	l_MA[0]=135*4550;//Front
	l_MA[1]=60*4550;//Back
	for (int i = 0; i < 2; i++)
	{
		leg_MA.data.push_back(l_MA[i]);
	}
	pub_lMA.publish(leg_MA);
   printf("leg_MA publish\n");
	ros::spinOnce();
	r.sleep();
	leg_MA.data.clear();
    printf("******************** front 135 back 60 sleep 20s******************************\n");
   sleep(20);

	//bodyact("fwd");
	r_motion=1;
   robot_motion.data=r_motion;
   pub_rm.publish(robot_motion);
   printf("robot_motion publish\n");
   ros::spinOnce();
   r.sleep();
    printf("************************* go 1s **********************************\n");
	usleep(1000000);

	//legma(98*300,95*300);
	l_MA[0]=98*4550;
	l_MA[1]=95*4550;
	for (int i = 0; i < 2; i++)
	{
		leg_MA.data.push_back(l_MA[i]);
	}
	pub_lMA.publish(leg_MA);
   printf("leg_MA publish\n");
	ros::spinOnce();
	r.sleep();
	leg_MA.data.clear();
    printf("************************** front 90 back 95 go & sleep 7s ***************************\n");
	usleep(7000000); //5.5sec

	//bodyact("stop");
	r_motion=0;
   robot_motion.data=r_motion;
   pub_rm.publish(robot_motion);
   printf("robot_motion publish\n");
   ros::spinOnce();
   r.sleep();
   usleep(50000);

   printf("END Down Stairs Prepare .\n\n");
}

void DownStairMode::mode_7()	//---'down' stairs alignment
{

	std_msgs::Int32MultiArray robot_speed,leg_speed,robot_MA,leg_MA,robot_HO,robot_VA,robot_MR,leg_HO,leg_VA;

	std_msgs::Int32 robot_motion,leg_motion;

	dynamixel_workbench_msgs::JointCommand joint_command;

	robot_speed.data.clear();
	leg_speed.data.clear();
	robot_MA.data.clear();
	leg_MA.data.clear();
	robot_HO.data.clear();
	robot_VA.data.clear();
	robot_MR.data.clear();

   ros::Rate r(2);

	int d7d=1000;


    /*---Data Record - Open---*/
//    FILE *M7XD=fopen("/home/ubuntu/CodeBlocks/TrackedRobot_Control/Recore Data/M7_XtionDistance.txt","w");
//    FILE *M7PL=fopen("/home/ubuntu/CodeBlocks/TrackedRobot_Control/Recore Data/M7_PixelLocation.txt","w");
//    FILE *M7VL=fopen("/home/ubuntu/CodeBlocks/TrackedRobot_Control/Recore Data/M7_VelLeft.txt","w");
//    FILE *M7VR=fopen("/home/ubuntu/CodeBlocks/TrackedRobot_Control/Recore Data/M7_VelRight.txt","w");
//    FILE *M7clcr=fopen("/home/ubuntu/CodeBlocks/TrackedRobot_Control/Recore Data/M7_clcr.txt","w");
//    FILE *M7DD=fopen("/home/ubuntu/CodeBlocks/TrackedRobot_Control/Recore Data/M7_downDistance.txt","w");
    /*---Vel - For Record---*/

    int vel_L=600,vel_R=600;

    //XtionMotor(600,0);

  	bottomm=bottom;
	topp=top - 0;
	joint_command.request.unit = "rad";
	joint_command.request.id = 1;
	joint_command.request.goal_position = bottomm*PI/180;
	joint_command_client.call(joint_command);
	joint_command.request.unit = "rad";
	joint_command.request.id = 2;
	joint_command.request.goal_position = topp*PI/180;
	joint_command_client.call(joint_command);
	ROS_INFO("bottom:%f,top:%f", bottomm,topp);
	usleep(500000);

    usleep(2000000);

    printf("Mode 7 , Down stairs alignment \n");

    //bodyact("fwd");
    	r_motion=1;
	   robot_motion.data=r_motion;
	   pub_rm.publish(robot_motion);
	   printf("robot_motion publish\n");
	   ros::spinOnce();
	   r.sleep();

    usleep(30000);

    int d_counter = 0;
   while(ros::ok())
    //while(d7d>800)//700
    {

   //bodyact("fwd");
      r_speed[0]=vel_L;
	   r_speed[1]=vel_R;
	   for (int i = 0; i < 2; i++)
	   {
	   	robot_speed.data.push_back(r_speed[i]);
	   }
	   pub_rs.publish(robot_speed);
	   printf("robot_speed publish\n");
      ros::spinOnce();
	   r.sleep();
      robot_speed.data.clear();
      usleep(50000);

	   r_motion=1;
	   robot_motion.data=r_motion;
	   pub_rm.publish(robot_motion);
	   printf("robot_motion publish\n");
	   ros::spinOnce();
	   r.sleep();
      usleep(30000);

        if(mid_x7!=0&&mid_y7!=0)
        {

            if(abs(d7)<=50)
            {

                //printf("error:%d , Alignment OK!\n",d7);

                vel_L=600;
                vel_R=600;
                //bodyJOGvel(vel_L,vel_R);
                //bodyJOGvel(1000,1000); //650,650
               r_speed[0]=vel_L;
	            r_speed[1]=vel_R;
	            for (int i = 0; i < 2; i++)
	            {
	   	         robot_speed.data.push_back(r_speed[i]);
	            }
	            pub_rs.publish(robot_speed);
	            printf("robot_speed publish\n");
	            ros::spinOnce();
	            r.sleep();
	            robot_speed.data.clear();
               usleep(50000);

               r_motion=1;
	            robot_motion.data=r_motion;
	            pub_rm.publish(robot_motion);
	            printf("robot_motion publish\n");
	            ros::spinOnce();
	            r.sleep();
               usleep(30000);
//                bodyact("fwd");
//                usleep(30000);
            }
            else if(d7<=-51)
            {

                //printf("error:%d , need some left\n",d7);

                vel_L=600;
                vel_R=550;
                //bodyJOGvel(vel_L,vel_R);
                //bodyJOGvel(850,1000); //250,250
               r_speed[0]=vel_L;
	            r_speed[1]=vel_R;
	            for (int i = 0; i < 2; i++)
	            {
	   	         robot_speed.data.push_back(r_speed[i]);
	            }
	            pub_rs.publish(robot_speed);
	            printf("robot_speed publish\n");
	            ros::spinOnce();
	            r.sleep();
	            robot_speed.data.clear();
               usleep(50000);


               r_motion=3;
	            robot_motion.data=r_motion;
	            pub_rm.publish(robot_motion);
	            printf("robot_motion publish\n");
	            ros::spinOnce();
	            r.sleep();
               usleep(30000);

            }
            else if(d7>=51)
            {
                printf("error:%d , need some right\n",d7);

                vel_L=600;
                vel_R=550;
                //bodyJOGvel(vel_L,vel_R);
                //bodyJOGvel(1000,850);
               r_speed[0]=vel_L;
	            r_speed[1]=vel_R;
	            for (int i = 0; i < 2; i++)
	            {
	   	         robot_speed.data.push_back(r_speed[i]);
	            }
	            pub_rs.publish(robot_speed);
	            printf("robot_speed publish\n");
	            ros::spinOnce();
	            r.sleep();
	            robot_speed.data.clear();
               usleep(50000);


               r_motion=4;
	            robot_motion.data=r_motion;
	            pub_rm.publish(robot_motion);
	            printf("robot_motion publish\n");
	            ros::spinOnce();
	            r.sleep();
               usleep(30000);

            }


        }
        else
        {
				printf("mode_7 : Stairs End\n");

        }
        //printf("d = %d \n",d);
        //fprintf(M7VL,"%d\n",vel_L);
        //fprintf(M7VR,"%d\n",vel_R);
        //fprintf(M7DD,"%d\n",d);

        if(d != 0 && d > 750)
        {
            d_counter++;
            printf("***************** d > 750 *********************************\n");
            if(d_counter >= 3)
                break;
        }

    }
    
	//----down stairs end , prepare-----
	printf("Landind prepare...\n");

	//bodyact("stop");

	r_motion=0;
   robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	printf("robot_motion publish\n");
	ros::spinOnce();
   r.sleep();
	usleep(500000);

	/*---Motor Set---*/

	//fprintf(M7VL,"Down stairs end prepare = %d\n",350);
    //fprintf(M7VR,"Down stairs end prepare = %d\n",350);

	//bodyJOGvel(350,350);
	r_speed[0]=600;
	r_speed[1]=600;
	for (int i = 0; i < 2; i++)
	{
	   robot_speed.data.push_back(r_speed[i]);
	}
	pub_rs.publish(robot_speed);
    printf("robot_speed publish\n");
    ros::spinOnce();
	r.sleep();
    robot_speed.data.clear();
    usleep(500000);


	//legMAvel(50,50); //80,50

	l_speed[0]=1000;
	l_speed[1]=1000;
	for (int i = 0; i < 2; i++)
	{
		leg_speed.data.push_back(l_speed[i]);
	}
	pub_ls.publish(leg_speed);
	printf("speed publish\n");
	ros::spinOnce();
	r.sleep();
	leg_speed.data.clear();

	usleep(500000);

	//legcs(0,0);
	   l_HO[0]=0;
		l_HO[1]=0;

		for (int i = 0; i < 2; i++)
		{
			robot_HO.data.push_back(l_HO[i]);
		}
		pub_lHO.publish(robot_HO);
		printf("robot_HO publish\n");
		ros::spinOnce();
		r.sleep();
		leg_HO.data.clear();

	usleep(500000);

	//bodyact("fwd");
	r_motion=1;
    	robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();
	usleep(2800000);

	//bodyact("stop");
// 	r_motion=1;
//      robot_motion.data=r_motion;
// 	pub_rm.publish(robot_motion);
// 	printf("robot_motion publish\n");
// 	ros::spinOnce();
// 	r.sleep();
// 	usleep(500000);

	//legma(0,-27000); //T3 up
	   l_MA[0]=0;
		l_MA[1]=-90*4550;
		for (int i = 0; i < 2; i++)
		{
			leg_MA.data.push_back(l_MA[i]);
		}
		pub_lMA.publish(leg_MA);
		printf("leg_MA publish\n");
		ros::spinOnce();
		r.sleep();
		leg_MA.data.clear();


	usleep(500000); //0.2

	//bodyact("fwd");
	r_motion=1;
    robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();
	usleep(3500000);

	//legma(-28000,-27000);
	l_MA[0]=-90*4550;
	l_MA[1]=-90*4550;
	for (int i = 0; i < 2; i++)
	{
		leg_MA.data.push_back(l_MA[i]);
	}
	pub_lMA.publish(leg_MA);
	printf("leg_MA publish\n");
	ros::spinOnce();
	r.sleep();
	leg_MA.data.clear();

	usleep(3000000);

	//bodyact("fwd");
	r_motion=1;
   robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();
	usleep(1000000);

//	bodyact("stop");
   r_motion=0;
   robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();
	usleep(1000000);


	// usleep(50000);

	//XtionMotor(0,0);

	bottomm=bottom;
	topp=top;
	joint_command.request.unit = "rad";
	joint_command.request.id = 1;
	joint_command.request.goal_position = bottomm*PI/180;
	joint_command_client.call(joint_command);
	joint_command.request.unit = "rad";
	joint_command.request.id = 2;
	joint_command.request.goal_position = topp*PI/180;
	joint_command_client.call(joint_command);
	ROS_INFO("bottom:%f,top:%f", bottomm,topp);
	usleep(500000);


	//bodyact("fwd");
	r_motion=1;
    robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();
	usleep(6000000);


	//legMAvel(400,400);

		l_speed[0]=1000;
		l_speed[1]=1000;
		for (int i = 0; i < 2; i++)
		{
			leg_speed.data.push_back(l_speed[i]);
		}
		pub_ls.publish(leg_speed);
		printf("speed publish\n");
		ros::spinOnce();
		r.sleep();
		leg_speed.data.clear();
	   usleep(500000);

	//legma(-28000,-27000);
	   l_MA[0]=-2000;
		l_MA[1]=-2000;
		for (int i = 0; i < 2; i++)
		{
			leg_MA.data.push_back(l_MA[i]);
		}
		pub_lMA.publish(leg_MA);
		printf("leg_MA publish\n");
		ros::spinOnce();
		r.sleep();
		leg_MA.data.clear();

    usleep(500000);

	//bodyact("stop");
	r_motion=0;
    robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();
	usleep(500000);

	DownStairMode::motor_switch(1,0);
	usleep(500000);
	DownStairMode::motor_switch(4,0);
	usleep(500000);
	DownStairMode::motor_switch(1,1);
	usleep(500000);
	DownStairMode::motor_switch(4,1);
	usleep(500000);
	DownStairMode::legMA(-26000,-27000);//leg up
	usleep(1500000);
	DownStairMode::leghome(0,0);
	usleep(500000);//sleep to wait the leg touch ground


	printf("Down stairs END ! \n");



}//mode7 end

//Handy Function
void DownStairMode::dynamixel_motor(int X,int Y)
{
	dynamixel_workbench_msgs::JointCommand joint_command;
	bottomm=bottom-X;
	topp=top-Y;
	joint_command.request.unit = "rad";
	joint_command.request.id = 1;
	joint_command.request.goal_position = bottomm*PI/180;
	joint_command_client.call(joint_command);
	joint_command.request.unit = "rad";
	joint_command.request.id = 2;
	joint_command.request.goal_position = topp*PI/180;
	joint_command_client.call(joint_command);

	printf("ZED turn down\n");
	ROS_INFO("bottom:%f,top:%f", bottomm,topp);
}
void DownStairMode::legMA(int front,int back)
{			
	std_msgs::Int32MultiArray leg_MA;

	l_MA[0]=front;
	l_MA[1]=back;
	for (int i = 0; i < 2; i++)
	{
		leg_MA.data.push_back(l_MA[i]);
	}
	pub_lMA.publish(leg_MA);
	printf("leg_MA publish\n");
	
	ros::spinOnce();          
}
void DownStairMode::robotMA(int left,int right)
{
	std_msgs::Int32MultiArray robot_MA;

	r_MA[0]=left;
	r_MA[1]=right;
	for (int i = 0; i < 2; i++)
	{
		robot_MA.data.push_back(r_MA[i]);
	}
	pub_rMA.publish(robot_MA);
	printf("robot_MA publish\n");
	
	ros::spinOnce();   
}
void DownStairMode::robotspeed(int left,int right)
{
	std_msgs::Int32MultiArray robot_speed;

	r_speed[0]=left;
	r_speed[1]=right;
	for (int i = 0; i < 2; i++)
	{
		robot_speed.data.push_back(r_speed[i]);
	}
	pub_rs.publish(robot_speed);
	printf("robot_speed publish\n");
	
	ros::spinOnce();   
}
void DownStairMode::legspeed(int front,int back)
{
	std_msgs::Int32MultiArray leg_speed;

	l_speed[0]=back;
	l_speed[1]=front;
	for (int i = 0; i < 2; i++)
	{
		leg_speed.data.push_back(l_speed[i]);
	}
	pub_ls.publish(leg_speed);
	printf("legspeed publish\n");
	
	ros::spinOnce();   
}

void DownStairMode::robotmotion(std::string rm)
{
	std_msgs::Int32 robot_motion;


	if  (rm =="Stop")
			robot_motion.data=0;
	else if(rm =="Forward")
			robot_motion.data=1;
	else if(rm =="Backward")
			robot_motion.data=2;
	else if(rm =="Left")
			robot_motion.data=3;
	else if(rm =="Right")
			robot_motion.data=4;
	else
		std::cout<<"error rm input!!\n";

	pub_rm.publish(robot_motion);
	//printf("robot_motion publish\n");
	
	ros::spinOnce();   
}
void DownStairMode::legmotion(std::string lm)
{
	
	std_msgs::Int32 leg_motion;

	if  (lm=="Front_Up")
			leg_motion.data=1;
	else if(lm=="Front_Down")
			leg_motion.data=2;
	else if(lm=="Back_Up")
			leg_motion.data=3;
	else if(lm=="Back_Down")
			leg_motion.data=4;
	else
		std::cout<<"error lm input!!\n";
	

	pub_lm.publish(leg_motion);
	printf("leg_motion publish\n");
	
	ros::spinOnce();   
}
void DownStairMode::leghome(int front,int back)
{
	std_msgs::Int32MultiArray leg_HO;

	l_HO[0]=back;
	l_HO[1]=front;
	for (int i = 0; i < 2; i++)
	{
		leg_HO.data.push_back(l_HO[i]);
	}
	pub_lHO.publish(leg_HO);
	printf("leghome publish\n");
	
	ros::spinOnce();   
}
void DownStairMode::motor_switch(int number,int serve_status)
{
	std_msgs::Int32MultiArray motor_switch;

	MS[0]=number;
	MS[1]=serve_status;
	for (int i = 0; i < 2; i++)
	{
		motor_switch.data.push_back(MS[i]);
	}
	pub_MS.publish(motor_switch);
	printf("motor_switch publish\n");
	
	ros::spinOnce();   
}

