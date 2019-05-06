#!/usr/bin/env python

from __future__ import print_function

#import roslib; roslib.load_manifest('pb_teleop')
import rospy
import threading
import time
import numpy as np

from geometry_msgs.msg import Twist
from std_msgs.msg import Int32
from std_msgs.msg import Float64
from std_msgs.msg import String
from std_msgs.msg import Int32MultiArray

import sys, select, termios, tty, math
global sub_once, leg_HO, r_speed
leg_HO = np.array([0, 0])
r_speed = np.array([0.0, 0.0])
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

def job_front_flipper_buffer(t_step, cur_step):
    global angle_step,front_degree,pub_front_flipper

    direction = (t_step-cur_step)/abs(t_step-cur_step)
    while cur_step!=t_step:
        cur_step+=direction
        front_degree = angle_step*cur_step
        if abs(front_degree)<0.0001:
            front_degree = 0
        pub_front_flipper.publish(front_degree)
        time.sleep(0.05)

def rM_callback(data):
    global r_speed, cur_left_speed, cur_right_speed,pub_left_vel,pub_right_vel
    REST_STEPS = 20
    MAX_SPEED = 3
    robot_motion = 0
    noAction = True
    robot_motion = data.data  #0:stop, 1:fw, 2:b, 3:l, 4:r
    if robot_motion == 0 and (cur_left_speed != 0 or cur_right_speed != 0):
        noAction = False
        print('Taking the break...')
        r = rospy.Rate(10)
        for i in range(REST_STEPS,-1,-1):
            left_vel = MAX_SPEED * (cur_left_speed*float(i)/REST_STEPS)
            right_vel = MAX_SPEED * (cur_left_speed*float(i)/REST_STEPS)
            pub_left_vel.publish(left_vel)
            pub_right_vel.publish(right_vel)
            r.sleep()
        cur_left_speed = 0
        cur_right_speed = 0
        print('done')

    elif robot_motion == 1:
        r = rospy.Rate(30)
        #print('cur='+str(cur_left_speed)+'  r_sp'+str(r_speed[0]))  #-------
        if cur_left_speed != r_speed[0] or cur_right_speed != r_speed[1]:
            print('Going forward...')
            noAction = False
            for i in range(1,REST_STEPS+1,1):
                left_vel = MAX_SPEED * (cur_left_speed + float((r_speed[0]-cur_left_speed)*i/REST_STEPS))
                right_vel = MAX_SPEED * (cur_right_speed + float((r_speed[1]-cur_right_speed)*i/REST_STEPS))
                pub_left_vel.publish(left_vel)
                pub_right_vel.publish(right_vel)
                #print('left_vel='+str(left_vel))  #-------
                r.sleep()
            cur_left_speed = r_speed[0]
            cur_right_speed = r_speed[1]
            #print('cur='+str(cur_left_speed)+'  r_sp'+str(r_speed[0]))  #-------
            print('done')

    elif robot_motion == 2:
        r = rospy.Rate(20)
        #print('cur='+str(cur_left_speed)+'  r_sp'+str(r_speed[0]))  #-------
        if cur_left_speed != r_speed[0]*-1 or cur_right_speed != r_speed[1]*-1:
            print('Going backward...')
            noAction = False
            for i in range(1,REST_STEPS+1,1):
                left_vel = MAX_SPEED * (cur_left_speed + (r_speed[0]*-1-cur_left_speed)*float(i)/REST_STEPS)
                right_vel = MAX_SPEED * (cur_right_speed + (r_speed[1]*-1-cur_right_speed)*float(i)/REST_STEPS)
                pub_left_vel.publish(left_vel)
                pub_right_vel.publish(right_vel)
                #print('left_vel='+str(left_vel))  #-------
                r.sleep()
            cur_left_speed = r_speed[0]*-1
            cur_right_speed = r_speed[1]*-1
            #print('cur='+str(cur_left_speed)+'  r_sp'+str(r_speed[0]))  #-------
            print('done')

    elif robot_motion == 3:
        noAction = False
        print('turning left...')
        r = rospy.Rate(10)
        left_vel = MAX_SPEED * -0.1
        right_vel = MAX_SPEED * 0.1
        pub_left_vel.publish(left_vel)
        pub_right_vel.publish(right_vel)
        r.sleep()
        cur_left_speed = -0.1
        cur_right_speed = 0.1
        print('done')

    elif robot_motion == 4:
        noAction = False
        print('turning right...')
        r = rospy.Rate(10)
        left_vel = MAX_SPEED * 0.1
        right_vel = MAX_SPEED * -0.1
        pub_left_vel.publish(left_vel)
        pub_right_vel.publish(right_vel)
        r.sleep()
        cur_left_speed = 0.1
        cur_right_speed = -0.1
        print('done')
        
    if noAction == False :    
        print("current:\tleft_vel: %s\tright_vel: %s\tfront_degree:%s\trear_degree:%s\t" \
            %(cur_left_speed, cur_right_speed, 180*front_degree/math.pi, 180*rear_degree/math.pi))


