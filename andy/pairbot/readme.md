# PAIRBOT
> update at 2019/1/
###### tags: `PAIR`
## implementation
### 完整板
`$roslaunch robot_follow pairbot_chasing.launch`
### 原始的程式不加避障功能的
`$roslaunch pb_gazebo empty_world.launch`

目前應該只有這兩個launch是可以正常運作的
## features

use the pairbot and turtlebot with the depth camera "zed" to simulate in the gazebo.
- there are some detail in my another github folder `obstacle_avoid` which I put in this pairbot.
## issues

1. In `pb_control`, the `move_base.py` have a problem with the TF name which is no compatible with the `robot_follow`, so I modify it.
2. In `pb_contol/launch/pb_control.launch`, this launch file has already launch the node, `robot_state_publisher`, but I need to launch the node at my launch file `pairbot_chaing.launch`. I delete this command here.
3. The gazebo world I create in the launch file has some problem when I create the map. When I edit the map in the gazebo, the map file cannot use directly because the launch file cannot read the state I modify in the gazebo, so I change the code in the map file. I put the state's poses to be the original poses in this map file and delete the state status in it. Finally, it works. Besides, be careful about the model in this map should be readable by gazebo. The model's directory is at `pairbot/src/pb_gazebo/models`
4. In the gazebo8 and gazebo7 there have many compatible issues between it. Maybe there still have some issues not solved. 
I found that the depth camera in gazebo 8 wouldn't pass `nan` for the distance is too far, but the gazebo7 did. so I change the urdf setting to let the camera distant range to be 0.05m ~ 20m. It can see far in the gazebo 7, but I think it may have some problem with it.
5. The gazebo simulation still too slow, I haven't found the solution yet.

6. maybe you should manual to import the map model when you launch the pairbot_chasing.
The model is put in the `pairbot/pb_gazebo/models`

## works
- 將zed camera裝到機器人上
在`pairbot/pb_gazebo/pb_descroption/urdf/pairbot.xarco`中可以看到將zed的urdf直接incude並且將link設定完成
### pairbot_chasing.launch
- 主要需要解決的是namespace的問題，原始的code因為將namesapce寫死，導致如果需要創造group，就會產生問題
- `robot_state_publisher`
放在`pb_control.launch`中，之前的turtlebot3也有出現過，非常重要，主要的用途有兩個，第一個是讓urdf中每個link的parent跟child的tf可以接起來，或是ROS中如果有node publish `joint_state`會將裡面的內容轉換成tf，如果`robot_state_publisher`沒有成功執行，在rqt中可以看到tf tree是不完整的
所以我主要需要解決的就是讓整個code的namespace可以統一好讓`robot_state_publisher`可以正確的抓到對的資訊。


