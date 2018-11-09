Ros Tutorial
===
> last updated on 20181109
> 
### Create a ROS Workspace
```shell
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/
catkin_make
```
It will create a CMakeLists.txt link in your 'src' folder. Additionally, if you look in your current directory you should now have a 'build' and 'devel' folder. Inside the 'devel' folder you can see that there are now several setup.*sh files. Sourcing any of these files will overlay this workspace on top of your environment. 
