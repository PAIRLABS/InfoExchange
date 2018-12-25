#!/usr/bin/env python

from __future__ import print_function

import roslib; roslib.load_manifest('pb_teleop')
import rospy

from geometry_msgs.msg import Twist
from std_msgs.msg import Float64

import sys, select, termios, tty, math

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
    'u':(1,0),
    'i':(STOP,0),
    'o':(-1,0),
    '7':(0,1),
    'y':(0,-1)
}

rearFlipper = {
    #'key':(speed, degree)
    'j':(1,0),
    'k':(STOP,0),
    'l':(-1,0),
    'h':(0,1),
    'n':(0,-1)
}

def getKey():
    tty.setraw(sys.stdin.fileno())
    select.select([sys.stdin], [], [], 0)
    key = sys.stdin.read(1)
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key


if __name__=="__main__":
    settings = termios.tcgetattr(sys.stdin)

    pub_vel = rospy.Publisher('/cmd_vel', Twist, queue_size = 10)
    pub_front_flipper = rospy.Publisher('/pairbot/joint_left_front_flipper_controller/command', Float64, queue_size = 10)
    pub_front_flipper_wheel = rospy.Publisher('/pairbot/joint_left_front_flipper_wheel_controller/command', Float64, queue_size = 10)
    pub_rear_flipper = rospy.Publisher('/pairbot/joint_left_rear_flipper_controller/command', Float64, queue_size = 10)
    pub_rear_flipper_wheel = rospy.Publisher('/pairbot/joint_left_rear_flipper_wheel_controller/command', Float64, queue_size = 10)
    
    rospy.init_node('pb_teleop')

    # base
    x = 0
    y = 0
    z = 0
    th = 0

    speed_step = 0.1
    angle_step = math.pi/36
    # front flipper
    front_speed = 0
    front_degree = 0
    # rear flipper
    rear_speed = 0
    rear_degree = 0

    try:
        twist = Twist()
        twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0
        twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0
        pub_vel.publish(twist)
        pub_front_flipper.publish(0.0)
        pub_front_flipper_wheel.publish(0.0)
        pub_rear_flipper.publish(0.0)
        pub_rear_flipper_wheel.publish(0.0)
        
        print(msg)
        while(1):
            key = getKey()
            if key in baseMove.keys(): #'key':(x, y, z, angular)
                if baseMove[key][0] == STOP:
                    x = 0
                    th = 0
                else:
                    x += speed_step*baseMove[key][0]
                    th += angle_step*baseMove[key][3]
                twist = Twist()
                twist.linear.x = x
                twist.linear.y = 0
                twist.linear.z = 0
                twist.angular.x = 0
                twist.angular.y = 0 
                twist.angular.z = th
                pub_vel.publish(twist)
            elif key in frontFlipper.keys(): #'key':(speed, degree)
                if frontFlipper[key][1] != 0:
                    front_degree += angle_step*frontFlipper[key][1]
                    if abs(front_degree)<0.0001:
                        front_degree = 0
                    pub_front_flipper.publish(front_degree)
                else:
                    if frontFlipper[key][0] == STOP:
                        front_speed = 0
                    else:
                        front_speed += speed_step*frontFlipper[key][0]
                    pub_front_flipper_wheel.publish(front_speed)
            elif key in rearFlipper.keys(): #'key':(speed, degree)
                if rearFlipper[key][1] != 0:
                    rear_degree += angle_step*rearFlipper[key][1]
                    if abs(rear_degree)<0.0001:
                        rear_degree = 0
                    pub_rear_flipper.publish(rear_degree)
                else:
                    if rearFlipper[key][0] == STOP:
                        rear_speed = 0
                    else:
                        rear_speed += speed_step*rearFlipper[key][0]
                    pub_rear_flipper_wheel.publish(rear_speed)
            elif key == 'q':
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
                pub_front_flipper_wheel.publish(front_speed)
                pub_rear_flipper.publish(rear_degree)
                pub_rear_flipper_wheel.publish(rear_speed)
            elif (key == '\x03'):
                print('Leaving......')
                break;
            print("current:\tx: %s\ttheta: %s\tfront_speed:%s\tfront_degree:%s\trear_speed:%s\trear_degree:%s\t" \
                  %(x, th, front_speed, front_degree, rear_speed, rear_degree))
    except Exception as e:
        print(e)

    finally:
        twist = Twist()
        twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0
        twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0
        pub_vel.publish(twist)
        pub_front_flipper.publish(0.0)
        pub_front_flipper_wheel.publish(0.0)
        pub_rear_flipper.publish(0.0)
        pub_rear_flipper_wheel.publish(0.0)

termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)