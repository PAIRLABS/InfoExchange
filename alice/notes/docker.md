Docker
===
> last updated on 20181030
# Reference
* [Docker](https://www.docker.com/)
* [Docker Hub](https://hub.docker.com/)
* [Docker -- 從入門到實踐](https://philipzheng.gitbooks.io/docker_practice/)
* [Docker 實戰系列](https://larrylu.blog/step-by-step-dockerize-your-app-ecd8940696f4)
* [A Docker Tutorial for Beginners](https://docker-curriculum.com/)
* [Docker Tutorial](https://www.tutorialspoint.com/docker/)
* [YouTube: Docker Tutorial](https://www.youtube.com/watch?v=pGYAg7TMmp0&list=PLoYCgNOIyGAAzevEST2qm2Xbe3aeLFvLc)
* [Docker Tutorial | Docker Tutorial for Beginners | Docker](https://www.youtube.com/watch?v=VlSW-tztsvM)
* [[done] From Zero to Docker - Tutorial for Beginners](https://www.youtube.com/watch?v=JprTjTViaEA)
# Notes
## Concept
![VM](https://blog.gtwang.org/wp-content/uploads/2017/06/virtual-machine-20170625-1.png)

![Docker](https://blog.gtwang.org/wp-content/uploads/2017/06/docker-container-20170625-1.png)

VM: including codes & libs & Guest OS
Docker: including only codes & libs

Docker can include many containers, which provide the resources necessary.
## Commands [(link)](https://docs.docker.com/engine/reference/commandline/docker/)
- check docker version
``` shell
docker --version
```
- check the instructions
``` shell
docker --help
```
- run images
``` shell
docker run [image_name]
```
*docker will automatically download images for you.*
```shell
docker run -it ubuntu [command]
docker run -it --name [container_name] ubuntu
docker run -it --rm --name [container_name] [image_name]
```
-i: run interactive shell
-t: pseudo terminal
--rm: automatically remove after exit
*Ctrl-P-Q: leave (container is still running)*
```shell
docker run -it -p [our_port]:[container_port] --name [container_name] [image_name]
```
- show the containers
```shell
docker ps
docker ps -a
```
- remove the container
```shell
docker rm -f [CONTAINER_NAMES/ID]
docker rm $(docker ps -a -f status=exited -q)
```

-  attach to containers
```shell
docker attach [container_ID/NAMES]
```
- start a container (even if it is not in docker ps)
```shell
docker start [container_name]
```
- show all the images downloaded
```shell
docker images
```

- remove images
```shell
docker rmi [image_ID/NAMES]
docker rmi -f [image_ID/NAMES]
```

- volumes (link our file system to the container's file system)
```shell
docker run --rm -v [local_path]:[container_path] --name [container_name] [image_name]
```

- Create a new image from a container’s changes [(link)](https://docs.docker.com/engine/reference/commandline/commit/#commit-a-container)
```shell
docker commit [OPTIONS] CONTAINER [REPOSITORY[:TAG]]
```

- Run docker w/ GPU
```shell
nvidia-docker
```

## DockerFile
example
```dockerfile=
# what images going to use to build it
FROM ubuntu
# the port you're going to use
EXPOSE 80 
```
*save as .Dockerfile*

build
```shell
docker build -t [image_name] .
docker build -t [image_name] -f ./.Dockerfile .
```