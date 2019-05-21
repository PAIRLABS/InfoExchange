FROM tensorflow/tensorflow:1.11.0-devel-gpu-py3
MAINTAINER Docker Nano <neverforever1201@gmail.com>
ARG username
ARG hostGid
ARG hostUid
RUN echo "arguments: $hostGid $hostUid $username"
RUN groupadd -g $hostGid $username 
RUN useradd -m -u $hostUid -g $hostGid $username
RUN usermod -aG sudo $username
RUN echo "$username:$username" | chpasswd
USER $username 
WORKDIR /home/$username

