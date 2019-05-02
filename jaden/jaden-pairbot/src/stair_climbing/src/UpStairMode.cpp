#include "ros/ros.h"
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Int16MultiArray.h"
#include "std_msgs/Int16.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"

// #include "ZED_init.h"
#include "UpStairMode.h"

#define  _UPSTAIRMODE_CPP_

// float top=-5,bottom=30;
// float topp=-5,bottomm=30;
// float angg=30; //34
// float ang=0;
static float top=134,bottom=45;
static float topp=134,bottomm=45;
static float angg=30; //34
static float ang=0;


void UpStairMode::d21Callback(const std_msgs::Int32::ConstPtr& msg_d21)
{
	d21=msg_d21->data;
}
void UpStairMode::d4Callback(const std_msgs::Int32::ConstPtr& msg_d4)
{
	d4=msg_d4->data;
}
void UpStairMode::cnt21Callback(const std_msgs::Float64::ConstPtr& msg_cnt21)
{
	counter21=msg_cnt21->data;
}
void UpStairMode::midCallback(const std_msgs::Int32MultiArray::ConstPtr& array_mid)
{
	mid_x=*(array_mid->data.begin());
	mid_y=*(array_mid->data.begin())+1;
	//ROS_INFO("mid:(%d,%d)",mid_x,mid_y);
}


void UpStairMode::start()
{
	ros::Rate r(2);
	while(ros::ok())
	{
		ROS_INFO("d21:%d , cnt21:%f",d21,counter21);
		if(d21!=0&&counter21!=0)break;
		ros::spinOnce();
		r.sleep();
	}
}


