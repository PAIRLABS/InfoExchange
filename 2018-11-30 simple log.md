# 2018-11-30 simple log

> 1. zed on turtlebot3 (urdf)
> 2. to do list

## 1. zed on turtlebot3

#### step0 : locate

> before start, I suggest you to install 'locate', which is useful for find the file you need in complex folders.

```
$ sudo apt-get update
$ sudo apt-get install mlocate
$ sudo updatedb
$ locate [filename]
```

#### step1: roslaunch :

```
$ export TURTLEBOT3_MODEL=burger
$ source devel/setup.bash
$ roslaunch turtlebot3_gazebo fake_turtlebot3_house.launch
```

​	and u can see zed on turtlebot3 in a house.

#### step2: check urdf file: 

```
$ locate fake_turtlebot3_burger.gazebo.xacro
$ locate fake_turtlebot3_burger.urdf.xacro
```

​	check `fake_turtlebot3_burger.gazebo.xacro`, u can find three camera(sensor & plugin) on turtlebot3 in the end of code.

#### step3: visualize depth camera on rviz:

```
$rosrun rviz rviz
```

1. modify fixed frame to `camera_link`
2. "Add" by topic: find `/camera_depth` and u can ad PointCloud2 or depthImage.



1. ## to do list

   1. understand how to transform depthimage to laserscan, Andy gogo.

      > http://wiki.ros.org/depthimage_to_laserscan

   1. my depth camera does not public the camera_Info, why ?????
