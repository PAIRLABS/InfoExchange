# PAIRBOT
> update at 2019/1/3

## implementation

1.`$catkin_make`
2.`$source devel/setup.sh`
3.`$roslaunch robot_follow pairbot_chasing.launch`

## features

use the pairbot and turtlebot with the depth camera "zed" to simulate in the gazebo.
- there are some detail in my another github folder `obstacle_avoid` which I put in this pairbot.
## issues and fix

1. In `pb_control`, the `move_base.py` have a problem with the TF name which is no compatible with the `robot_follow`, so I modify it.
2. In `pb_contol/launch/pb_control.launch`, this launch file has already launch the node, `robot_state_publisher`, but I need to launch the node at my launch file `pairbot_chaing.launch`. I delete this command here.
3. The gazebo world I create in the launch file has some problem when I create the map. When I edit the map in the gazebo, the map file cannot use directly because the launch file cannot read the state I modify in the gazebo, so I change the code in the map file. I put the state's poses to be the original poses in this map file and delete the state status in it. Finally, it works. Besides, be careful about the model in this map should be readable by gazebo. The model's directory is at `pairbot/src/pb_gazebo/models`
4. In the gazebo8 and gazebo7 there have many compatible issues between it. Maybe there still have some issues not solved. 

I found that the depth camera in gazebo 8 wouldn't pass `nan` for the distance is too far, but the gazebo7 did. so I change the urdf setting to let the camera distant range to be 0.05m ~ 20m. It can see far in the gazebo 7, but I think it may have some problem with it.
5. The gazebo simulation still too slow, I haven't found the solution yet.
