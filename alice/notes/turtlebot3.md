Ros Tutorial
===
> last updated on 20181109
> 
### Create a ROS Workspace and download TurtleBot3 metapackages and dependent ROS packages
```shell
cd ~/catkin_ws/src/
git clone https://github.com/ROBOTIS-GIT/turtlebot3_simulations.git
git clone https://github.com/ROBOTIS-GIT/turtlebot3_msgs.git
git clone https://github.com/ROBOTIS-GIT/turtlebot3.git
cd ~/catkin_ws
source devel/setup.bash
catkin_make
```
### Use turtlebot3_fake to run a simple simulation node

```shell
export TURTLEBOT3_MODEL=${TB3_MODEL}
roslaunch turtlebot3_fake turtlebot3_fake.launch
roslaunch turtlebot3_teleop turtlebot3_teleop_key.launch
```

Specify the model name of TurtleBot3. The `${TB3_MODEL}` is the name of the model you are using in `burger`, `waffle`, `waffle_pi`. If you want to permanently set the export settings, please refer to [Export TURTLEBOT3_MODEL](http://emanual.robotis.com/docs/en/platform/turtlebot3/export_turtlebot3_model) page.

### Simulate TurtleBot3 with Gazebo

There are two ways:

1. use with ROS through `turtlebot3_gazebo` package.
2. use only gazebo and `turtlebot3_gazebo_plugin` without using ROS.

#### Method1: package

```shell
export TURTLEBOT3_MODEL=${TB3_MODEL}
roslaunch turtlebot3_gazebo turtlebot3_empty_world.launch
roslaunch turtlebot3_gazebo turtlebot3_world.launch
```

http://emanual.robotis.com/docs/en/platform/turtlebot3/simulation/#ros-packages-for-gazebo

11. 2. 1. 5. #### [Virtual Navigation with TurtleBot3](http://emanual.robotis.com/docs/en/platform/turtlebot3/simulation/#virtual-navigation-with-turtlebot3)