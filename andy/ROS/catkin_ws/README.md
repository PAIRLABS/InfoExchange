Instruction
===
## installation
---
the ROS melodic is install in **Ubuntu 18.04 LTS**
**you should create a catkin workspace to put these source in it.**

## beginner tutorial
---
listener and talker is a simple demo for how to publish and subscribe a custom **message**.
listener get the input number and subscriber output with the sum.

add_two_ints_client and add_two_ints_server is another way to sum with the **service**

## opencvtest
---
 you should install uab_cam package before runnung this package
> sudo apt-get install ros-melodic-usb_cam

the package is about get the image from the webcam and put a data tag on it, then publish the image with the tag.
the subscriber get the message and show the image on screen, and show the tag on terminal simultaneously.

use
> rosrun usb_cam usb_cam_node

to get the image from webcam
