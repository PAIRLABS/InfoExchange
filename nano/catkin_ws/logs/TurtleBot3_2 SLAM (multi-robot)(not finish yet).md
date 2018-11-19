## TurtleBot3 control practice (multi-robot & SLAM) (not finish yet)

------

> note : This instructions were tested on `Ubuntu 16.04` and `ROS Kinetic Kame`.

> this tutorial's goal is to 

> if you clone my file directly, you can jump to step 3.

------

## step 0: install locate

> before start, I suggest you to install 'locate', which is useful for find the file you need in complex folders.

```
$ sudo apt-get update
$ sudo apt-get install mlocate
$ sudo updatedb
$ locate [filename]
```

------

## step 1: setup

```
$ cd ~/catkin_ws/src/
$ git clone https://github.com/ROBOTIS-GIT/turtlebot3_msgs.git
$ git clone https://github.com/ROBOTIS-GIT/turtlebot3.git
$ git clone https://github.com/ROBOTIS-GIT/turtlebot3_simulations.git
```

------

## step 2: catkin_make

```
$ cd ~/catkin_ws
$ catkin_make
```

------



## step 3: execute multi_turtlebot3.launch

> Execute multi_turtlebot3.launch in package turtlebot3_gazebo, which may spawn three tuetlebot3 in house.

```
$ export TURTLEBOT3_MODEL=waffle_pi
$ source devel/setup.bash
$ roslaunch turtlebot3_gazebo multi_turtlebot3.launch
```

By the way, you can control these turtlebot3 with teleoperation node, in new terminal :

> namespace : tb3_0, tb3_1, tb3_2

```
$ cd ~/catkin_ws
$ export TURTLEBOT3_MODEL=waffle_pi
$ source devel/setup.bash
$ ROS_NAMESPACE=tb3_0 rosrun turtlebot3_teleop turtlebot3_teleop_key
```

### 1. code display (multi_turtlebot3.launch) :

```
<!-- multi_turtlebot3.launch -->
<launch>
  <arg name="model" default="$(env TURTLEBOT3_MODEL)" doc="model type [burger, waffle, waffle_pi]"/>
  <arg name="first_tb3"  default="tb3_0"/>
  <arg name="second_tb3" default="tb3_1"/>
  <arg name="third_tb3"  default="tb3_2"/>

  <arg name="first_tb3_x_pos" default="-7.0"/>
  <arg name="first_tb3_y_pos" default="-1.0"/>
  <arg name="first_tb3_z_pos" default=" 0.0"/>
  <arg name="first_tb3_yaw"   default=" 1.57"/>

  <arg name="second_tb3_x_pos" default=" 7.0"/>
  <arg name="second_tb3_y_pos" default="-1.0"/>
  <arg name="second_tb3_z_pos" default=" 0.0"/>
  <arg name="second_tb3_yaw"   default=" 1.57"/>

  <arg name="third_tb3_x_pos" default=" 0.5"/>
  <arg name="third_tb3_y_pos" default=" 3.0"/>
  <arg name="third_tb3_z_pos" default=" 0.0"/>
  <arg name="third_tb3_yaw"   default=" 0.0"/>

  <include file="$(find gazebo_ros)/launch/empty_world.launch">
    <arg name="world_name" value="$(find turtlebot3_gazebo)/worlds/turtlebot3_house.world"/>
    <arg name="paused" value="false"/>
    <arg name="use_sim_time" value="true"/>
    <arg name="gui" value="true"/>
    <arg name="headless" value="false"/>
    <arg name="debug" value="false"/>
  </include>  

  <group ns = "$(arg first_tb3)">
    <param name="robot_description" command="$(find xacro)/xacro --inorder $(find turtlebot3_description)/urdf/turtlebot3_$(arg model).urdf.xacro" />

    <node pkg="robot_state_publisher" type="robot_state_publisher" name="robot_state_publisher" output="screen">
      <param name="publish_frequency" type="double" value="50.0" />
      <param name="tf_prefix" value="$(arg first_tb3)" />
    </node>
    
    <node name="spawn_urdf" pkg="gazebo_ros" type="spawn_model" args="-urdf -model $(arg first_tb3) -x $(arg first_tb3_x_pos) -y $(arg first_tb3_y_pos) -z $(arg first_tb3_z_pos) -Y $(arg first_tb3_yaw) -param robot_description" />
  </group>

  <group ns = "$(arg second_tb3)">
    <param name="robot_description" command="$(find xacro)/xacro --inorder $(find turtlebot3_description)/urdf/turtlebot3_$(arg model).urdf.xacro" />

    <node pkg="robot_state_publisher" type="robot_state_publisher" name="robot_state_publisher" output="screen">
      <param name="publish_frequency" type="double" value="50.0" />
      <param name="tf_prefix" value="$(arg second_tb3)" />
    </node>

    <node name="spawn_urdf" pkg="gazebo_ros" type="spawn_model" args="-urdf -model $(arg second_tb3) -x $(arg second_tb3_x_pos) -y $(arg second_tb3_y_pos) -z $(arg second_tb3_z_pos) -Y $(arg second_tb3_yaw) -param robot_description" />
  </group>

  <group ns = "$(arg third_tb3)">
    <param name="robot_description" command="$(find xacro)/xacro --inorder $(find turtlebot3_description)/urdf/turtlebot3_$(arg model).urdf.xacro" />

    <node pkg="robot_state_publisher" type="robot_state_publisher" name="robot_state_publisher" output="screen">
      <param name="publish_frequency" type="double" value="50.0" />
      <param name="tf_prefix" value="$(arg third_tb3)" />
    </node>

    <node name="spawn_urdf" pkg="gazebo_ros" type="spawn_model" args="-urdf -model $(arg third_tb3) -x $(arg third_tb3_x_pos) -y $(arg third_tb3_y_pos) -z $(arg third_tb3_z_pos) -Y $(arg third_tb3_yaw) -param robot_description" />
  </group>

</launch>
<!-- multi_turtlebot3.launch -->
```

