# This Dockerfile is used to build an ROS + VNC + Tensorflow image based on Ubuntu 18.04
FROM tensorflow/tensorflow:1.11.0-devel-gpu-py3

LABEL maintainer "Edward Wang"
MAINTAINER Edward Wang "https://github.com/edwardwang2nctu"
ENV REFRESHED_AT 2019-01-11

# Install sudo
RUN apt-get update && \
    apt-get install -y sudo \
    xterm \
    curl \
    vim \
    git \
    tmux \
    lsb-release
RUN pip3 install jupyter-tensorboard

CMD ["/bin/bash"]