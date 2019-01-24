#!/usr/bin/env python
import rospy
from std_msgs.msg import Float64

def divide(a,b):
    return Float64(a.data/b.data)

pubs = []

def callback(data):
    #coef=Float64(0.7)
    #res=divide(data,coef)
    #    rospy.loginfo(data)
    rospy.logdebug("callback for left wheel")
    rospy.logdebug("%s received!", str(data))
    for p in pubs:
        p.publish(data)

T_LEFT_BASE_WHEELS = "joint_left_base_wheel_{0}_controller/command"
T_LF_FW  = 'joint_left_front_flipper_wheel_controller/command'
T_LR_FW  = 'joint_left_rear_flipper_wheel_controller/command'
T_LF_FMW = 'joint_left_front_flipper_middle_wheel_{0}_controller/command'
T_LR_FMW = 'joint_left_rear_flipper_middle_wheel_{0}_controller/command'

def listener():
    rospy.init_node('listener', anonymous=True, log_level=rospy.DEBUG)
    pubs.extend([ rospy.Publisher(T_LEFT_BASE_WHEELS.format(i+1), Float64, queue_size=10) for i in range(12)])
    rospy.logdebug("There are %s publishers for left base wheels", str(len(pubs)))
    base_wheel_number = len(pubs)

    pubs.append(rospy.Publisher(T_LF_FW, Float64, queue_size=10))
    pubs.append(rospy.Publisher(T_LR_FW, Float64, queue_size=10))
    pubs.extend([ rospy.Publisher(T_LF_FMW.format(i+1), Float64, queue_size=10) for i in range(6)])
    pubs.extend([ rospy.Publisher(T_LR_FMW.format(i+1), Float64, queue_size=10) for i in range(6)])
    rospy.logdebug("There are %s publishers for left flipper wheels", str(len(pubs)-base_wheel_number))

    rospy.Subscriber("left/command", Float64, callback)
    rospy.spin()

if __name__ == '__main__':
    listener()
