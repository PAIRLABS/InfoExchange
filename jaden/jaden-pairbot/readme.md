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

