# docker image from tensorflow (confirm the usage of GPU)

> images source : https://hub.docker.com/r/tensorflow/tensorflow/
>
> --  all of tensorflow's images are based on ubuntu 16.04



> step 1 : build image and run container
>
> step 2 : check if jupyter notebook work
>
> step 3 :make sure the usage of GPU



## step 1 : build image and run container

##### 1.1) git clone my repository to your work space

```
$ git clone https://github.com/Nano1201/InfoExchange.git
$ cd InfoExchange/nano/build_tensor_image/
```

##### 1.2) execute shell script in `InfoExchange/nano/build_tensor_image/` to build image

```
## cd InfoExchange/nano/build_tensor_image/ 
$ chmod +x ten_image_userbuild
$ ./ten_image_userbuild
```

##### 1.3 ) check out your image

```
$ docker images
REPOSITORY             TAG               IMAGE ID        CREATED             SIZE
[your_user_name]/tensorflow  1.11.0-devel-gpu-py3  902b75c0d8d7  41 hours ago 3.85GB
```

##### 1.4 ) docker run container

```
$ nvidia-docker run --rm -it -p 8888:8888 \
--volume [your_workspace_host]:[your_workspace_container] \
--hostname [your_host_name]  --name [your_container-name] \
[your_user_name]/tensorflow:1.11.0-devel-gpu-py3 /bin/bash
```

◆ add ` -p 8888:8888` for the **usage of jupyter notebook**

◆ add `--volume [your_workspace_host]:[your_workspace_container]` to **mount local directory to conatiner**

◆ add `--hostname [your_host_name] ` to **replace your hostname** from random string

◆ add `--name [your_container-name] ` to **indicate that who you are**

◆ example :

```
$ nvidia-docker run --rm -it -p 8888:8888 \
--volume /home/nano/test:/home/nano/test \
--hostname nano-e3-gpu  --name nano-tensorflow-gpu \
nano/tensorflow:1.11.0-devel-gpu-py3 /bin/bash
```



##### 1.5) password in container

◆ default **user name** and **password in image** are both assigned by **your user name in host**.

```
# in host terminal, execute whoami to check it out.
$ whoami
```

```
# in container terminal, execute passwd to change your password if you want
$ passwd
```



## step 2 : check if jupyter notebook work

```
#in container
$ jupyter notebook --ip=0.0.0.0 --port=8888
```

◆ Copy/paste the URL you get into your browser, remember to modify IP in the beginning of  of URI.



## step 3 :  make sure the usage of GPU

3.1 ) execute nvidia-smi

```
## in container

$ nvidia-smi
Sun Jan 13 02:54:35 2019
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 396.54                 Driver Version: 396.54                    |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|===============================+======================+======================|
|   0  GeForce GTX 108...  Off  | 00000000:01:00.0 Off |                  N/A |
|  0%   42C    P8    17W / 250W |    716MiB / 11178MiB |      0%      Default |
+-------------------------------+----------------------+----------------------+
|   1  GeForce GTX 108...  Off  | 00000000:02:00.0 Off |                  N/A |
|  1%   45C    P8    19W / 250W |      2MiB / 11178MiB |      0%      Default |
+-------------------------------+----------------------+----------------------+

+-----------------------------------------------------------------------------+
| Processes:                                                       GPU Memory |
|  GPU       PID   Type   Process name                             Usage      |
|=============================================================================|
+-----------------------------------------------------------------------------+

```



3.2 ) try a simple matrix operation with GPU

```
#in comtainer
$ jupyter notebook --ip=0.0.0.0 --port=8888
```

◆ execute `gpu_tester.ipynb` in `InfoExchange/nano/build_tensor_image/gpu_test_code`



------

#### Reference:

1. docker hub(Tensorflow) : https://hub.docker.com/r/tensorflow/tensorflow/
2. useradd : http://linux.vbird.org/linux_basic/0410accountmanager.php#useradd

