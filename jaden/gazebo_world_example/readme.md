## Pairworld

- ``pairworld_gazebo`` is the package for this example. 
- See ``Launch Gazebo world.odp`` for more details.
- Require gazebo_ros: http://gazebosim.org/tutorials?tut=ros_installing




## Steps:

Put ``pairworld_gazebo`` in any ``catkin_work_space\src``. Under your work space:

There are different ways to use this world package:

### Launch world and robot in separate files:

1. Launch the world:

   ```shell
   $ catkin_make
   ```

   ```shell
   $ source [To your setup.bash]
   ```

   ```shell
   $ roslaunch pairworld_gazebo watch_tower_world.launch
   ```

   

2. Add a robot in the world:

   In your launch file, delete the code of your world part.

   ```shell
   $ roslaunch [Your Robot package] [Robot without world].launch
   ```


Notice that “robot package” and “world package” should be in the same work space.

### Launch world and robot at once:

1. Replace the code of the part where you startup your world in your launch file with following:

   ```launch
   <include file="$(find gazebo_ros)/launch/empty_world.launch">
       <arg name="world_name" value="$(find pairworld_gazebo)/worlds/watch_tower_world.world"/>
     </include> 
   ```

   

2. Launch:

   ```shell
   $ catkin_make
   ```

   ```shell
   $ source [To your setup.bash]
   ```

   ```shell
   $ roslaunch [Your Robot package] [Robot with world].launch
   ```



### Open the world with only Gazebo:

If you want to take a look the world without ROS, under ``pairworld_gazebo/worlds`` :

1. Set environment variable:

   ```shell
   $ export GAZEBO_MODEL_PATH="[Full path from root]/pairworld_gazebo/.."
   ```

   

2. Open it:

   ```shell
   $ gazebo ./watch_tower_world.world
   ```

   



