turtlebot 3 gazeobo
===

- This program will let two robot simulate in gazebo and rviz
- It will have collision detection that made in the gazebo

#### in the node control
- I found that there is similar in the gazebo node and the turtlebot3_fake node(in the previous node)
- the `teleop` publish the `cmd_vel` with the linear speed and angular speed.
- Both the gazebo and the fake node will transform the speed to the /tf.
- In the `turtlebot3_burger.urdf.xacro`, it will include the `<xacro:include filename="$(find turtlebot3_description)/urdf/turtlebot3_burger.gazebo.xacro"/>`, and in this file it is the script about the speed transform to the tf.
