Change user id in docker container to get permission from local
===
> last updated on 20181031
> .
> use `henry2423/ros-vnc-ubuntu:kinetic` as an example
# Steps
**1. check your user id and group id from local**
```shell
$ id [user-name]
```
you will get sth like below
```uid=1002(alice) gid=1002(alice)```

**2. run docker**
```shell
docker run -it .........
```
**3. change the docker's user id and group id**
assign your own group id and user id to ```ros``` (or the other user name)
```shell
$ sudo su - # switch to root to assign new UID
$ id ros # remember the original id for later use on files
$ usermod -u [your-user-id] ros
$ groupmod -g [your-group-id] ros
$ id ros # check you've successfully changed
```
**4. change all the files' user ids and group ids**
```shell
$ find / -group [original-group-id] -exec chgrp -h ros {} \;
$ find / -user [original-user-id] -exec chown -h ros {} \;
```
**5. commit docker to image**
exit or detach the docker, and then commit it
```shell
docker commit [container-name] [your-own-image-name]
```
**6. finish! run your own docker image**
```shell
$ docker run -it --name [container-name] -p [outer-port]:[inner-port] -v [local-path]:[docker-path] [image-name] [command]
```
for example
```docker run -it --name alice-ros -p 50000:5901 -p 60000:6901 -v ~/alice-ros:/home/ros/alice-ros alice/ros-vnc-ubuntu:kinetic bash```