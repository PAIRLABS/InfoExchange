## Zed model in gazebo (how to insert and visualize in RVIZ)

1. go to upper folder of zed_sim

   ```
   ~/zed_model $ ls
   Zed_sim
   ```

2. copy to gazebo's model folder

   > if you can not find `~/.gazebo/`, run your gazebo first, and you can find the folder.   

   ```
   ~/zed_model$ cp -r zed_sim/ ~/.gazebo/model/
   ```

3. find model in gazebo insert

   ```
   $ roscore
   $ roslaunch [any_package][and_launch_in_package]
   ```

   1. find `zed_sim` in `insert`
   ![image](InfoExchange/nano/zed_model/log_image/image 1.PNG)

   2. place to anyplace as you wish
   ![image](InfoExchange/nano/zed_model/log_image/image 2.PNG)

4. visualize depth camera and RGB camera in RVIZ

   ```
   $ rosrun rviz rviz 
   ```

   1. add depth image (camera)
   ![image](InfoExchange/nano/zed_model/log_image/image 3.PNG)
   ![image](InfoExchange/nano/zed_model/log_image/image 4.PNG)

   2. add right and left RGB image (camera)
   ![image](InfoExchange/nano/zed_model/log_image/image 5.PNG)
   ![image](InfoExchange/nano/zed_model/log_image/image 6.PNG)
