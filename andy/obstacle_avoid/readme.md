# readme

---
## instruction
此份code是避障演算法，並且可以透過修改launch檔的參數來套用到不同的機器人上。

演算法透過接收到攝影機的depth image知道周圍障礙物的距離，並且在避障的同時做到追蹤目標物體。
## issue
因為input只有目標物體的座標還有周圍障礙物的距離，所以沒辦法作到路徑規劃，所以當目標在障礙物的後方或是在一個房間內，這個演算法沒辦法規劃路徑並走到目標位置。
## impementation
執行檔位於`src/robot_follow/depth_turtlebot3_world.launch`

如果需要改深度攝影機的解析度，必須去urdf內修改，並且也要在`follow.cpp`內引用的`turtlebot3_gazebo/turtlebot3_depth.h`中做修改
（這邊忘記做直接從launch中調用參數來調整解析度了，之後可以補上）

### follow.cpp

- `void Turtlebot3Drive::depthScanMsgCallBack(const sensor_msgs::Image::ConstPtr &msg)`
這邊是把攝影機收到的depth image 轉換成一個2D array裡面存每個像素點偵測到的距離為多少公尺
- `bool Turtlebot3Drive::controlLoop()`
這邊是避障的控制函式，我設計的邏輯是取深度畫面中最中間的橫線並掃過去那條橫線的深度來判斷有沒有障礙物，並且先判斷哪個障礙物離機器人最近並且往另外一個方向避開，如果障礙物距離機器人夠遠，那就開始往目標的物體移動，追蹤目標物體的tf是使用下列的函式，詳細的使用方法可以去找doc來看
```=C++
listener.waitForTransform(base_footprint, target_base_frame, ros::Time(0), ros::Duration(10));
listener.lookupTransform(base_footprint, target_base_frame,	ros::Time(0), transform);
```
### depth_turtlebot3_world.launch
有一個地方比較需要注意的是
```=XML
<node name="my_stp" pkg="tf" type="static_transform_publisher" args="0 0 0 0 0 0 map ns1/odom 20"/>
<node name="target_stp" pkg="tf" type="static_transform_publisher" args="0 0 0 0 0 0 map target/odom 20"/>
```
記得要把兩個機器人的tf連接在一起，不然lookuptransform會找不到tf
