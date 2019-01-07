#!/bin/bash
# Program:
#       This program builds image and changes id automatically.
#		Assign the image name that you want and it's parent image name.
# History:
# 2019/01/4	Jaden	First release
#PATH=/home/jaden:/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
#export PATH

#Check your currect uid
yourHostUid="$(id -u)"
echo "Your host user uid: ${yourHostUid}"
yourHostGid="$(id -g)"
echo "Your host user gid: ${yourHostGid}"

#Run to generate the container
imageName=${1:-"new-image"}
pImageName=${2:-"jaden/ros-vnc-ubuntu:kinetic"}
echo "Image name is: ${imageName}, Parent image name is: ${pImageName}"
docker run -d --name ${imageName} -it ${pImageName} /bin/bash

#Change this docker's uid and gid
echo "Original container uid is: $(docker exec ${imageName} id -u)"
containerOrigUid="$(docker exec ${imageName} id -u)"
containerOrigGid="$(docker exec ${imageName} id -g)"
echo exit
docker stop ${imageName}
docker rm ${imageName}

docker build -t ${imageName} . --build-arg imageName=${pImageName} --build-arg hostUid=${yourHostUid} --build-arg hostGid=${yourHostGid} --build-arg containerUid=${containerOrigUid} --build-arg containerGid=${containerOrigGid}


#Exit
echo "script end"
exit 0
