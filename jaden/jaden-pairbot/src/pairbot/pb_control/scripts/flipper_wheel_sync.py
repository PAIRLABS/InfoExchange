#!/usr/bin/env python
import rospy
from std_msgs.msg import Float64
from control_msgs.msg import JointControllerState

T_LF_FW = '/pairbot/joint_left_front_flipper_wheel_controller/command'
T_LF_FMW = '/pairbot/joint_left_front_flipper_middle_wheel_{0}_controller/command'
T_RF_FW = '/pairbot/joint_right_front_flipper_wheel_controller/command'
T_RF_FMW = '/pairbot/joint_right_front_flipper_middle_wheel_{0}_controller/command'

T_LR_FW = '/pairbot/joint_left_rear_flipper_wheel_controller/command'
T_LR_FMW = '/pairbot/joint_left_rear_flipper_middle_wheel_{0}_controller/command'
T_RR_FW = '/pairbot/joint_right_rear_flipper_wheel_controller/command'
T_RR_FMW = '/pairbot/joint_right_rear_flipper_middle_wheel_{0}_controller/command'

pubs_front = []
pubs_rear = []

def sync_init():
    #pubs_front.append(rospy.Publisher(T_LF_FW, Float64, queue_size=10))
    pubs_front.extend([ rospy.Publisher(T_LF_FMW.format(i+1), Float64, queue_size=10) for i in range(6)])
    pubs_front.append(rospy.Publisher(T_RF_FW, Float64, queue_size=10))
    pubs_front.extend([ rospy.Publisher(T_RF_FMW.format(i+1), Float64, queue_size=10) for i in range(6)])
    #pubs_rear.append(rospy.Publisher(T_LR_FW, Float64, queue_size=10))
    pubs_rear.extend([ rospy.Publisher(T_LR_FMW.format(i+1), Float64, queue_size=10) for i in range(6)])
    pubs_rear.append(rospy.Publisher(T_RR_FW, Float64, queue_size=10))
    pubs_rear.extend([ rospy.Publisher(T_RR_FMW.format(i+1), Float64, queue_size=10) for i in range(6)])
    

# Left flipper is a mirror of right flipper, so angles of flippers are inversed (i,-i)
def callback_for_front_flipper(data):
    rospy.logdebug("callback for front flipper wheel")
    rospy.logdebug("There are %s publishers", str(len(pubs_front)))
    for p in pubs_front:
        rospy.logdebug("%s received!", str(data))
        p.publish(data)

def callback_for_rear_flipper(data):
    rospy.logdebug("callback for rear flipper wheel")
    rospy.logdebug("There are %s publishers", str(len(pubs_rear)))
    for p in pubs_rear:
        p.publish(data)

def listener():
    rospy.init_node('listener', anonymous=True, log_level=rospy.DEBUG)
    sync_init()
    rospy.logdebug("init done!")
    rospy.logdebug("There are %s publishers in front", str(len(pubs_front)))
    rospy.logdebug("There are %s publishers in rear", str(len(pubs_rear)))
    rospy.Subscriber("/pairbot/joint_left_front_flipper_wheel_controller/command", Float64, callback_for_front_flipper)
    #rospy.Subscriber("/pairbot/joint_right_front_flipper_wheel_controller/command", JointControllerState, callback_for_front_flipper)
    rospy.Subscriber("/pairbot/joint_left_rear_flipper_wheel_controller/command", Float64, callback_for_rear_flipper)
    #rospy.Subscriber("/pairbot/joint_right_rear_flipper_wheel_controller/command", JointControllerState, callback_for_rear_flipper)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


if __name__ == '__main__':
    listener()
