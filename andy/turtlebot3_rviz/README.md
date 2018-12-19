README
===
###### tags: `README`

### simple turtlebot3 in rviz
1. run `catkin_make` in a new folder to create a work space
2. put `catkin_make/src` in it and run `catkin_make` again
3. run
```
$ export TURTLEBOT3_MODEL=$burger
$ roslaunch turtlebot3_fake turtlebot3_fake.launch
```
4. run
```
$ export TURTLEBOT3_MODEL=burger
$ roslaunch turtlebot3_teleop turtlebot3_teleop_key.launch
```
you can refer it in detail:
>http://emanual.robotis.com/docs/en/platform/turtlebot3/simulation/
5. There are some notes I made in  `catkin_tb3/turtlebot3_note.md`

### two robots in gazebo with rviz
1. run 
```
$ export TURTLEBOT3_MODEL=$burger
$ roslaunch turtlebot3_gazebo turtlebot3_empty_world_multi.launch
$ roslaunch turtlebot3_fake turtlebot3_gazebo_multi.launch
```
2. then it will display the same scheme in gazebo and rviz
3. there is note in `catkin_tb3/turtlebot3_gazebo_note.md`
### avoiding obstacles and chasing target
1.run
```
$ export TURTLEBOT3_MODEL=$burger
$roslaunch turtlebot3_gazebo depth_turtlebot3_world.launch
```
2.the robot will auto chase another robot and you can drag the target robot by youself
