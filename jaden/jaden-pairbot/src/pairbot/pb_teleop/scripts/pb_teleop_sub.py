#!/usr/bin/env python

from __future__ import print_function

#import roslib; roslib.load_manifest('pb_teleop')
import rospy
import threading
import time
import numpy as np

from geometry_msgs.msg import Twist
from std_msgs.msg import Float64
from std_msgs.msg import String
from std_msgs.msg import Int32MultiArray

import sys, select, termios, tty, math
global sub_once, leg_HO
leg_HO = np.array([0, 0])
msg = """
Reading from the keyboard and Publishing to Twist!
---------------------------
Moving around (Base Wheel):
        w     
   a    s    d
        x

---------------------------
Front Flipper:
(wheel)
   u    i    o
(up/down) 
7 (+degree)
y (-degree)
---------------------------
Rear Flipper:
(wheel)
   j    k    l
(up/down)
h (+degree)
n (-degree)

anything else : stop
CTRL-C to quit
"""

STOP = -9999

baseMove = {
    #'key':(x, y, z, angular)
    'w':(1,0,0,0),
    'a':(0,0,0,1),
    's':(STOP,0,0,0),
    'x':(-1,0,0,0),
    'd':(0,0,0,-1)
}

frontFlipper = {
    #'key':(speed, degree)
    'u':(1,30),  # math.pi/180*30
    'i':(1,0),
    'o':(1,-30), # math.pi/180*30
    '7':(0,1),
    'y':(0,-1)
}

rearFlipper = {
    #'key':(speed, degree)
    'j':(1,30), # math.pi/180*30
    'k':(1,0),
    'l':(1,-30),# math.pi/180*30
    'h':(0,1),
    'n':(0,-1)
}


def rMA_callback(data):
    global pub_vel,pub_front_flipper,pub_rear_flipper
    global x,y,z,th,speed_step,angle_step,front_speed,front_degree,rear_speed,rear_degree
    global sub_once
    robot_MA = data.data
    print("getin keycb, kmsg="+str(robot_MA))
    # if (cur_time - timestamp)<0.1:
    #     print("Click keystroke too fast! Ignore input temporarily....")
    #     continue
    # timestamp = cur_time
    endWhile = False
    while endWhile == False:
        if keymsg in baseMove.keys(): #'keymsg':(x, y, z, angular)
            if baseMove[keymsg][0] == STOP:
                print('Taking the break...')
                r = rospy.Rate(10)
                for i in range(REST_STEPS,-1,-1):
                    twist = Twist()
                    twist.linear.x = x * float(i)/REST_STEPS
                    twist.linear.y = 0
                    twist.linear.z = 0
                    twist.angular.x = 0
                    twist.angular.y = 0
                    twist.angular.z = th * float(i) /REST_STEPS
                    pub_vel.publish(twist)
                    r.sleep()
                x = 0
                th = 0
                print('done')
            else:
                x += speed_step*baseMove[keymsg][0]
                th += angle_step*baseMove[keymsg][3]
                twist = Twist()
                twist.linear.x = x
                twist.linear.y = 0
                twist.linear.z = 0
                twist.angular.x = 0
                twist.angular.y = 0
                twist.angular.z = th
                pub_vel.publish(twist)
        elif keymsg in frontFlipper.keys(): #'keymsg':(target/increase, steps)
            if frontFlipper[keymsg][0] == 1: # target degree
                t_step = frontFlipper[keymsg][1]
                sub_once = rospy.Subscriber('/pairbot/joint_left_front_flipper_controller/command', Float64, cb_front_degree)
                print('after get='+str(front_degree))   
                cur_step = int(front_degree/angle_step)
                if t_step == cur_step:
                    break
                print('moving front flipper to {0}....'.format(180*t_step*angle_step/math.pi))
                r = rospy.Rate(20)
                direction = (t_step-cur_step)/abs(t_step-cur_step)
                while cur_step!=t_step:
                    cur_step+=direction
                    front_degree = angle_step*cur_step
                    if abs(front_degree)<0.0001:
                        front_degree = 0
                    pub_front_flipper.publish(front_degree)
                    r.sleep()
                print('done')
            else:
                front_degree += angle_step*frontFlipper[keymsg][1]
                if abs(front_degree)<0.0001:
                    front_degree = 0
                pub_front_flipper.publish(front_degree)
        elif keymsg in rearFlipper.keys(): #'keymsg':(speed, degree)
            if rearFlipper[keymsg][0] == 1: # target degree
                t_step = rearFlipper[keymsg][1]
                sub_once = rospy.Subscriber('/pairbot/joint_left_rear_flipper_controller/command', Float64, cb_rear_degree)
                print('after get='+str(rear_degree))  
                cur_step = int(rear_degree/angle_step)
                if t_step == cur_step:
                    break
                print('moving rear flipper to {0}....'.format(180*t_step*angle_step/math.pi))
                r = rospy.Rate(20)
                direction = (t_step-cur_step)/abs(t_step-cur_step)
                while cur_step!=t_step:
                    cur_step+=direction
                    rear_degree = angle_step*cur_step
                    if abs(rear_degree)<0.0001:
                        rear_degree = 0
                    pub_rear_flipper.publish(rear_degree)
                    r.sleep()
                print('done')
            else:
                rear_degree += angle_step*rearFlipper[keymsg][1]
                if abs(rear_degree)<0.0001:
                    rear_degree = 0
                pub_rear_flipper.publish(rear_degree)
        elif keymsg == 'q':
            print('Putting the robot into rest...')
            r = rospy.Rate(10)
            for i in range(REST_STEPS,-1,-1):
                twist = Twist()
                twist.linear.x = x * float(i)/REST_STEPS
                twist.linear.y = 0
                twist.linear.z = 0
                twist.angular.x = 0
                twist.angular.y = 0
                twist.angular.z = th * float(i) /REST_STEPS
                pub_vel.publish(twist)
                pub_front_flipper.publish(front_degree*float(i)/REST_STEPS)
                pub_rear_flipper.publish(rear_degree*float(i)/REST_STEPS)
                r.sleep()
            x = 0
            th = 0
            front_speed = 0
            front_degree = 0
            rear_speed = 0
            rear_degree = 0
            twist = Twist()
            twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0
            twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0
            pub_vel.publish(twist)
            pub_front_flipper.publish(front_degree)
            pub_rear_flipper.publish(rear_degree)
            print('done')
            
        endWhile = True
        print("current:\tx: %s\ttheta: %s\tfront_degree:%s\trear_degree:%s\t" \
            %(x, th, 180*front_degree/math.pi, 180*rear_degree/math.pi))