int UpStairMode::mode_21()	//---Alignment & Calculated the tilt angle
{
    /*---Time---*/
    //time_t t1 = time(NULL);
	/*---"depth_find" Function Variable---*/
	int i=0,j=0,x=0,y=0,mid_x=0,mid_y=0,sw,mid_x2=0,mid_y2=0;
	/*---For Alignment(Mode 21) Luck---*/
    int key=0;//Left & Right
	int key1=0;//Far & Close
	int Align=0;
	/*---For 1m Alignment---*/
	//int counter21 = 10000;

	/*---Depth Data Variable---*/
	//int d21=0,cont21=0,i3=0,j3=0,y3=0,sum3=0;
	int cont21=0,i3=0,j3=0,y3=0,sum3=0;
	double xx3=0,t21=0,td21=0;
	float z3=0,x3=0;

	/*---Out While---*/
	int d=800;
	int err=15;
	float far_dis=125,close_dis=115,stop_align_dis=100;
	ros::param::get(NameSpace+"/err", err);
	ros::param::get(NameSpace+"/far_dis", far_dis);
	ros::param::get(NameSpace+"/close_dis", close_dis);
	ros::param::get(NameSpace+"/stop_align_dis", stop_align_dis);
	std::cout<<"err="<<err<<"\n";
	std::cout<<"stop_align_dis="<<stop_align_dis<<"\n";
	/*---Vel - For Record---*/
	int vel_L=0,vel_R=0;
	int cnt=0;
			/*---Data Record - Open---*/
		//    FILE *M2XD=fopen("/home/ubuntu/CodeBlocks/TrackedRobot_Control/Recore Data/M2_XtionDistance.txt","w");
		//    FILE *M2PL=fopen("/home/ubuntu/CodeBlocks/TrackedRobot_Control/Recore Data/M2_PixelLocation.txt","w");
		//    FILE *M2VL=fopen("/home/ubuntu/CodeBlocks/TrackedRobot_Control/Recore Data/M2_VelLeft.txt","w");
		//    FILE *M2VR=fopen("/home/ubuntu/CodeBlocks/TrackedRobot_Control/Recore Data/M2_VelRight.txt","w");
		//    FILE *M2dldr=fopen("/home/ubuntu/CodeBlocks/TrackedRobot_Control/Recore Data/M2_dldr.txt","w");



   printf("Mode21,1m Alignment Mode\n");
	//=======ROS============
	std_msgs::Int32MultiArray robot_speed,leg_speed,robot_MA,leg_MA,robot_HO,robot_VA,robot_MR,leg_HO,leg_VA;

	std_msgs::Int32 robot_motion,leg_motion;

	dynamixel_workbench_msgs::JointCommand joint_command;
	std_msgs::Float64 workbenchV;
	std_msgs::Float64 frontflipperang;

	robot_speed.data.clear();
	leg_speed.data.clear();
	robot_MA.data.clear();
	leg_MA.data.clear();
	leg_HO.data.clear();
	robot_HO.data.clear();
	robot_VA.data.clear();
	robot_MR.data.clear();
	
	ros::Rate r(2);

	//=======================
    bottomm=bottom;
	topp=top-15;
	joint_command.request.unit = "rad";
	joint_command.request.id = 1;
	joint_command.request.goal_position = bottomm*PI/180;
	joint_command_client.call(joint_command);
	joint_command.request.unit = "rad";
	joint_command.request.id = 2;
	joint_command.request.goal_position = topp*PI/180;
	joint_command_client.call(joint_command);
	workbenchV.data = (float)(15*PI/180);
	pub_workbenchV.publish(workbenchV);

	printf("ZED turn down\n");
	ROS_INFO("bottom:%f,top:%f", bottomm,topp);
	
	l_MA[0]=-90*4550;
	l_MA[1]=-90*4550;
	for (int i = 0; i < 2; i++)
	{
		leg_MA.data.push_back(l_MA[i]);
		printf("MA:%d\n",l_MA[i]);
	}
	pub_lMA.publish(leg_MA);
	printf("leg_MA publish***************\n");
	
	// l_HO[0]=90;
	// l_HO[1]=90;
	// for (int i = 0; i < 2; i++)
	// {
	// 	leg_HO.data.push_back(l_HO[i]);
	// }
	// pub_lHO.publish(leg_HO);
	// printf("leg_HO publish\n");
	// ros::spinOnce();
	// r.sleep();
	// leg_HO.data.clear();

	//while(d>600)
	while(ros::ok())
	{

		ros::spinOnce();
		r.sleep();
		mid_x2=0;mid_y2=0;

		//streamDepth.readFrame( &frameDepth );
		//SD.Depth_Arr();
		//SD.depth_find(21,&mid_x2,&mid_y2,&mid_x,&mid_y,&i,&j,&x,&sw);

		//Judge stairs distance
		//d21=(abs((depthMD(mid_x2-100,mid_y2+25)+depthMD(mid_x2-100,mid_y2+30)+depthMD(mid_x2-100,mid_y2+20))/3-(depthMD(mid_x2+100,mid_y2+25)+depthMD(mid_x2+100,mid_y2+30)+depthMD(mid_x2+100,mid_y2+20))/3));
       //fprintf(M2dldr,"%d\n",d21);

		//d21=(abs((SD.depthMD(mid_x-100,mid_y+25)+SD.depthMD(mid_x-100,mid_y+30)+SD.depthMD(mid_x-100,mid_y+20))/3-(SD.depthMD(mid_x+100,mid_y+25)+SD.depthMD(mid_x+100,mid_y+30)+SD.depthMD(mid_x+100,mid_y+20))/3));
        //Dec demo

		printf("d21=%d\n",d21);

		if(abs(d21)<=err)
		{
			printf("counter21 = %f\n",counter21);
			usleep(10000);

			if(counter21<=stop_align_dis) //5F:830, 4F:680
			{
				printf("distance OK\n");
				key1=3;//distance OK
			}

			if(key1!=1 && counter21>stop_align_dis)	//5F:820, 4F:670 ; too far ,TR fwd org=1100  depthMD(mid_x2,mid_y2)>900
			{
				key1=1;
				printf("Too Far\n");
				printf("stop_align_dis=%f\n",stop_align_dis);
		//bodyact("stop");

				r_motion=0;
				robot_motion.data=r_motion;
				pub_rm.publish(robot_motion);
				//*** pub_rm for simulating version
				twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
				twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
				pub_vel.publish(twist);
				//*** pub_rm for simulating version
				printf("robot_motion publish\n");
				ros::spinOnce();

				usleep(5000);

				vel_L=350;
				vel_R=350;
		//bodyJOGvel(vel_L,vel_R);//400

				r_speed[0]=vel_L;
				r_speed[1]=vel_R;
				for (int i = 0; i < 2; i++)
				{
					robot_speed.data.push_back(r_speed[i]);
				}
				pub_rs.publish(robot_speed);

				printf("robot_speed publish\n");

				usleep(5000);
		//bodyact("fwd");

				r_motion=1;
				robot_motion.data=r_motion;
				pub_rm.publish(robot_motion);
				//*** pub_rm for simulating version
				twist.linear.x = 0.28; twist.linear.y = 0; twist.linear.z = 0;
				twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
				pub_vel.publish(twist);
				//*** pub_rm for simulating version
				printf("robot_motion publish\n");
				ros::spinOnce();

				usleep(50000);
			}
			else if(key1!=2 && counter21<close_dis) //5F:780, 4F:590 ; too close,TR back //900 depthMD(mid_x2,mid_y2)<800
			{
				key1=2;
				printf("Too Close\n");
				printf("close_dis=%f\n",close_dis);
		//bodyact("stop");

				r_motion=0;
				robot_motion.data=r_motion;
				pub_rm.publish(robot_motion);
				//*** pub_rm for simulating version
				twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
				twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
				pub_vel.publish(twist);
				//*** pub_rm for simulating version
				printf("robot_motion publish\n");
				ros::spinOnce();

				usleep(5000);

				vel_L=350;
				vel_R=350;
		//bodyJOGvel(vel_L,vel_R);//300

				r_speed[0]=vel_L;
				r_speed[1]=vel_R;
				for (int i = 0; i < 2; i++)
				{
					robot_speed.data.push_back(r_speed[i]);
				}
				pub_rs.publish(robot_speed);

				printf("robot_speed publish\n");

		//bodyact("back");

				r_motion=2;
				robot_motion.data=r_motion;
				pub_rm.publish(robot_motion);
				//*** pub_rm for simulating version
				twist.linear.x = -0.2; twist.linear.y = 0; twist.linear.z = 0;
				twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
				pub_vel.publish(twist);
				//*** pub_rm for simulating version
				printf("robot_motion publish\n");
				ros::spinOnce();

				usleep(50000);
			}
			else if(key1==3)
			{
				printf("d21:%d , Mode2 Alignment OK\n",d21);

				r_motion=0;
				robot_motion.data=r_motion;
				pub_rm.publish(robot_motion);
				//*** pub_rm for simulating version
				twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
				twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
				pub_vel.publish(twist);
				//*** pub_rm for simulating version
				printf("robot_motion publish\n");
				ros::spinOnce();

				usleep(5000);

				if(sum3!=0) //never happen
				{
					printf("Calculate the angle\n");
					z3=float(sum3/j3);
					//xx3=((((21.2/119)-(21.2/123))/(105.5-101.1))*((depthMD(mid_x2,mid_y2)/10)-101.1)+(21.2/123));
					x3=abs(mid_y-mid_y2)*xx3;

					//angg=34;
					//angg=(57.2958*atan(x3,z3)*6);  //org -> ang=57.2958*atan(x3,z3);
					//ang=(float)(57.2958*atan(x3,z3)*6);
					printf("ang=%f \n",angg);


                    /*---Decide to know too left or right---*/
                    t21=abs(i-mid_x2);
                    td21=t21*xx3;
                    printf(" Left side stairs(td21)= %f cm\n",td21);
                    printf(" Left side stairs(mid_x2)= %d cm\n",mid_x2);


                    if(mid_x2<=300) //330
                    {
                        t21=abs(i-mid_x2);
                        td21=t21*xx3;
                        printf(" Left side stairs(td21)= %f cm\n",td21);
                        return 1;//mode_21 failed because of the left side stairs.
                    }
					//                    else if(mid_x2>=331)
					//                    {
					//                        t21=abs(mid_x2-x);
					//                        td21=t21*xx3;
					//                        printf("right side stairs(td21)= %f cm\n",td21);
					//                        return 2;//mode_21 failed because of the right side stairs.
					//                    }

                    /*---Decide to End mode21---*/
					if(td21>50 || angg>45 || angg>20) //lsd (left stairs distance)
					{
						return 0; //mode_21 succeed.
					}
					else return 1;
				}//if(sum3!=0) end
			} //Key3 end
		}//if(d21<=20) end
		//else if(key!=1 && d21>=20 && depthMD(mid_x2-100,mid_y2+20)>depthMD(mid_x2+100,mid_y2+20) && depthMD(mid_x2-100,mid_y2+25)>depthMD(mid_x2+100,mid_y2+25))
		else if( d21<=-err )
		{
			printf("d21:%d , Mode2 Need Left\n",d21);

			//bodyact("stop");
			r_motion=0;
			robot_motion.data=r_motion;
			pub_rm.publish(robot_motion);
			//*** pub_rm for simulating version
			twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
			twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
			pub_vel.publish(twist);
			//*** pub_rm for simulating version
			printf("robot_motion publish\n");
			ros::spinOnce();

			usleep(5000);

			vel_L=300;
        	vel_R=300;
			//bodyJOGvel(vel_L,vel_R);

			r_speed[0]=vel_L;
			r_speed[1]=vel_R;
			for (int i = 0; i < 2; i++)
			{
				robot_speed.data.push_back(r_speed[i]);
			}
			pub_rs.publish(robot_speed);
			printf("robot_speed publish\n");

			//bodyact("left");

			r_motion=3;
			robot_motion.data=r_motion;
			pub_rm.publish(robot_motion);
			//*** pub_rm for simulating version
			twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
			twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 2*PI/144;
			pub_vel.publish(twist);
			//*** pub_rm for simulating version
			printf("robot_motion publish\n");
			ros::spinOnce();

			usleep(300000);


			r_motion=0;
			robot_motion.data=r_motion;
			pub_rm.publish(robot_motion);
			//*** pub_rm for simulating version
			twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
			twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
			pub_vel.publish(twist);
			//*** pub_rm for simulating version
			printf("robot_motion publish\n");
			ros::spinOnce();

			usleep(5000);
			key1=0;
		}
		//else if(key!=2 && d21>=20&&depthMD(mid_x2-100,mid_y2+20)<depthMD(mid_x2+100,mid_y2+20) && depthMD(mid_x2-100,mid_y2+25)<depthMD(mid_x2+100,mid_y2+25))
		else if( d21>=err /*&& SD.depthMD(mid_x-100,mid_y+20)>SD.depthMD(mid_x+100,mid_y+20) && SD.depthMD(mid_x-100,mid_y+25)>SD.depthMD(mid_x+100,mid_y+25)*/)
		//Dec demo
		{
			printf("d21:%d , Mode2 Need Right\n",d21);

	//bodyact("stop");

			r_motion=0;
			robot_motion.data=r_motion;
			pub_rm.publish(robot_motion);
			//*** pub_rm for simulating version
			twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
			twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
			pub_vel.publish(twist);
			//*** pub_rm for simulating version
			printf("robot_motion publish\n");
			ros::spinOnce();

			usleep(5000);

			vel_L=300;
			vel_R=300;
	//bodyJOGvel(vel_L,vel_R);
			r_speed[0]=vel_L;
			r_speed[1]=vel_R;
			for (int i = 0; i < 2; i++)
			{
				robot_speed.data.push_back(r_speed[i]);
			}
			pub_rs.publish(robot_speed);

			printf("robot_speed publish\n");
	//bodyact("right");

			r_motion=4;
			robot_motion.data=r_motion;
			pub_rm.publish(robot_motion);
			//*** pub_rm for simulating version
			twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
			twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = -2*PI/144;
			pub_vel.publish(twist);
			//*** pub_rm for simulating version
			printf("robot_motion publish\n");
			ros::spinOnce();

			usleep(300000);//200000


			r_motion=0;
			robot_motion.data=r_motion;
			pub_rm.publish(robot_motion);
			//*** pub_rm for simulating version
			twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
			twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
			pub_vel.publish(twist);
			//*** pub_rm for simulating version
			printf("robot_motion publish\n");
			ros::spinOnce();
			usleep(5000);
			key1=0;
		}



	if(counter21<stop_align_dis )
		break;

	ros::spinOnce();
	r.sleep();

	}//while(d>600) end
	// int cnt_dis=0;
	while(ros::ok())
	{
		ros::Rate r(1);
		UpStairMode::robotspeed(350,350);
		if( counter21>far_dis )//80
		{
			UpStairMode::robotmotion("Forward");
			printf("cnt21:%f > far_dis:%f\n",counter21, far_dis);
			std::cout<<"Too far\n";
			r.sleep();

		}
		else if(counter21<close_dis)//70
		{
			UpStairMode::robotmotion("Backward");
			printf("cnt21:%f < close_dis:%f\n",counter21, close_dis);
			std::cout<<"Too close\n";
			r.sleep();
		}
		else
		{
			UpStairMode::robotmotion("Stop");
			printf("cnt21:%f is fine\n",counter21);
			break;
			//cnt_dis++;
			r.sleep();
		}
		if (counter21>stop_align_dis+10)
		{
			UpStairMode::robotmotion("Stop");
			break;
			//cnt_dis++;
			r.sleep();
		}

		// if(cnt_dis>=3)
		// {
		// 	break;
		// }
	}
	UpStairMode::robotmotion("Stop");
	std::cout<<"The end of Mode 21\n";
				/*---Time end---*/
			//    time_t t2 = time(NULL);
			//    fprintf(M2XD,"Mode2 Time = %d\n",t2-t1);

				/*---Data Record - Close---*/
			//    fclose(M2XD);
			//    fclose(M2PL);
			//    fclose(M2VL);
			//    fclose(M2VR);
			//    fclose(M2dldr);

}//mode21 end