### 2. code explanation (multi_turtlebot3.launch):

#### 	1. set argument for each turtlebot :

```
<!-- multi_turtlebot3.launch -->
<launch>
  <arg name="model" default="$(env TURTLEBOT3_MODEL)" doc="model type [burger, waffle, waffle_pi]"/>
  <arg name="first_tb3"  default="tb3_0"/>
  <arg name="second_tb3" default="tb3_1"/>
  <arg name="third_tb3"  default="tb3_2"/>

  <arg name="first_tb3_x_pos" default="-7.0"/>
  <arg name="first_tb3_y_pos" default="-1.0"/>
  <arg name="first_tb3_z_pos" default=" 0.0"/>
  <arg name="first_tb3_yaw"   default=" 1.57"/>

  <arg name="second_tb3_x_pos" default=" 7.0"/>
  <arg name="second_tb3_y_pos" default="-1.0"/>
  <arg name="second_tb3_z_pos" default=" 0.0"/>
  <arg name="second_tb3_yaw"   default=" 1.57"/>

  <arg name="third_tb3_x_pos" default=" 0.5"/>
  <arg name="third_tb3_y_pos" default=" 3.0"/>
  <arg name="third_tb3_z_pos" default=" 0.0"/>
  <arg name="third_tb3_yaw"   default=" 0.0"/>
```

#### 2. setup scene in gazebo

>   setup scene in gazebo (you will see a house in gazebo)

```
  <include file="$(find gazebo_ros)/launch/empty_world.launch">
    <arg name="world_name" value="$(find turtlebot3_gazebo)/worlds/turtlebot3_house.world"/>
    <arg name="paused" value="false"/>
    <arg name="use_sim_time" value="true"/>
    <arg name="gui" value="true"/>
    <arg name="headless" value="false"/>
    <arg name="debug" value="false"/>
  </include>  
```

#### 3. spawn turtleboe3 by each namespace and argument

> skip   `second_tb3 `and `third_tb3`, which are similar with `first_tb3`

