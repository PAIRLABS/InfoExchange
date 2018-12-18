ROS
===
> last updated on 20181031
> 
# Reference
- [ROS Wiki](http://wiki.ros.org/ROS)
- [大頭怪的異想空間](https://charlyhuangrostutorial.wordpress.com)
- [ROS系列文整理 (ROS Tutorials)](https://pojenlai.wordpress.com/2012/12/14/ros%E6%95%99%E5%AD%B8%E7%B3%BB%E5%88%97%E6%96%87%E6%95%B4%E7%90%86/)
- [ROS tutorial for Beginners](https://www.youtube.com/playlist?list=PLK0b4e05LnzZWg_7QrIQWyvSPX2WN2ncc)
- [ROS courses](https://youtu.be/0BxVPCInS3M)
- [ROS basics](https://www.youtube.com/playlist?list=PLk51HrKSBQ8-jTgD0qgRp1vmQeVSJ5SQC)
- [ROS video tutorial](https://youtu.be/9U6GDonGFHw)
- [ROS tutorials](https://www.youtube.com/playlist?list=PLH-vcjgGSri0sipCweLukjPvJuoUpGchJ)
# Notes
## Concept
ROS is an operating system to run Robot, just like Windows to different brands of computers. ROS integrates all the robot-related resources.
### Filesystem Level
- **Package:** ***The main unit*** in ROS. A package may contain ROS runtime processes (nodes), a ROS-dependent library, datasets, configuration files, or anything else that is usefully organized together.
- **Metapackages:** Metapackages are specialized Packages which only serve to represent a group of related other packages.
- **Package Manifests:** Manifests (package.xml) provide metadata about a package, including its name, version, description, license information, dependencies, and other meta information like exported packages. 
- **Repositories:** ***A collection of packages which share a common VCS system.*** Packages which share a VCS share the same version and can be released together using the ***catkin release*** automation tool bloom. Often these repositories will map to converted rosbuild Stacks. Repositories can also contain only one package.
- **Message (msg) types:** Message descriptions, stored in my_package/msg/MyMessageType.msg, define the data structures for messages sent in ROS.
- **Service (srv) types:** Service descriptions, stored in my_package/srv/MyServiceType.srv, define the request and response data structures for services in ROS.

### ROS Computation Graph Level
![nodes & topic](http://ros.org/images/wiki/ROS_basic_concepts.png)
- **Nodes:** ***A control unit***. Nodes are processes that perform computation. ROS is designed to be modular at a fine-grained scale; ***a robot control system usually comprises many nodes***. For example, one node controls a laser range-finder, one node controls the wheel motors, one node performs localization, one node performs path planning, one Node provides a graphical view of the system, and so on. A ROS node is written with the use of a ROS client library, such as roscpp or rospy.
- **Master:** ***Master manages the communication and interaction b/w nodes.*** The ROS Master provides name ***registration and lookup*** to the rest of the Computation Graph. Without the Master, nodes would not be able to ***find each other, exchange messages, or invoke services***.
- **Parameter Server:** The Parameter Server allows ***data to be stored by key in a central location***. It is currently part of the Master.
- **Messages:** ***Nodes communicate with each other by passing messages***. A message is simply a data structure, comprising typed fields. Standard primitive types (integer, floating point, boolean, etc.) are supported, as are arrays of primitive types. Messages can include arbitrarily nested structures and arrays (much like C structs).
- **Topics:** Messages are routed ***via a transport system with publish / subscribe*** semantics. A node sends out a message by publishing it to a given topic. ***The topic is a name that is used to identify the content of the message***. A node that is ***interested in a certain kind of data will subscribe to the appropriate topic***. There may be ***multiple concurrent publishers and subscribers for a single topic***, and ***a single node may publish and/or subscribe to multiple topics***. In general, publishers and subscribers are not aware of each others' existence. The idea is to decouple the production of information from its consumption. Logically, one can think of a topic as a strongly typed message bus. Each bus has a name, and anyone can connect to the bus to send or receive messages as long as they are the right type.
- **Services:** The publish / subscribe model is a very flexible communication paradigm, but its many-to-many, one-way transport is not appropriate for request / reply interactions, which are often required in a distributed system. ***Request / reply is done via services***, which are defined by a pair of message structures: one for the request and one for the reply. A providing node offers a service under a name and a client uses the service by sending the request message and awaiting the reply. ROS client libraries generally present this interaction to the programmer as if it were a remote procedure call.
- ***Bags:*** ***Bags are a format for saving and playing back ROS message data.*** Bags are an important mechanism for storing data, such as sensor data, that can be difficult to collect but is necessary for developing and testing algorithms.
