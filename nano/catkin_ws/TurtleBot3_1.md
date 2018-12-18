# TurtleBot3 control practice (link and tf)

------

> note : This instructions were tested on `Ubuntu 16.04` and `ROS Kinetic Kame`.

> this tutorial's goal is to change the control node of turtlebot3's wheel by modifying `turtlebot3_fake.cpp`

> if you clone my file directly, you can jump to step 6.
------

## step 0: install locate

> before start, I suggest you to install 'locate', which is useful for find the file you need in complex folders.

```
$ sudo apt-get update
$ sudo apt-get install mlocate
$ sudo updatedb
$ locate [file_hard_to_find_QAQ]
```

------

## step 1: setup

```
$ cd ~/catkin_ws/src/
```

```
$ git clone https://github.com/ROBOTIS-GIT/turtlebot3_msgs.git
$ git clone https://github.com/ROBOTIS-GIT/turtlebot3.git
```

```
$ git clone https://github.com/ROBOTIS-GIT/turtlebot3_simulations.git
```

------

## step 2: catkin_make

```
$ cd ~/catkin_ws
$ catkin_make
```

------

## step 3: execute 

> Execute the turtlebot3_fake.launch file in the turtlebot3_fake package.  turtlebot3_fake is a very simple simulation node. Furthermore, we are going to control this virtual TurtleBot3 in Rviz with a teleoperation node :

### In first terminal :

```
$ export TURTLEBOT3_MODEL=burger
$ source ~/catkin_ws/devel/setup.bash
$ roslaunch turtlebot3_fake turtlebot3_fake.launch
```

### In second terminal :

```
$ export TURTLEBOT3_MODEL=burger
$ source ~/catkin_ws/devel/setup.bash
$ roslaunch turtlebot3_teleop turtlebot3_teleop_key.launch
```

Now, you can play with your Turtlerobot3 for a while, and I suggest you turn off the **odometry** and turn on **TF** in Rviz：

