## Pairbot

- ``jaden-pairbot`` is the work space for this example.

- Under ``jaden-pairbot/src/pairbot/pb_description/urdf/`` :

  ``pairbot.xacro``  is pairbot's main urdf file.

- Under ``jaden-pairbot/src/pairbot/pb_gazebo/launch/`` :

  Pairbot is called in ``empty_world.launch`` .

- Under ``jaden-pairbot/src/pairbot/pb_control/launch/`` :

  ``pb_control.launch`` runs a lot of controllers and it is included by ``empty_world.launch`` 

- Main control functions are in ``jaden-pairbot/src/pairbot/pb_control/scripts/`` 



## Steps:

1. Copy this work space to your local, under work space:

   ```shell
   $ catkin_make
   ```

   ```shell
   $ source ./devel/setup.bash
   ```

   to build the package.

2. 

   1. Empty world:

      ```shell
      $ roslaunch pb_gazebo empty_world.launch
      ```

      to launch an empty world and you will see Pairbot in there.

      In another terminal, under ``jaden-pairbot/src/pairbot/pb_teleop/scripts`` run:

      ```shell
      $ ./pb_teleop.py
      ```

      to execute the  tele-operation program.

      

   2. Tracking another robot:

      ```shell
      $ roslaunch robot_follow pairbot_chasing.launch
      ```

      You will see Pairbot tracks a small turtlebot.



## Steps for stair climbing (Up stairs):

1. ``apt-get`` install workbench packages:

   ```shell
   $ sudo apt-get install ros-kinetic-dynamixel-workbench
   $ sudo apt-get install ros-kinetic-dynamixel-workbench-msgs
   ```

2. Copy this work space to your local, under work space:

   ```shell
   $ catkin_make
   ```

   ```shell
   $ source ./devel/setup.bash
   ```

   to build the package.

3. Launch the MIRC of NCTU world:

   ```shell
   $ roslaunch pb_gazebo pb_world.launch
   ```

4. Launch stair detecting program in another terminal:

   ```shell
   $ source ./devel/setup.bash
   $ roslaunch stair_climbing stair_mark.launch
   ```

5. Move Pairbot to the position as picture with pb_teleop, in another terminal:

   ```shell
   $ ./src/pairbot/pb_teleop/scripts/pb_teleop.py
   ```

   or you can just modify pairbot's pose in Gazebo GUI.

   ![](/home/jaden/Pictures/up_stairs_pose.png)

   Notice the pose: x, y, z, on the left side.

6. Turn on the teleop for subscribes in another terminal:

   ```shell
   $ ./src/pairbot/pb_teleop/scripts/pb_teleop_sub.py
   ```

7. Launch the up stairs main program, in another terminal:

   ```shell
   $ roslaunch stair_climbing up_stair.launch
   ```

   You will see the Pairbot is climbing the stairs.

## Steps for stair climbing (Down stairs):

1. ``apt-get`` install workbench packages:

   ```shell
   $ sudo apt-get install ros-kinetic-dynamixel-workbench
   $ sudo apt-get install ros-kinetic-dynamixel-workbench-msgs
   ```

2. Copy this work space to your local, under work space:

   ```shell
   $ catkin_make
   ```

   ```shell
   $ source ./devel/setup.bash
   ```

   to build the package.

3. Launch the MIRC of NCTU world:

   ```shell
   $ roslaunch pb_gazebo pb_world.launch
   ```

4. Launch stair detecting program in another terminal:

   ```shell
   $ source ./devel/setup.bash
   $ roslaunch stair_climbing stair_mark.launch
   ```

5. Move Pairbot to the position as picture with pb_teleop, in another terminal:

   ```shell
   $ ./src/pairbot/pb_teleop/scripts/pb_teleop.py
   ```

   or you can just modify pairbot's pose in Gazebo GUI.

   ![](/home/jaden/Pictures/down_stairs_pose.png)

   Notice the pose: x, y, z, on the left side.

6. Turn on the teleop for subscribes in another terminal:

   ```shell
   $ ./src/pairbot/pb_teleop/scripts/pb_teleop_sub.py
   ```

7. Launch the up stairs main program, in another terminal:

   ```shell
   $ roslaunch stair_climbing down_stair.launch
   ```

   You will see the Pairbot is going down stairs.