void UpStairMode::mode_3()
{
	/*---Time---*/
    //time_t t1 = time(NULL);

	/*---Data Record - open---*/
    //FILE *M3Time=fopen("/home/ubuntu/CodeBlocks/TrackedRobot_Control/Recore Data/M3_time.txt","w");


	std_msgs::Int32MultiArray robot_speed,leg_speed,robot_MA,leg_MA,robot_HO,robot_VA,robot_MR,leg_HO,leg_VA;

	std_msgs::Int32 robot_motion,leg_motion;

	dynamixel_workbench_msgs::JointCommand joint_command;
	std_msgs::Float64 workbenchV;
	std_msgs::Float64 frontflipperang;

	robot_speed.data.clear();
	leg_speed.data.clear();
	robot_MA.data.clear();
	leg_MA.data.clear();
	robot_HO.data.clear();
	leg_HO.data.clear();
	robot_VA.data.clear();
	robot_MR.data.clear();

	ros::Rate r(1);

	ros::spinOnce();
	r.sleep();

	//bodyact("stop");
	r_motion=0;//robot_motion
	robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	//*** pub_rm for simulating version
	twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
	twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
	pub_vel.publish(twist);
	//*** pub_rm for simulating version
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();
	usleep(50000);

	//XtionMotor(740-angg,0); //375 5F 640

	bottomm=bottom;
	topp=top-15;
	joint_command.request.unit = "rad";
	joint_command.request.id = 1;
	joint_command.request.goal_position = bottomm*PI/180;
	joint_command_client.call(joint_command);
	joint_command.request.unit = "rad";
	joint_command.request.id = 2;
	joint_command.request.goal_position = topp*PI/180;
	joint_command_client.call(joint_command);
	workbenchV.data = (float)(15*PI/180);
	pub_workbenchV.publish(workbenchV);

	printf("ZED turn down\n");
	ROS_INFO("bottom:%f,top:%f", bottomm,topp);
	// topp=topp+30;
	// head_bottom.data=bottomm;
	// head_top.data=topp;
	// pub_hb.publish(head_bottom);
	// pub_ht.publish(head_top);
	// ROS_INFO("bottom:%f,top:%f", head_bottom.data,head_top.data);
	// ros::spinOnce();
	// r.sleep();
	usleep(30000);

	printf("Mode 3 , Up Stairs Prepare\n");

	if(angg<45 && angg>20)
	{
		/*---Speed set---*/
		//bodyJOGvel(450,450);//350,350
		//legMAvel(200,200);
		//r_speed[0]=450;
		//r_speed[1]=450;
		r_speed[0]=700;
		r_speed[1]=700;
		l_speed[0]=1000;
		l_speed[1]=1000;
		for (int i = 0; i < 2; i++)
		{
			robot_speed.data.push_back(r_speed[i]);
			leg_speed.data.push_back(l_speed[i]);
		}
		pub_rs.publish(robot_speed);
		pub_ls.publish(leg_speed);
		printf("speed publish\n");
		ros::spinOnce();
		r.sleep();
		robot_speed.data.clear();
		leg_speed.data.clear();
		usleep(30000);
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
		/*---01---*/
		//legma((85-angg)*300,(85-angg)*300); //T0, T3 move down 90,90

		l_MA[0]=(90-angg)*4550;
		l_MA[1]=(90-angg)*4550;
		for (int i = 0; i < 2; i++)
		{
			leg_MA.data.push_back(l_MA[i]);
		}
		pub_lMA.publish(leg_MA);
		printf("leg_MA publish\n");
		ros::spinOnce();
		r.sleep();
		leg_MA.data.clear();
		printf("************************************sleep 10s****************************\n");
		//sleep(3); //6.5 sec
		usleep(10000000);
		/*---02 Time: 7sec---*/
		//bodyact("fwd");
		r_motion=1;//robot_motion
		robot_motion.data=r_motion;
		pub_rm.publish(robot_motion);
		//*** pub_rm for simulating version
		twist.linear.x = 0.56; twist.linear.y = 0; twist.linear.z = 0;
		twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
		pub_vel.publish(twist);
		//*** pub_rm for simulating version
		printf("robot_motion publish\n");
		ros::spinOnce();
		r.sleep();
		usleep(1000000);
		printf("********************* go 1s *****************\n");

		//legma((85-angg)*300,90*300); //T3 move down first, T0 not move 90,90
		l_MA[0]=80*4550;
		l_MA[1]=80*4550;
		for (int i = 0; i < 2; i++)
		{
			leg_MA.data.push_back(l_MA[i]);
			printf("MA:%d\n",l_MA[i]);
		}
		pub_lMA.publish(leg_MA);
		ros::spinOnce();
		printf("leg_MA publish\n");
		r.sleep();
		leg_MA.data.clear();
		// usleep(6800000);
		//usleep(7800000);
		printf("************************************sleep 4s****************************\n");
		usleep(4000000);
		/*---03---*/
		//legma(85*300,85*300); //T0 move down, T3 not move 98,90
		l_MA[0]=90*4550;
		l_MA[1]=90*4550;
		for (int i = 0; i < 2; i++)
		{
			leg_MA.data.push_back(l_MA[i]);
		}
		pub_lMA.publish(leg_MA);
		printf("leg_MA publish\n");
		ros::spinOnce();
		r.sleep();
		leg_MA.data.clear();
		printf("************************************sleep 2.5s****************************\n");
		usleep(2500000);
		//bodyact("stop");
		r_motion=0;//robot_motion
		robot_motion.data=r_motion;
		pub_rm.publish(robot_motion);
		//*** pub_rm for simulating version
		twist.linear.x = 0.1; twist.linear.y = 0; twist.linear.z = 0;
		twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
		pub_vel.publish(twist);
		//*** pub_rm for simulating version
		printf("robot_motion publish\n");
		ros::spinOnce();
		r.sleep();
		usleep(50000);

		//*out_mode=4;
		printf("mode3 End\n");
	}
	else
	{
		printf("Mode3 Stair Angle Greater or less than Limit\n");
		//mode_21();
		//d=800;
	}



}//mode3 end