![image](https://github.com/Nano1201/InfoExchange/blob/master/nano/catkin_ws/image/turm_of_odom.PNG)  

------

## step 4: rosrun  rqt_graph & rqt_tf_tree

> to understand to node, topic, and frame of this project, let's run **rqt_graph** and **rqt_tf_tree**

```
$ rosrun rqt_graph rqt_graph
```

![image](https://github.com/Nano1201/InfoExchange/blob/master/nano/catkin_ws/image/rqt_0.PNG)

also, we can check the frame tree by ` rqt_tf_tree`

```
$ rosrun rqt_tf_tree rqt_tf_tree
```

![image](https://github.com/Nano1201/InfoExchange/blob/master/nano/catkin_ws/image/tf_tree_0.PNG)

note :

1. `basic_footprint` frame can be understood as a frame contain the whole robot (parent frame of all robot's frame)
2. `base_link` frame can be understood as a frame contain turtlebot3's body.
3. when`turtlebot3_fake_node` receive `/turtlebot3_teleop_keyborad`'s topic, it will update `base_footprint` frame's pose( origin and rotation ) by broadcast to `/tf` , in this case, the whole robot will move as you wish.
4. when robot is moving, the wheel will rotate with the linear velocity, but this rotation is not broadcast by `/turtlebot3_teleop_keyboaed` to `/tf` directly, it would be published to  `/robot_state_publisher` before broadcasted to `/tf`

------

**However, our goal is to transform this rotation not by `/robot_state_publisher`, but  from `/turtlebot3_teleop_keyboaed` to /`tf` directly.**

**In other words, topic `/joint_state` will not be public.**

------

## step 5 : modify turtlebot3_fake.cpp

> goal :
>
> 1. do not publish topic `/joint_state`
> 2. broadcast the rotation of `wheel_right_link` and `wheel_left_link` to `/tf` directly

------

1. find `turtlebot3_fake.cpp`

```
$ cd ~/catkin_ws/src/turtlebot3_simulations/turtlebot3_fake/src
$ vim turtlebot3_fake.cpp
```

1. find  `joint_states_pub_ = nh_.advertise<sensor_msgs::JointState>("joint_states", 100);` and comment it

```
//joint_states_pub_ = nh_.advertise<sensor_msgs::JointState>("joint_states", 100);
```

1. find `void Turtlebot3Fake::updateJoint(void) {...} ` and modify to :

```
void Turtlebot3Fake::updateJoint(void)
{
//  joint_states_.position[LEFT]  = last_position_[LEFT];
//  joint_states_.position[RIGHT] = last_position_[RIGHT];
//  joint_states_.velocity[LEFT]  = last_velocity_[LEFT];
//  joint_states_.velocity[RIGHT] = last_velocity_[RIGHT];

  static tf::TransformBroadcaster br_right;
  tf::Transform transform_right;
  transform_right.setOrigin( tf::Vector3(0.0,-0.080, 0.023) );
  tf::Quaternion q_right;
  q_right.setRPY(-1.57, last_position_[RIGHT] ,0 );
  transform_right.setRotation(q_right);
  br_right.sendTransform(tf::StampedTransform(transform_right, ros::Time::now(), "base_link", "wheel_right_link"));

  static tf::TransformBroadcaster br_left;
  tf::Transform transform_left;
  transform_left.setOrigin( tf::Vector3(0.0,0.08,0.023) );
  tf::Quaternion q_left;
  q_left.setRPY(-1.57, last_position_[LEFT] ,0 );
  transform_left.setRotation(q_left);
  br_left.sendTransform(tf::StampedTransform(transform_left, ros::Time::now(), "base_link", "wheel_left_link"));
}

```

> ### Q: how to set the transform of wheel?
>
> A: transform has two most important part-- Origin and rotation, which can describe the relationship of two frames, so we have to set Origin and RPY(rotation) here:
>
> 1. `setOrigin` :  setting the origin of transform. the value refer from `turtlebot3_burger.urdf.xacro`, which define the link's physical properties(viaual, collision, inertial....) of a robot, and connect these link by joint. You can find this file by `locate`.
> 2. [tf2::Quaternion::setRPY(const tf2Scalar &  roll,const tf2Scalar &  pitch,const tf2Scalar &  yaw )](http://docs.ros.org/jade/api/tf2/html/classtf2_1_1Quaternion.html#a2ffba08e3fb4c10f9d5cbd6fc43399de):  three parameter means rotation around x-axis y-axis z-axis.	 

1. find `joint_states_.header.stamp = time_now;` and ` joint_states_pub_.publish(joint_states_);` and comment :

```
  // joint_states
  updateJoint();
  //joint_states_.header.stamp = time_now;
  //joint_states_pub_.publish(joint_states_);
```



------

## step 6: catkin_make and execute

```
$ cd ~/catkin_ws
$ catkin_make
```

In first terminal:

```
$ export TURTLEBOT3_MODEL=burger
$ source ~/catkin_ws/devel/setup.bash
$ roslaunch turtlebot3_fake turtlebot3_fake.launch
```

In second terminal:

```
$ export TURTLEBOT3_MODEL=burger
$ source ~/catkin_ws/devel/setup.bash
$roslaunch turtlebot3_teleop turtlebot3_teleop_key.launch
```

and you can see the same scene with step 3, however, what's the difference?  Let's go step 6.

## step 7: rosrun  rqt_graph & rqt_tf_tree

let's check the `rqt_graph` and `rqt_tf_tree again`

```
$ rosrun rqt_graph rqt_graph
```

![image](https://github.com/Nano1201/InfoExchange/blob/master/nano/catkin_ws/image/rqt_1.PNG)

```
$ rosrun rqt_tf_tree rqt_tf_tree
```

![image](https://github.com/Nano1201/InfoExchange/blob/master/nano/catkin_ws/image/tf_tree_1.PNG)

  you can see that after `/turtlebot3_fake_node` receive topic from `/turtlebot3_teleop_keyboard`, it broadcast `/tf` directly and which cam achieve the same effect as the origin version of `turtlebot3_fake.cpp`.  And the broadcaster of `wheel_left_link` and `wheel_right_link` are changed to `/turtlebot3_fake_node`

#### However, one may wonder that what /robot_state_publisher broadcast to tf ?

ans :   In our new version of `turtlebot3_fake.cpp`, `/robot_state_publisher` broadcast the initial state of `base_footprint` and its child frame only in the beginning of rviz start. After that, all the pose of turtlebot3 is broadcasted by  `/turtlebot3_fake_node`.