```
 <group ns = "$(arg first_tb3)">
    <param name="robot_description" command="$(find xacro)/xacro --inorder $(find turtlebot3_description)/urdf/turtlebot3_$(arg model).urdf.xacro" />

    <node pkg="robot_state_publisher" type="robot_state_publisher" name="robot_state_publisher" output="screen">
      <param name="publish_frequency" type="double" value="50.0" />
      <param name="tf_prefix" value="$(arg first_tb3)" />
    </node>
    
    <node name="spawn_urdf" pkg="gazebo_ros" type="spawn_model" args="-urdf -model $(arg first_tb3) -x $(arg first_tb3_x_pos) -y $(arg first_tb3_y_pos) -z $(arg first_tb3_z_pos) -Y $(arg first_tb3_yaw) -param robot_description" />
  </group>
```

1. In parameter ``robot_description`, `turtlebot3_$(arg model).urdf.xacro` is a urdf file which **define the physical properties of link and joint and their relationship** of turtlebot3 model **in gazebo node**, and then publish to topic `/ns/joint_states`.
2. node `robot_state_publisher` will broadcast the transform relationship of frame of turtlebot3's link in tf form.
3. node `spawn_urdf` will spawn our turtlebot3 model for each namespace by argument we have already set.

#### 4. rosrun rqt_graph and rqt_tf_tree

```
$ rosrun rqt_graph rqt_graph
```
![image](https://github.com/Nano1201/InfoExchange/blob/master/nano/catkin_ws/image/SLAM_TurtleBot3_image/rqt_graph_1.PNG)  

> Q : what is `/tf_static` ?
>
> A :  reference by [tf and tf2](http://wiki.ros.org/tf2/Migration#Addition_of_.2BAC8-tf_static_topic) , `/tf_static` has the same format with `/tf`, but transforms on the /tf_static topic are assumed to never change and be valid for all timestamps.  

```
$ rosrun rqt_tf_tree rqt_tf_tree
```

> we skip tb3_0 and tb3_1, which is similar with tb3_2

![image](https://github.com/Nano1201/InfoExchange/blob/master/nano/catkin_ws/image/SLAM_TurtleBot3_image/rqt_tf_tree_1.PNG)



## step 4: execute SLAM

> The **SLAM (Simultaneous Localization and Mapping)** is a technique to draw a map by estimating current location in an arbitrary space (here is by **gmapping**)

### 1. execute in three new terminal :

> don't forget `$ export TURTLEBOT3_MODEL=waffle_pi` and `$ source devel/setup.bash`in these new terminal

```
$ ROS_NAMESPACE=tb3_0 roslaunch turtlebot3_slam turtlebot3_gmapping.launch set_base_frame:=tb3_0/base_footprint set_odom_frame:=tb3_0/odom set_map_frame:=tb3_0/map
```

```
$ ROS_NAMESPACE=tb3_1 roslaunch turtlebot3_slam turtlebot3_gmapping.launch set_base_frame:=tb3_1/base_footprint set_odom_frame:=tb3_1/odom set_map_frame:=tb3_1/map
```

```
$ ROS_NAMESPACE=tb3_2 roslaunch turtlebot3_slam turtlebot3_gmapping.launch set_base_frame:=tb3_2/base_footprint set_odom_frame:=tb3_2/odom set_map_frame:=tb3_2/map
```

### 2. code display (turtlebot3_gmapping.launch) :

```
<!-- turtlebot3_gmapping.launch  -->
<launch>
  <!-- Arguments -->
  <arg name="model" default="$(env TURTLEBOT3_MODEL)" doc="model type [burger, waffle, waffle_pi]"/>
  <arg name="configuration_basename" default="turtlebot3_lds_2d.lua"/>
  <arg name="set_base_frame" default="base_footprint"/>
  <arg name="set_odom_frame" default="odom"/>
  <arg name="set_map_frame"  default="map"/>

  <!-- Gmapping -->
  <node pkg="gmapping" type="slam_gmapping" name="turtlebot3_slam_gmapping" output="screen">
    <param name="base_frame" value="$(arg set_base_frame)"/>
    <param name="odom_frame" value="$(arg set_odom_frame)"/>
    <param name="map_frame"  value="$(arg set_map_frame)"/>
    <param name="map_update_interval" value="2.0"/> <!-- seconds -->
    <param name="maxUrange" value="3.0"/>
    <param name="sigma" value="0.05"/>
    <param name="kernelSize" value="1"/>
    <param name="lstep" value="0.05"/>
    <param name="astep" value="0.05"/>
    <param name="iterations" value="5"/>
    <param name="lsigma" value="0.075"/>
    <param name="ogain" value="3.0"/>
    <param name="lskip" value="0"/>
    <param name="minimumScore" value="50"/>
    <param name="srr" value="0.1"/>
    <param name="srt" value="0.2"/>
    <param name="str" value="0.1"/>
    <param name="stt" value="0.2"/>
    <param name="linearUpdate" value="1.0"/>
    <param name="angularUpdate" value="0.2"/>
    <param name="temporalUpdate" value="0.5"/>
    <param name="resampleThreshold" value="0.5"/>
    <param name="particles" value="100"/>
    <param name="xmin" value="-10.0"/>
    <param name="ymin" value="-10.0"/>
    <param name="xmax" value="10.0"/>
    <param name="ymax" value="10.0"/>
    <param name="delta" value="0.05"/>
    <param name="llsamplerange" value="0.01"/>
    <param name="llsamplestep" value="0.01"/>
    <param name="lasamplerange" value="0.005"/>
    <param name="lasamplestep" value="0.005"/>
  </node>
