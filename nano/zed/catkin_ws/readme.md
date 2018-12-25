# Install zed SDK and zed_ros_wrapper

> note : This instructions were tested on `Ubuntu 16.04`, `ROS Kinetic Kame` and `CUDA 9.0`.

step 1: download zed SDK 2.7.1 

 1. https://www.stereolabs.com/developers/release/#sdkdownloads_anchor

 2. download [ZED SDK for Ubuntu 16 v2.7 (CUDA 9)](https://download.stereolabs.com/zedsdk/2.7/ubuntu16_cuda9)   (older versions may have some bug)

 3. ```
    $ cd Downloads
    $ chmod +x ZED_SDK_Ubuntu16_CUDA9_v2.7.1.run
    $ sudo apt-get update
    $ ./ZED_SDK_Ubuntu16_CUDA9_v2.7.1.run
    ```

setp 2: install other necessary  Library dependencies:

```
    $ sudo apt-get install libpcl1
    $ sudo apt-get install ros-kinetic-pcl-ros
    $ sudo apt-get install ros-$(rosversion -d)-tf2-geometry-msgs
    $ sudo apt-get install ros-kinetic-rviz-imu-plugin (for zed mini visualize IMU information using Rviz)
```

step 3: download zed_ros_wrapper

```
   	$ cd ~/catkin_ws/src
    $ git clone https://github.com/stereolabs/zed-ros-wrapper.git
    $ cd ..
    $ catkin_make
    $ source devel/setup.bash
```

 step 4: testing

```
   	$ cd ~/catkin_ws/src
   	$ roslaunch zed_display_rviz display.launch # by default open a ZED  
```



reference:

1. http://wiki.ros.org/zed-ros-wrapper
2. https://github.com/stereolabs/zed-ros-wrapper
