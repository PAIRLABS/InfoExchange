#!/usr/bin/env python
import rospy
from gazebo_msgs.srv import GetLinkState
from std_msgs.msg import Float64
from control_msgs.msg import JointControllerState
from geometry_msgs.msg import Twist
from gazebo_msgs.msg import LinkStates
import tf


MAX_SPEED=3
MAX_DELTA_SPEED=MAX_SPEED
MM_front_degree=0


def getfront_degree():
    return MM_front_degree

def callback(data):
    global MM_front_degree
    MM_front_degree = data.data
    print('MMfd='+str(MM_front_degree))


def callback_transformation(data):
    index=data.name.index("pairbot::base_link")
    pose=data.pose[index]
    ns = rospy.get_param('tf_prefix', '')
    base_link = ns + "/base_link"
    odom = ns + "/odom"
    br = tf.TransformBroadcaster()
    br.sendTransform((pose.position.x, pose.position.y, pose.position.z),
                     (pose.orientation.x, pose.orientation.y, pose.orientation.z, pose.orientation.w),
                     rospy.Time.now(),
                     base_link,
                     odom)

def listener():
    print('in listener')    
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber('/pairbot/joint_left_front_flipper_controller/command', Float64, callback)
    #rospy.Subscriber('/gazebo/link_states', LinkStates, callback_transformation)
    rospy.spin()


#if __name__ == 'pb_teleop':
listener()
