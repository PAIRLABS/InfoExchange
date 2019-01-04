#!/bin/bash
# Program:
#       This program builds container and changes id automatically.
#		Assign the container name that you want and it's image name.
# History:
# 2019/01/4	Jaden	First release
#PATH=/home/jaden:/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
#export PATH

#Check your currect uid
yourHostUserId="$(id | cut -c5-8)"
echo "Your host user ID: ${yourHostUserId}"

#Run to generate the container
dockerName=${1:-"new-container"}
docker run -d --name ${1} -it ${2} /bin/bash

#Change this docker's uid and gid
echo "Original container ID is: $(docker exec ${1} id | cut -c5-8)"
containerOrigUserId="$(docker exec ${1} id | cut -c5-8)"

docker exec -u 0 ${1} usermod -u ${yourHostUserId} ros
docker exec -u 0 ${1} groupmod -g ${yourHostUserId} ros
echo "Container ID has been changed to: $(docker exec ${1} id | cut -c5-8)"
docker start ${1}
docker exec -u 0 ${1} find / -group ${containerOrigUserId} -exec chgrp -h ros {} \;
docker exec -u 0 ${1} find / -user ${containerOrigUserId} -exec chown -h ros {} \;

#Exit
echo "script end"
exit 0
