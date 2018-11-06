README
===
###### tags: `README`
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