void UpStairMode::mode_4()	//---Climbing mode
{
	std_msgs::Int32MultiArray robot_speed,leg_speed,robot_MA,leg_MA,robot_HO,robot_VA,robot_MR,leg_HO,leg_VA;

	std_msgs::Int32 robot_motion,leg_motion;

	dynamixel_workbench_msgs::JointCommand joint_command;
	std_msgs::Float64 workbenchV;
	std_msgs::Float64 frontflipperang;

	robot_speed.data.clear();
	leg_speed.data.clear();
	robot_MA.data.clear();
	leg_MA.data.clear();
	leg_VA.data.clear();
	robot_HO.data.clear();
	robot_VA.data.clear();
	robot_MR.data.clear();
	leg_HO.data.clear();

	ros::Rate r(2);

	//ros::spinOnce();
	//r.sleep();

	int cont4=0;
	int key=0;
	int vel_L=0,vel_R=0;



   printf("Mode 4 , Climbing Mode\n");


	//legMAvel(1,1);
	//legJOGvel(1,1);
	//legva(50,50);
		l_VA[0]=200;
		l_VA[1]=200;
		l_speed[0]=1;
		l_speed[1]=1;
		for (int i = 0; i < 2; i++)
		{
			leg_VA.data.push_back(l_VA[i]);
			leg_speed.data.push_back(l_speed[i]);
		}
		pub_lVA.publish(leg_VA);
		pub_ls.publish(leg_speed);
		printf("speed publish\n");
		ros::spinOnce();
		r.sleep();
		leg_speed.data.clear();
		leg_VA.data.clear();


	//bodyact("fwd");
	r_motion=0;//robot_motion
	robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	//*** pub_rm for simulating version
	twist.linear.x = 0.1; twist.linear.y = 0; twist.linear.z = 0;
	twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
	pub_vel.publish(twist);
	//*** pub_rm for simulating version
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();
   	usleep(50000);

	while(ros::ok())
	{
		bottomm=bottom;
		topp=top-40;
		joint_command.request.unit = "rad";
		joint_command.request.id = 1;
		joint_command.request.goal_position = bottomm*PI/180;
		joint_command_client.call(joint_command);
		joint_command.request.unit = "rad";
		joint_command.request.id = 2;
		joint_command.request.goal_position = topp*PI/180;
		joint_command_client.call(joint_command);
		workbenchV.data = (float)(40*PI/180);
		pub_workbenchV.publish(workbenchV);

		printf("ZED turn down\n");
		ROS_INFO("bottom:%f,top:%f,workbenchV:%f", bottomm,topp, workbenchV.data);
		printf("mid:(%d,%d) , d4:%d , cnt21:%f",mid_x,mid_y,d4,counter21);		
		if(mid_x!=0 && mid_y!=0 && counter21<1500)
		{
			if(d4<=56 && d4>=-56) //40
			{
				vel_L=600;//1000
				vel_R=600;//1000
			//bodyJOGvel(vel_L,vel_R);
				r_speed[0]=vel_L;
				r_speed[1]=vel_R;
				for (int i = 0; i < 2; i++)
				{
					robot_speed.data.push_back(r_speed[i]);
				}
				pub_rs.publish(robot_speed);
				printf("speed publish\n");
				ros::spinOnce();
				r.sleep();
				robot_speed.data.clear();
				usleep(10000);
				//bodyact("fwd");
				r_motion=1;//robot_motion
				robot_motion.data=r_motion;
				pub_rm.publish(robot_motion);
				//*** pub_rm for simulating version
				twist.linear.x = 0.48; twist.linear.y = 0; twist.linear.z = 0;
				twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
				pub_vel.publish(twist);
				//*** pub_rm for simulating version
				printf("robot_motion publish\n");
				ros::spinOnce();
				r.sleep();
				usleep(10000);
			}
			else if(d4<-56)
			{
				printf("need turn left\n");
				vel_L=550;//880
            	vel_R=600;//1000
				//bodyJOGvel(vel_L,vel_R);
				r_speed[0]=vel_L;
				r_speed[1]=vel_R;
				for (int i = 0; i < 2; i++)
				{
					robot_speed.data.push_back(r_speed[i]);
				}
				pub_rs.publish(robot_speed);
				printf("speed publish\n");
				ros::spinOnce();
				r.sleep();
				robot_speed.data.clear();
				usleep(10000);
				//bodyact("fwd");
				r_motion=1;//robot_motion
				robot_motion.data=r_motion;
				pub_rm.publish(robot_motion);
				//*** pub_rm for simulating version
				twist.linear.x = 0.48; twist.linear.y = 0; twist.linear.z = 0;
				twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = PI/144*2;
				pub_vel.publish(twist);
				//*** pub_rm for simulating version
				printf("robot_motion publish\n");
				ros::spinOnce();
				r.sleep();
				usleep(10000);


			}
			else if(d4>56)
			{
				printf("need turn right\n");
				vel_L=600;//1000
		        vel_R=550;//880
				//bodyJOGvel(vel_L,vel_R);
				r_speed[0]=vel_L;
				r_speed[1]=vel_R;
				for (int i = 0; i < 2; i++)
				{
					robot_speed.data.push_back(r_speed[i]);
				}
				pub_rs.publish(robot_speed);
				printf("speed publish\n");
				ros::spinOnce();
				r.sleep();
				robot_speed.data.clear();
				usleep(10000);
				//bodyact("fwd");
				r_motion=1;//robot_motion
				robot_motion.data=r_motion;
				pub_rm.publish(robot_motion);
				//*** pub_rm for simulating version
				twist.linear.x = 0.48; twist.linear.y = 0; twist.linear.z = 0;
				twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = -PI/144*2;
				pub_vel.publish(twist);
				//*** pub_rm for simulating version
				printf("robot_motion publish\n");
				ros::spinOnce();
				r.sleep();
				usleep(10000);//0.2


			}

		}
		else
		{
			printf("---Ready go to the top!---\n");
			key=0;

			//bodyact("stop");
			r_motion=0;//robot_motion
			robot_motion.data=r_motion;
			pub_rm.publish(robot_motion);
			//*** pub_rm for simulating version
			twist.linear.x = 0.1; twist.linear.y = 0; twist.linear.z = 0;
			twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
			pub_vel.publish(twist);
			//*** pub_rm for simulating version
			printf("robot_motion publish\n");
			ros::spinOnce();
			r.sleep();

			usleep(50000);
			vel_L=600;
			vel_R=600;
			//bodyJOGvel(vel_L,vel_R);
			r_speed[0]=vel_L;
			r_speed[1]=vel_R;
			for (int i = 0; i < 2; i++)
			{
				robot_speed.data.push_back(r_speed[i]);
			}
			pub_rs.publish(robot_speed);
			printf("speed publish\n");
			ros::spinOnce();
			r.sleep();
			robot_speed.data.clear();

			//bodyact("fwd");
			r_motion=0;//robot_motion
			robot_motion.data=r_motion;
			pub_rm.publish(robot_motion);
			//*** pub_rm for simulating version
			twist.linear.x = 0.1; twist.linear.y = 0; twist.linear.z = 0;
			twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
			pub_vel.publish(twist);
			//*** pub_rm for simulating version
			printf("robot_motion publish\n");
			ros::spinOnce();
			r.sleep();

         	usleep(50000);//0.05
			cont4++;
			if(cont4>=2)break;
		}


	}//while(cont4<3) end

	printf("---Near to TOP---\n\n\n");
	/*---ALL STOP---*/

		//bodyact("stop");
		//legact("stop","stop");
	r_motion=0;//robot_motion
	robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	//*** pub_rm for simulating version
	twist.linear.x = 0.1; twist.linear.y = 0; twist.linear.z = 0;
	twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
	pub_vel.publish(twist);
	//*** pub_rm for simulating version
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();


	usleep(10000);

	//legva(600,600);
	l_VA[0]=600;
	l_VA[1]=600;
	for (int i = 0; i < 2; i++)
	{
		leg_VA.data.push_back(l_VA[i]);
	}
	pub_lVA.publish(leg_VA);
	printf("speed publish\n");
	ros::spinOnce();
	r.sleep();
	leg_VA.data.clear();

	usleep(10000);

	//bodyJOGvel_select(1,700);
	//bodyJOGvel_select(2,700);
	//usleep(10000);


	//bodyJOGvel(450,450); //400,400
	r_speed[0]=600;
	r_speed[1]=600;
	for (int i = 0; i < 2; i++)
	{
		robot_speed.data.push_back(r_speed[i]);
	}
	pub_rs.publish(robot_speed);
	printf("speed publish\n");
	ros::spinOnce();
	r.sleep();
	robot_speed.data.clear();
	usleep(10000);
	//bodyact("fwd");
	r_motion=1;//robot_motion
	robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	//*** pub_rm for simulating version
	twist.linear.x = 0.48; twist.linear.y = 0; twist.linear.z = 0;
	twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
	pub_vel.publish(twist);
	//*** pub_rm for simulating version
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();

	usleep(3700000); //org:6500000 ,5700000

	printf("---TOP Lay Down Front Arm---\n\n\n");

	/*---02.Stop and set speed---*/

	//bodyact("stop");
	r_motion=0;//robot_motion
	robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	//*** pub_rm for simulating version
	twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
	twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
	pub_vel.publish(twist);
	//*** pub_rm for simulating version
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();

	usleep(50000);//0.5
	//bodyJOGvel(400,400);
	//legMAvel(200,200);
	r_speed[0]=600;
	r_speed[1]=600;
	l_speed[0]=1000;
	l_speed[1]=1000;
	for (int i = 0; i < 2; i++)
	{
		robot_speed.data.push_back(r_speed[i]);
		leg_speed.data.push_back(l_speed[i]);
	}
	pub_rs.publish(robot_speed);
	pub_ls.publish(leg_speed);
	printf("speed publish\n");
	ros::spinOnce();
	r.sleep();
	leg_speed.data.clear();
	robot_speed.data.clear();

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
	workbenchV.data = (float)(0*PI/180);
	pub_workbenchV.publish(workbenchV);

	printf("ZED init\n");
	ROS_INFO("bottom:%f,top:%f", bottomm,topp);


	// head_bottom.data=bottom;
	// head_top.data=top;
	// pub_hb.publish(head_bottom);
	// pub_ht.publish(head_top);
	// ROS_INFO("bottom:%f,top:%f", head_bottom.data,head_top.data);
	// ros::spinOnce();
	// r.sleep();
	usleep(500000);
   

	/*---03. Time : 7sec---*/

	//legma(95*300,135*300);
	l_MA[0]=135*4550;
	l_MA[1]=115*4550;
	for (int i = 0; i < 2; i++)
	{
		leg_MA.data.push_back(l_MA[i]);
		printf("MA:%d\n",l_MA[i]);
	}
	pub_lMA.publish(leg_MA);
	printf("leg_MA publish\n");
	ros::spinOnce();
	r.sleep();
	leg_MA.data.clear();


	r_motion=1;//robot_motion
	robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	//*** pub_rm for simulating version
	twist.linear.x = 0.48; twist.linear.y = 0; twist.linear.z = 0;
	twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
	pub_vel.publish(twist);
	//*** pub_rm for simulating version
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();
    printf("**********************front: %d back %d, go & sleep 3.5s********************\n", l_MA[0], l_MA[1]);
	usleep(3500000);

	l_MA[0]=60*4550;
	l_MA[1]=55*4550;


	for (int i = 0; i < 2; i++)
	{
		leg_MA.data.push_back(l_MA[i]);
		printf("MA:%d\n",l_MA[i]);
	}
	pub_lMA.publish(leg_MA);
	printf("leg_MA publish\n");	
    printf("**********************front: %d back %d********************\n", l_MA[0], l_MA[1]);
    ros::spinOnce();
	leg_MA.data.clear();
	UpStairMode::motor_switch(1,0);
	usleep(500000);
	UpStairMode::motor_switch(4,0);
	usleep(500000);
	UpStairMode::motor_switch(1,1);
	usleep(500000);
	UpStairMode::motor_switch(4,1);
	usleep(500000);

	//bodyact("fwd");
	// usleep(4000000);
	// UpStairMode::legspeed(600,600);
	// UpStairMode::legmotion("Front_Down");
	// UpStairMode::legmotion("Back_Down");
	// UpStairMode::leghome(0,0);
	// usleep(1000000);//sleep to wait the leg touch ground
	// UpStairMode::leghome(0,0);
	usleep(500000);

	/*---04---*/


	//legma(0,0);
	// l_MA[0]=0;
	// l_MA[1]=0;
	// for (int i = 0; i < 2; i++)
	// {
	// 	leg_MA.data.push_back(l_MA[i]);
	// 	printf("MA:%d\n",l_MA[i]);
	// }
	// pub_lMA.publish(leg_MA);
	// printf("leg_MA publish\n");
	// ros::spinOnce();
	// r.sleep();
	// leg_MA.data.clear();
	usleep(3500000);

	//bodyact("stop");
	r_motion=0;//robot_motion
	robot_motion.data=r_motion;
	pub_rm.publish(robot_motion);
	//*** pub_rm for simulating version
	twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
	twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
	pub_vel.publish(twist);
	//*** pub_rm for simulating version
	printf("robot_motion publish\n");
	ros::spinOnce();
	r.sleep();

	usleep(50000);

	//legMAvel(500,500);
	l_speed[0]=1000;
	l_speed[1]=1000;
	for (int i = 0; i < 2; i++)
	{
		leg_speed.data.push_back(l_speed[i]);
	}
	pub_ls.publish(leg_speed);
	printf("leg_speed publish\n");
	ros::spinOnce();
	r.sleep();
	leg_speed.data.clear();
	//legma(0,0);
	UpStairMode::legMA(-26000,-27000);//leg up
    printf("******************leg recover****************\n");
	usleep(2000000);

	UpStairMode::leghome(0,0);
	usleep(1000000);//sleep to wait the leg touch ground
	printf("---Already On the TOP---\n");
	printf("Mode4 End \n\n\n");

}	//mode4 end


