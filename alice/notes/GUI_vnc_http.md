GUI interface w/ VNC or HTTP protocol
===
> last updated on 20181026
use ```henry2423/ros-vnc-ubuntu``` images do run a container
```shell
docker run -it -p [outer_web_port]:[inner_web_port] -p [outer_vnc_port]:[inner_vnc_port] henry2423/ros-vnc-ubuntu:kinetic bash
```
inner_web_port: 5901
outer_web_port: 6901
## VNC
- subnet
1. use `ifconfig` to find out subnet ip
2. use ==VNC Viewer / Remmina Remote Desktop Cliet== to connect to `172.17.0.6:5901`
3. password: `vncpasswd`

- non-subnet
1. use ==VNC Viewer / Remmina Remote Desktop Cliet== connect to `e1.pairlabs.ai:[outer_vnc_port]`
2. password: `vncpasswd`

## HTTP
- subnet
1. use any ==Web Server== to connect to `http://172.17.0.6:6901/?password=vncpasswd`

- non-subnet
1. use any ==Web Server== to connect to `http://e1.pairlabs.ai:[outer_web_port]/?password=vncpasswd`
