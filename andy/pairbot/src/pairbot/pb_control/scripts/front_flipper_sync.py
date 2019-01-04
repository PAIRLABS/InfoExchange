#!/usr/bin/env python
import rospy
from std_msgs.msg import Float64
from control_msgs.msg import JointControllerState


# Left flipper is a mirror of right flipper, so angles of flippers are inversed (i,-i)
def callback_for_right_front_flipper(data):
    pub = rospy.Publisher('/pairbot/joint_left_front_flipper_controller/command', Float64, queue_size=10)
    pub.publish(data.data)

def callback_for_left_front_flipper(data):
    pub = rospy.Publisher('/pairbot/joint_right_front_flipper_controller/command', Float64, queue_size=10)
    rospy.logdebug("%s received!", str(data.data))
    pub.publish(data.data)

def listener():
    rospy.init_node('listener', anonymous=True, log_level=rospy.DEBUG)
    # rospy.Subscriber("/pairbot/joint_right_front_flipper_controller/command", Float64, callback_for_right_front_flipper)
    rospy.Subscriber("/pairbot/joint_left_front_flipper_controller/command", Float64, callback_for_left_front_flipper)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


if __name__ == '__main__':
    listener()
