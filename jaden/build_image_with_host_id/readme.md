## This program builds image and changes id automatically.

### Steps

#### 1. Build a image from another image

```shell
$ ./idBuild [New image name] [Original image name]
```

It will generate a new image, detect your host uid and gid and set the ros's uid and gid as your host user.

example:

```shell
$ ./idBuild myimage jaden/ros-vnc-ubuntu:kinetic
```

