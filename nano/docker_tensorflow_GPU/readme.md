# docker image from tensorflow (confirm the usage of GPU)

> images source : https://hub.docker.com/r/tensorflow/tensorflow/
>
> --  all of tensorflow's images are based on ubuntu 16.04



> step 1 : nvidia-docker run container from tensorflow
>
> step 2 : make sure the usage of GPU
>
> step 3 : jupyter notebook



## step 1 : nvidia-docker run container from tensorflow

```
nvidia-docker run --rm -it -p 8888:8888 --hostname [your_host_name]  --name [your_container-name] tensorflow/tensorflow:latest-gpu /bin/bash
```



## step 2 : make sure the usage of GPU

### 1. execute `nvidia-smi`

```
## in container

root@nano-e3-gpu:/notebooks# nvidia-smi
Tue Jan  8 01:47:13 2019
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 396.54                 Driver Version: 396.54                    |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|===============================+======================+======================|
|   0  GeForce GTX 108...  Off  | 00000000:01:00.0 Off |                  N/A |
|  0%   43C    P8    17W / 250W |    643MiB / 11178MiB |      0%      Default |
+-------------------------------+----------------------+----------------------+
|   1  GeForce GTX 108...  Off  | 00000000:02:00.0 Off |                  N/A |
|  0%   38C    P8    10W / 250W |      2MiB / 11178MiB |      0%      Default |
+-------------------------------+----------------------+----------------------+

+-----------------------------------------------------------------------------+
| Processes:                                                       GPU Memory |
|  GPU       PID   Type   Process name                             Usage      |
|=============================================================================|
+-----------------------------------------------------------------------------+

```

### 2. try a simple matrix operation with GPU

##### vim check.py​

```
## in container
## install vim
$apt-get update
$apt-get install vim

$vim check.py
```

```
# check.py
import tensorflow as tf

if __name__ == "__main__":
    with tf.device('/gpu:0'):
        a = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[2, 3], name='a')
        b = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[3, 2], name='b')
        c = tf.matmul(a, b)

    with tf.Session() as sess:
        print (sess.run(c))
```

##### execute check.py

```
## in container
root@nano-e3-gpu:/notebooks# python check.py
2019-01-08 01:57:42.545195: I tensorflow/core/platform/cpu_feature_guard.cc:141] Your CPU supports instructions that this TensorFlow binary was not compiled to use: AVX2 FMA
2019-01-08 01:57:42.671252: I tensorflow/stream_executor/cuda/cuda_gpu_executor.cc:964] successful NUMA node read from SysFS had negative value (-1), but there must be at least one NUMA node, so returning NUMA node zero
2019-01-08 01:57:42.671922: I tensorflow/core/common_runtime/gpu/gpu_device.cc:1432] Found device 0 with properties:
name: GeForce GTX 1080 Ti major: 6 minor: 1 memoryClockRate(GHz): 1.645
pciBusID: 0000:01:00.0
totalMemory: 10.92GiB freeMemory: 10.19GiB
2019-01-08 01:57:42.765408: I tensorflow/stream_executor/cuda/cuda_gpu_executor.cc:964] successful NUMA node read from SysFS had negative value (-1), but there must be at least one NUMA node, so returning NUMA node zero
2019-01-08 01:57:42.765970: I tensorflow/core/common_runtime/gpu/gpu_device.cc:1432] Found device 1 with properties:
name: GeForce GTX 1080 Ti major: 6 minor: 1 memoryClockRate(GHz): 1.645
pciBusID: 0000:02:00.0
totalMemory: 10.92GiB freeMemory: 10.76GiB
2019-01-08 01:57:42.766838: I tensorflow/core/common_runtime/gpu/gpu_device.cc:1511] Adding visible gpu devices: 0, 1
2019-01-08 01:57:43.120349: I tensorflow/core/common_runtime/gpu/gpu_device.cc:982] Device interconnect StreamExecutor with strength 1 edge matrix:
2019-01-08 01:57:43.120379: I tensorflow/core/common_runtime/gpu/gpu_device.cc:988]      0 1
2019-01-08 01:57:43.120385: I tensorflow/core/common_runtime/gpu/gpu_device.cc:1001] 0:   N Y
2019-01-08 01:57:43.120388: I tensorflow/core/common_runtime/gpu/gpu_device.cc:1001] 1:   Y N
2019-01-08 01:57:43.120910: I tensorflow/core/common_runtime/gpu/gpu_device.cc:1115] Created TensorFlow device (/job:localhost/replica:0/task:0/device:GPU:0 with 9847 MB memory) -> physical GPU (device: 0, name: GeForce GTX 1080 Ti, pci bus id: 0000:01:00.0, compute capability: 6.1)
2019-01-08 01:57:43.121302: I tensorflow/core/common_runtime/gpu/gpu_device.cc:1115] Created TensorFlow device (/job:localhost/replica:0/task:0/device:GPU:1 with 10405 MB memory) -> physical GPU (device: 1, name: GeForce GTX 1080 Ti, pci bus id: 0000:02:00.0, compute capability: 6.1)
[[22. 28.]
 [49. 64.]]

#success ^_^
```

### 3.compare with container without GPU

```
$ docker run -it --rm -p 7777:8888 --hostname [host_name] --name [container_name]  tensorflow/tensorflow /bin/bash

#and execute 1 and 2 upper
```



## step 3 : jupyter notebook

##### 1. execute `jupyter notebook --allow-root`

```
## in container
root@nano-e3-gpu:/notebooks# jupyter notebook --allow-root
[I 02:07:58.514 NotebookApp] Writing notebook server cookie secret to /root/.local/share/jupyter/runtime/notebook_cookie_secret
[I 02:07:58.526 NotebookApp] Serving notebooks from local directory: /notebooks
[I 02:07:58.526 NotebookApp] The Jupyter Notebook is running at:
[I 02:07:58.526 NotebookApp] http://(nano-e3-gpu or 127.0.0.1):8888/?token=79956ac5cf9e460693832b71b0a46cf3f0974519acaaeda8
[I 02:07:58.527 NotebookApp] Use Control-C to stop this server and shut down all kernels (twice to skip confirmation).
[C 02:07:58.527 NotebookApp]

    Copy/paste this URL into your browser when you connect for the first time,
    to login with a token:
        http://(nano-e3-gpu or 127.0.0.1):8888/?token=79956ac5cf9e460693832b71b0a46cf3f0974519acaaeda8
```



##### 2. Copy/paste the URL upper into your browser, note that to modify the host port, and note that the string after token is the initial password.







------

#### Reference:

1. docker hub(Tensorflow) : https://hub.docker.com/r/tensorflow/tensorflow/

1. https://blog.csdn.net/sinat_26917383/article/details/78728215#6tensorflowJupyter_notebook_213

