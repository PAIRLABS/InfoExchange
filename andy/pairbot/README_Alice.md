README.md
===
> last updated on 20181201
> By Alice
## Environment
using `ros-kinetic` version
install `ros-control` and `ros-controller` packages
```bash
sudo apt-get install ros-kinetic-ros-control ros-kinetic-ros-controllers
```
## Catkin_make and Source
```bash
catkin_make && source devel/setup.bash
```
## Launch
```bash
roslaunch pb_gazebo empty_world.launch
roslaunch pb_control pb_control.launch
```

## Control using rostopic
#### move
```bash
rostopic pub -1 /turtle1/cmd_vel geometry_msgs/Twist  '{linear:  {x: 0.0, y: 0.0, z: 0.0}, angular: {x: 0.0,y: 0.0,z: 0.0}}'

```

#### flipper
```bash
 rostopic pub /pairbot/joint_right_front_flipper_controller/command std_msgs/Float64 '1.5
```

#### flipper wheels
```bash
rostopic pub /pairbot/joint_right_front_flipper_wheel_controller/command std_msgs/Float64 '2'
```

## Gazebo client