</launch>
<!-- turtlebot3_gmapping.launch  -->
```

### 3. node slam_gmapping explanation:

> The [gmapping](http://wiki.ros.org/gmapping) package is a ROS node called **slam_gmapping** which provide **laser-based SLAM**, by slam_gmapping, we can create a 2-D grid map  from laser and pose data collected by a mobile robot. 

> (We will merge these three map in step 5 and represent it on rviz in step 6.)

1.  Subscribe topic :
    1.  `tf` and `tf_static` ([tf/tfMessage](http://docs.ros.org/api/tf/html/msg/tfMessage.html)) : Transforms necessary to relate frames for laser, base, and odometry.
    2.   `scan` ([sensor_msgs/LaserScan](http://docs.ros.org/api/sensor_msgs/html/msg/LaserScan.html)) : used to create map mainly.
    
2.  Publish topic :
    1.  `map_metadata` ([nav_msgs/MapMetaData](http://docs.ros.org/api/nav_msgs/html/msg/MapMetaData.html)) : update meta data of map periodically (ex : map_load_time, width, height, origin...)
    2.  `map` ([nav_msgs/OccupancyGrid](http://docs.ros.org/api/nav_msgs/html/msg/OccupancyGrid.html)) : update  map periodically.
    3.  `tf`  ([tf/tfMessage](http://docs.ros.org/api/tf/html/msg/tfMessage.html)) : update the transform of map and odom (see rqt_tf_tree below).
3.  Service:
    1.  `dynamic_map` ([nav_msgs/GetMap](http://docs.ros.org/api/nav_msgs/html/srv/GetMap.html)) :  map data in service  type

4. Broadcast tf Transforms:
    1. `map` -> `odom` : the current estimate of the robot's pose within the map frame

5.  Parameters :  &*@#^(#@^(*&

### 4. rosrun rqt_graph and rqt_tf_tree

```
$ rosrun rqt_graph rqt_graph
```

![image](https://github.com/Nano1201/InfoExchange/blob/master/nano/catkin_ws/image/SLAM_TurtleBot3_image/rqt_graph_2.PNG)

```
$ rosrun rqt_tf_tree rqt_tf_tree
```

![image](https://github.com/Nano1201/InfoExchange/blob/master/nano/catkin_ws/image/SLAM_TurtleBot3_image/rqt_tf_tree_2.PNG)



<to be continue......>



> reference net : 
>
> 1. turtlebot3 : http://emanual.robotis.com/docs/en/platform/turtlebot3/simulation/#turtlebot3-simulation-using-fake-node
> 2. ROS-gmapping : http://wiki.ros.org/gmapping#Required_tf_Transforms