void UpStairMode::legMA(int front,int back)
{
	std_msgs::Int32MultiArray leg_MA;

	l_MA[0]=back;
	l_MA[1]=front;
	for (int i = 0; i < 2; i++)
	{
		leg_MA.data.push_back(l_MA[i]);
	}
	pub_lMA.publish(leg_MA);
	printf("leg_MA publish\n");

	ros::spinOnce();
}
void UpStairMode::robotMA(int left,int right)
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
void UpStairMode::robotspeed(int left,int right)
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
void UpStairMode::legspeed(int front,int back)
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

void UpStairMode::robotmotion(std::string rm)
{
	std_msgs::Int32 robot_motion;


	if  (rm =="Stop"){
		robot_motion.data=0;
		//*** pub_rm for simulating version
		twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
		twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
		pub_vel.publish(twist);
		//*** pub_rm for simulating version
	}
	else if(rm =="Forward"){
		robot_motion.data=1;
		//*** pub_rm for simulating version
		twist.linear.x = 0.28; twist.linear.y = 0; twist.linear.z = 0;
		twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
		pub_vel.publish(twist);
		//*** pub_rm for simulating version
	}
	else if(rm =="Backward"){
		robot_motion.data=2;
		//*** pub_rm for simulating version
		twist.linear.x = -0.28; twist.linear.y = 0; twist.linear.z = 0;
		twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0;
		pub_vel.publish(twist);
		//*** pub_rm for simulating version
	}
	else if(rm =="Left"){
		robot_motion.data=3;
		//*** pub_rm for simulating version
		twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
		twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = PI/144;
		pub_vel.publish(twist);
		//*** pub_rm for simulating version
	}
	else if(rm =="Right"){
		robot_motion.data=4;
		//*** pub_rm for simulating version
		twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0;
		twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = -PI/144;
		pub_vel.publish(twist);
		//*** pub_rm for simulating version
	}
	else
		std::cout<<"error rm input!!\n";

	pub_rm.publish(robot_motion);
	//printf("robot_motion publish\n");

	ros::spinOnce();
}
void UpStairMode::legmotion(std::string lm)
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
void UpStairMode::leghome(int front,int back)
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
void UpStairMode::motor_switch(int number,int serve_status)
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
