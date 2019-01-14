#!/usr/bin/env python
import rospy
from std_msgs.msg import Float64

def divide(a,b):
    return Float64(a.data/b.data)

pubs = []

# right flipper is a mirror of right flipper, so angles of flippers are inversed (i,-i)
def callback(data):
    #coef=Float64(0.7)
    #res=divide(data,coef)
    #    rospy.loginfo(data)
    rospy.logdebug("callback for right wheel")
    rospy.logdebug("%s received!", str(data))
    for p in pubs:
        p.publish(data)

T_RIGHT_BASE_WHEELS = "/pairbot/joint_right_base_wheel_{0}_controller/command"
T_RF_FW  = '/pairbot/joint_right_front_flipper_wheel_controller/command'
T_RR_FW  = '/pairbot/joint_right_rear_flipper_wheel_controller/command'
T_RF_FMW = '/pairbot/joint_right_front_flipper_middle_wheel_{0}_controller/command'
T_RR_FMW = '/pairbot/joint_right_rear_flipper_middle_wheel_{0}_controller/command'

def listener():
    rospy.init_node('listener', anonymous=True, log_level=rospy.DEBUG)
    pubs.extend([ rospy.Publisher(T_RIGHT_BASE_WHEELS.format(i+1), Float64, queue_size=10) for i in range(12)])
    rospy.logdebug("There are %s publishers for right base wheels", str(len(pubs)))
    base_wheel_number = len(pubs)

    pubs.append(rospy.Publisher(T_RF_FW, Float64, queue_size=10))
    pubs.append(rospy.Publisher(T_RR_FW, Float64, queue_size=10))
    pubs.extend([ rospy.Publisher(T_RF_FMW.format(i+1), Float64, queue_size=10) for i in range(6)])
    pubs.extend([ rospy.Publisher(T_RR_FMW.format(i+1), Float64, queue_size=10) for i in range(6)])
    rospy.logdebug("There are %s publishers for right flipper wheels", str(len(pubs)-base_wheel_number))

    rospy.Subscriber("/pairbot/right/command", Float64, callback)
    rospy.spin()

if __name__ == '__main__':
    listener()