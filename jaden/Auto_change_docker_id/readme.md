## This program builds container and changes id automatically.

### Steps

#### 1. Run a container

```shell
$ sh idrun.sh [container name] [image name]
```

It will generate a new container, detect your host uid and set the ros's uid as your host user.

example:

```shell
$ sh idrun.sh mycontainer jaden/ros-vnc-ubuntu:kinetic
```

Note that if you didn't assign container name then default name will be "new-container"