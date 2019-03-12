## Pairworld

- This example shows how to use a world package with your robot.
- These instructions were tested on `Ubuntu 16.04`, `ROS Kinetic` and `Gazebo 7`.
- Require `gazebo_ros`: http://gazebosim.org/tutorials?tut=ros_installing
- ``pairworld_gazebo`` is the package for this example. 
- See ``Launch Gazebo world.odp`` for more details.




## Steps:

Put ``pairworld_gazebo`` in any ``catkin_workspace/src``. Usually, it might be the workspace which contains your robot. 

**If you don't have a robot**...The next step is to install dependent packages for `TurtleBot3`, if you already have your own robot, you can skip this part:

```shell
$ sudo apt-get install ros-kinetic-joy ros-kinetic-teleop-twist-joy ros-kinetic-teleop-twist-keyboard ros-kinetic-laser-proc ros-kinetic-rgbd-launch ros-kinetic-depthimage-to-laserscan ros-kinetic-rosserial-arduino ros-kinetic-rosserial-python ros-kinetic-rosserial-server ros-kinetic-rosserial-client ros-kinetic-rosserial-msgs ros-kinetic-amcl ros-kinetic-map-server ros-kinetic-move-base ros-kinetic-urdf ros-kinetic-xacro ros-kinetic-compressed-image-transport ros-kinetic-rqt-image-view ros-kinetic-gmapping ros-kinetic-navigation ros-kinetic-interactive-markers
$ cd ~/catkin_ws/src/
$ git clone https://github.com/ROBOTIS-GIT/turtlebot3_msgs.git
$ git clone https://github.com/ROBOTIS-GIT/turtlebot3.git
$ git clone https://github.com/ROBOTIS-GIT/turtlebot3_simulations.git
$ cd ~/catkin_ws && catkin_make
```

`Turtlebot3` website:http://emanual.robotis.com/docs/en/platform/turtlebot3/simulation/#simulation



There are several ways to use this world package, as follows :

#### #1: Launch world and robot in separate files:

1. Launch the world:

   ```shell
   $ cd [Path to your workspace]
   $ catkin_make
   $ source devel/setup.bash
   $ roslaunch pairworld_gazebo watch_tower_world.launch
   ```

2. Add a robot in the world:

   In your robot's launch file, delete the code of your world part. In another terminal:

   ```shell
   $ source devel/setup.bash
   $ roslaunch [Your Robot package] [Robot without world].launch
   ```

Notice that “robot package” and “world package” should be in the same workspace.



#### #2: Launch world and robot at once:

1. Replace the code of the part where you startup your world in your launch file with following:

   ```launch
   <include file="$(find gazebo_ros)/launch/empty_world.launch">
       <arg name="world_name" value="$(find pairworld_gazebo)/worlds/watch_tower_world.world"/>
     </include> 
   ```

2. Launch:

   ```shell
   $ cd [Path to your workspace]
   $ catkin_make
   $ source devel/setup.bash
   $ roslaunch [Your Robot package] [Robot with world].launch
   ```


Notice that “robot package” and “world package” should be in the same workspace.



#### #3: Open the world with only Gazebo:

If you want to take a look the world without ROS:

1. In terminal move to the directory where you put this package:

   ```shell
   $ cd [Path to pairworld_gazebo]
   ```

2. Now, under `pairworld_gazebo` directory, set environment variable:

   ```shell
   $ export GAZEBO_MODEL_PATH="$(pwd)/.."
   ```

3. Open the world with gazebo:

   ```shell
   $ gazebo ./worlds/watch_tower_world.world
   ```

   It may take some time, if this is your first time running.

