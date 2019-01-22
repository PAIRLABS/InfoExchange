#TODO

====

- There are still some problems in the namespace.
the current version we need to create the `robot_state_publisher` at the outside's launch file(`empty_world.launch` and `pairbot_chasing.launch`)
```
   <!--robot_state_publisher-->
   <param name="tf_prefix" value="pairbot"/>
   <node name="robot_state_publisher" pkg="robot_state_publisher" type="robot_state_publisher"
   respawn="false" output="screen">
   <param name="publish_frequency" type="double" value="50.0" />
   </node>

```
the node need to pass the parameter *tf_prefix* to set the namespace.
We want to just put the parameter in this launch file and put the node in `pb_control.launch`, which is include in this launch file. However, there is a problem. In `pb_control.launch`, all the topic in `control_spawner` is fix, and all the python scripts in pb_control is subscribe these fix topics, so we need to fix it in future work.


- note the change in this file
