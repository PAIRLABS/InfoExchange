FROM pairlabs/tensorflow:1.11.0-devel-gpu-py3
MAINTAINER Docker Nano <neverforever1201@gmail.com>
ARG username
ARG hostGid
ARG hostUid
RUN echo "arguments: $hostGid $hostUid $username"
RUN groupadd -g $hostGid $username 
RUN useradd -m -s /bin/bash -u $hostUid -g $hostGid $username
RUN usermod -aG sudo $username
USER $username 
WORKDIR /home/$username

