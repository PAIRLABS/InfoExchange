turtlebot3
===
![](https://i.imgur.com/n6nHZeh.png)



## 執行流程
1. 執行兩個launch檔 fake 跟 teleop
2. **teleop** 主要只有將terminal的input publish到 `/cmd_vel`這個topic上，然後就沒事了
3. fake執行後開了三個node:
    **fake_node**
    **robot_state_pulisher**
    rviz
4. 並且引入了 description.launch.xml
    裡面導入了整個機器人的urdf.xacro，也就是機器人的架構設計與參數
### launch檔介紹
---
```=xml
<launch>
  <arg name="model" default="$(env TURTLEBOT3_MODEL)" doc="model type [burger, waffle, waffle_pi]"/>

  <param name="tb3_model" value="$(arg model)"/>

  <include file="$(find turtlebot3_bringup)/launch/includes/description.launch.xml">
    <arg name="model" value="$(arg model)" />
  </include>

  <node pkg="turtlebot3_fake" type="turtlebot3_fake_node" name="turtlebot3_fake_node" output="screen" />

  <node pkg="robot_state_publisher" type="robot_state_publisher" name="robot_state_publisher" output="screen">
    <param name="publish_frequency" type="double" value="50.0" />
  </node>

  <node name="rviz" pkg="rviz" type="rviz" args="-d $(find turtlebot3_fake)/rviz/turtlebot3_fake.rviz"/>
</launch>
```
- param可以傳入node中，node端可以用`getparam`取得參數
- arg只有在launch檔中傳遞參數，對node並無影響
- `<node />`就是直接執行node，`output=screen`就是會將log產生到terminal上
### fake_node
---
- 接收parameter server的parameter(在程式碼中parameter server並沒有這些參數，所以使用的是default，使用者可以自己額外加入等等)
    - joint name
    - odom frame
    - base frame

- 計算從`cmd_vel`來的指令(velocity & angular velocity)
- 計算所有的位置與速度資訊存到odom中並publish
- 更新joints的資訊並publish `joint_state`
- publish `/tf`
  下列代碼是傳至tf的資訊
```=C++
void Turtlebot3Fake::updateTF(geometry_msgs::TransformStamped& odom_tf)
{
  odom_tf.header = odom_.header;
  odom_tf.child_frame_id = odom_.child_frame_id;
  odom_tf.transform.translation.x = odom_.pose.pose.position.x;
  odom_tf.transform.translation.y = odom_.pose.pose.position.y;
  odom_tf.transform.translation.z = odom_.pose.pose.position.z;
  odom_tf.transform.rotation = odom_.pose.pose.orientation;
}
```
### robot state publisher
---
除了fake node 本身會傳tf，robot state publisher也會
不過這邊是透過接收joint_state並轉換成tf的格式傳送，而joint_state裡面的資訊如下
```=C++
void Turtlebot3Fake::updateJoint(void)
{
  joint_states_.position[LEFT]  = last_position_[LEFT];
  joint_states_.position[RIGHT] = last_position_[RIGHT];
  joint_states_.velocity[LEFT]  = last_velocity_[LEFT];
  joint_states_.velocity[RIGHT] = last_velocity_[RIGHT];
}
```
所以主要是傳左右輪子的速度以及位置
### TF
---
![](https://i.imgur.com/A51fEZR.png)
從rqt讀到的資訊，主要是收到了wheel_left_link與wheel_right_link的tf
是從robot_state_publisher收到的
還會從fake node那邊收到odom的tf
- odom_tf是指整個機器人架構的root，掌管整個機器人的移動狀態
- 兩個node同時advertise同一個topic (`/tf`)
![](https://i.imgur.com/oXeny5O.png)

### URDF.xacro
---
定義機器人的架構還有link name與joint name，並可以直接呼叫stl作為零件的3D模型，而整個機器人是一個樹狀結構，就是有parent & child的形成方式。
底下的網站介紹得很詳細
https://blog.csdn.net/sunbibei/article/details/52297524



