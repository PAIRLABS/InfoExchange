## Pairworld

- ``pairworld_gazebo`` is the package for this example. 

- See ``Launch Gazebo world.odp`` for more details.




## Steps:

1. Launch the world:

   Put ``pairworld_gazebo`` in any ``catkin_work_space\src``. Under your work space:

   ```shell
   $ catkin_make
   ```

   ```shell
   $ roslaunch pairworld_gazebo watch_tower_world.launch
   ```

   

2. Add a robot in the world:

   ```shell
   $ roslaunch [Your Robot package] [Robot without world].launch
   ```

   


Notice that “robot package” and “world package” should be in the same work space.