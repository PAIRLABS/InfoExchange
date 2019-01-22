## This is an example for introduce how tf works.

- #### ``catkin_ws`` is the work space for this example.

- #### Under ``catkin_ws/src/tf_example/scripts/`` :

  #### ``conv.py`` & ``conv5field.py`` can convert files exported after executed.

  #### ``spinTurtle`` & `` spinTurtle_box`` are shell scripts for execute example.



## Steps:

1. Copy this work space to your local, under work space:

   ```shell
   $ source ./devel/setup.bash
   ```

   ```shell
   $ catkin_make
   ```

   to build the package.

2. ```shell
   $ roslaunch learning_tf start_demo.launch
   ```

   to launch example 1.

   ```shell
   $ roslaunch learning_tf start_demo_box.launch
   ```

   to launch example 2.

3. In another terminal, change directory to ``catkin_ws/src/tf_example/scripts/`` :

   run 

   ```shell
   $ ./spinTurtle
   ```

    or 

   ```shell
   $ ./spinTurtle_box
   ```

    for execute the example 1 or 2.

4. To use ``conv.py`` or ``conv5field.py``: After executed the example 1, change directory to ``catkin_ws/src/tf_example/scripts/``, you will see ``spinExport.csv`` and for example 2 it will be ``spinExport_box.csv`` .

   To convert ``spinExport.csv``:

   ```
   $ python2 conv.py > [file name].csv 
   ```

   To convert ``spinExport_box.csv``:

   ```
   $ python2 conv5field.py > [file name].csv 
   ```

   

#### Please see ``ROS_tf_introduction.odp`` for more details.

