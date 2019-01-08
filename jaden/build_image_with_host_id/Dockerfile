# This Dockerfile is used to build an ROS + VNC image based on Ubuntu 16.04
ARG imageName
FROM $imageName

LABEL maintainer "Jaden Lin"
MAINTAINER Jaden Lin "https://github.com/jadenlin0106"
ENV REFRESHED_AT 2019-01-07

#Configure Arg
ARG userName="ros"

#Host uid & gid should be given from --build-arg 
ARG hostUid=""
ARG hostGid=""
RUN echo "Host uid is set to $hostUid"
RUN echo "Host gid is set to $hostGid"

# RUN usermod
USER root
RUN export containerUid=`id $userName -u` && \
	export containerGid=`id $userName -g` && \
	echo "Changing original container uid $containerUid to $hostUid, and gid $containerGid to $hostGid" && \
	usermod -u $hostUid  $userName && \
	groupmod -g $hostGid $userName && \
	find / -ignore_readdir_race -path /proc -prune -o -user $containerUid -exec chown -h $userName {} \; && \
	find / -ignore_readdir_race -path /proc -prune -o -group $containerGid -exec chgrp -h $userName {} \;
USER $userName



ENTRYPOINT ["/dockerstartup/vnc_startup.sh"]
CMD ["--wait"]
