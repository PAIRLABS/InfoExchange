## This program builds image and changes id automatically.

### Steps

#### 1. Build a image from another image

```shell
$ sh idBuild.sh [image name] [parents image name]
```

It will generate a new image, detect your host uid and set the ros's uid as your host user.

example:

```shell
$ sh idBuild.sh myimage jaden/ros-vnc-ubuntu:kinetic
```

Note that if you didn't assign any parameter then default image name will be "new-image" and parents image name is jaden/ros-vnc-ubuntu:kinetic