def lMA_callback(data):
    global pub_vel,pub_front_flipper,pub_rear_flipper
    global x,y,z,th,speed_step,angle_step,front_speed,front_degree,rear_speed,rear_degree,leg_HO
    global sub_once
    leg_MA = np.array([0, 0])
    leg_MA[0] = data.data[0]  #front
    leg_MA[1] = data.data[1]  #rear
    sub_once = rospy.Subscriber('/leg_HO', Int32MultiArray, cb_leg_HO)
    endWhile = False
    while endWhile == False:
        t_step = leg_HO[0]-leg_MA[0]/4550  #HOME
        sub_once = rospy.Subscriber('/pairbot/joint_left_front_flipper_controller/command', Float64, cb_front_degree)
        print('get lHO0='+str(leg_HO[0]))   
        cur_step = int(front_degree/angle_step)
        if t_step == cur_step:
            break
        print('moving front flipper to {0}....'.format(180*t_step*angle_step/math.pi))
        r = rospy.Rate(20)
        direction = (t_step-cur_step)/abs(t_step-cur_step)
        while cur_step!=t_step:
            cur_step+=direction
            front_degree = angle_step*cur_step
            if abs(front_degree)<0.0001:
                front_degree = 0
            pub_front_flipper.publish(front_degree)
            r.sleep()
        print('done')


        t_step = leg_HO[1]-leg_MA[1]/4550
        sub_once = rospy.Subscriber('/pairbot/joint_left_rear_flipper_controller/command', Float64, cb_rear_degree)
        print('get lHO1='+str(leg_HO[1]))  
        cur_step = int(rear_degree/angle_step)
        if t_step == cur_step:
            break
        print('moving rear flipper to {0}....'.format(180*t_step*angle_step/math.pi))
        r = rospy.Rate(20)
        direction = (t_step-cur_step)/abs(t_step-cur_step)
        while cur_step!=t_step:
            cur_step+=direction
            rear_degree = angle_step*cur_step
            if abs(rear_degree)<0.0001:
                rear_degree = 0
            pub_rear_flipper.publish(rear_degree)
            r.sleep()
        print('done')
            
        endWhile = True
        print("current:\tx: %s\ttheta: %s\tfront_degree:%s\trear_degree:%s\t" \
            %(x, th, 180*front_degree/math.pi, 180*rear_degree/math.pi))
    
def cb_front_degree(data):
    global front_degree
    front_degree = data.data
    #print("receiveee="+keymsg)
    sub_once.unregister()

def cb_rear_degree(data):
    global rear_degree
    rear_degree = data.data
    #print("receiveee="+keymsg)
    sub_once.unregister()

def cb_leg_HO(data):
    global leg_HO, front_degree, rear_degree
    leg_HO[0] = front_degree*180/math.pi + data.data[0]  #front
    leg_HO[1] = rear_degree*180/math.pi + data.data[1]  #rear
    sub_once.unregister()



REST_STEPS=20

def listener():
    rospy.init_node('listener', anonymous=True)
 
    global pub_vel,pub_front_flipper,pub_rear_flipper
    pub_vel = rospy.Publisher('/pairbot/cmd_vel', Twist, queue_size = 10)
    pub_front_flipper = rospy.Publisher('/pairbot/joint_left_front_flipper_controller/command', Float64, queue_size = 10)
    pub_rear_flipper = rospy.Publisher('/pairbot/joint_left_rear_flipper_controller/command', Float64, queue_size = 10)
    

    # base
    global x,y,z,th,speed_step,angle_step,front_speed,front_degree,rear_speed,rear_degree
    x = 0
    y = 0
    z = 0
    th = 0

    speed_step = 0.1
    angle_step = math.pi/180
    # front flipper
    front_speed = 0
    front_degree = 0
    # rear flipper
    rear_speed = 0
    rear_degree = 0

    print("Ready for subscribe teleop")
    # rospy.Subscriber("robot_MA", Int32MultiArray, rMA_callback)
    rospy.Subscriber("leg_MA", Int32MultiArray, lMA_callback)
    rospy.spin()

if __name__=="__main__":
    settings = termios.tcgetattr(sys.stdin)
    listener()
    
termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