def lMA_front_callback(data):
    global pub_front_flipper,pub_rear_flipper
    global angle_step,front_speed,front_degree,rear_speed,rear_degree,leg_HO
    global sub_once, cur_left_speed, cur_right_speed
    leg_MA = np.array([0, 0])
    leg_MA[0] = data.data[0]  #front
    leg_MA[1] = data.data[1]  #rear
    #sub_once = rospy.Subscriber('/leg_HO', Int32MultiArray, cb_leg_HO)
    endWhile = False
    while endWhile == False:
        t_step = leg_HO[0]-leg_MA[0]/4550  #HOME
        sub_once = rospy.Subscriber('/pairbot/joint_left_front_flipper_controller/command', Float64, cb_front_degree)
        print('HO[0]='+str(leg_HO[0])+'- MA[0]='+str(leg_MA[0]/4550))   
        cur_step = int(front_degree/angle_step)
        if t_step == cur_step:
            print('t_step=%s == cur_step=%s, break'%(t_step, cur_step))
            break
        print('moving front flipper to {0}....'.format(180*t_step*angle_step/math.pi))
        threading._start_new_thread(job_front_flipper_buffer, (t_step,cur_step,))
        # r = rospy.Rate(20)
        # direction = (t_step-cur_step)/abs(t_step-cur_step)
        # while cur_step!=t_step:
        #     cur_step+=direction
        #     front_degree = angle_step*cur_step
        #     if abs(front_degree)<0.0001:
        #         front_degree = 0
        #     pub_front_flipper.publish(front_degree)
        #     r.sleep()
        print('move front flipper done')
            
        endWhile = True
        print("current:\tleft_vel: %s\tright_vel: %s\tfront_degree:%s\trear_degree:%s\t" \
        %(cur_left_speed, cur_right_speed, 180*front_degree/math.pi, 180*rear_degree/math.pi))

def lMA_rear_callback(data):
    global pub_front_flipper,pub_rear_flipper
    global angle_step,front_speed,front_degree,rear_speed,rear_degree,leg_HO
    global sub_once, cur_left_speed, cur_right_speed
    leg_MA = np.array([0, 0])
    leg_MA[0] = data.data[0]  #front
    leg_MA[1] = data.data[1]  #rear
    #sub_once = rospy.Subscriber('/leg_HO', Int32MultiArray, cb_leg_HO)
    endWhile = False
    while endWhile == False:
        t_step = leg_HO[1]-leg_MA[1]/4550
        sub_once = rospy.Subscriber('/pairbot/joint_left_rear_flipper_controller/command', Float64, cb_rear_degree)
        print('HO[1]='+str(leg_HO[1])+'- MA[1]='+str(leg_MA[1]/4550))   
        cur_step = int(rear_degree/angle_step)
        if t_step == cur_step:
            print('t_step=%s == cur_step=%s, break'%(t_step, cur_step))
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
        print('move rear flipper done')
            
        endWhile = True
        print("current:\tleft_vel: %s\tright_vel: %s\tfront_degree:%s\trear_degree:%s\t" \
        %(cur_left_speed, cur_right_speed, 180*front_degree/math.pi, 180*rear_degree/math.pi))
    
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
    global leg_HO, front_degree, rear_degree, sub_once
    sub_once = rospy.Subscriber('/pairbot/joint_left_front_flipper_controller/command', Float64, cb_front_degree)
    sub_once = rospy.Subscriber('/pairbot/joint_left_rear_flipper_controller/command', Float64, cb_rear_degree)
    leg_HO[0] = front_degree*180/math.pi + data.data[0]  #front
    leg_HO[1] = rear_degree*180/math.pi + data.data[1]  #rear
    print('set HO*****0*****'+str(leg_HO[0]))
    print('set HO*****1*****'+str(leg_HO[1]))
    #sub_once.unregister()

def cb_robot_speed(data):
    global r_speed
    r_speed[0] = data.data[0]/1250.0  #left wheels
    r_speed[1] = data.data[1]/1250.0  #right wheels
    #print(r_speed[0])
    #sub_once.unregister()



REST_STEPS=20

def listener():
    rospy.init_node('listener', anonymous=True)
 
    global pub_vel,pub_front_flipper,pub_rear_flipper,pub_left_vel,pub_right_vel
    pub_left_vel = rospy.Publisher('/pairbot/left/command', Float64, queue_size=10)
    pub_right_vel = rospy.Publisher('/pairbot/right/command', Float64, queue_size=10)
    pub_front_flipper = rospy.Publisher('/pairbot/joint_left_front_flipper_controller/command', Float64, queue_size = 10)
    pub_rear_flipper = rospy.Publisher('/pairbot/joint_left_rear_flipper_controller/command', Float64, queue_size = 10)
    

    # base
    global speed_step,angle_step,front_speed,front_degree,rear_speed,rear_degree,cur_left_speed, cur_right_speed, r_speed

    speed_step = 0.1
    angle_step = math.pi/180
    # front flipper
    front_speed = 0
    front_degree = 0
    # rear flipper
    rear_speed = 0
    rear_degree = 0
    cur_left_speed = 0.0
    cur_right_speed = 0.0
    r_speed[0] = 0.0  #left wheels
    r_speed[1] = 0.0  #right wheels

    print("Ready for subscribe teleop")
    # rospy.Subscriber("robot_MA", Int32MultiArray, rMA_callback)
    rospy.Subscriber("leg_MA", Int32MultiArray, lMA_front_callback)
    rospy.Subscriber("leg_MA", Int32MultiArray, lMA_rear_callback)
    rospy.Subscriber("robot_motion", Int32, rM_callback)
    rospy.Subscriber('/robot_speed', Int32MultiArray, cb_robot_speed)
    rospy.Subscriber('/leg_HO', Int32MultiArray, cb_leg_HO)
    rospy.spin()

if __name__=="__main__":
    settings = termios.tcgetattr(sys.stdin)
    listener()
    
termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
