## Getting started

Using TensorFlow with docker, object detection example.

#### step

1. Download docker:

   https://github.com/jadenlin0106/notebooks/blob/master/docker/install_docker.md



2. Create a container

   It will download docker image from Internet and run:

   ```shell
   # Create a volume for container
   $ mkdir ~/dk-jaden-tensorflow
   # Create the container
   $ docker run --runtime=nvidia --name jaden-tensorflow -v ~/dk-jaden-tensorflow:/dk-jaden-tensorflow -p 49152:8888 -p 49153:6006 -it tensorflow/tensorflow:1.12.0-gpu-py3 bash
   ```

   - `--runtime=nvidia` : Container can use GPU resource.
   - `--name jaden-tensorflow` : Container name.
   - `-v [host path:container path]` : Mount host disk to container. **Important!! if you don't have the folder in host, docker will create one, but the owner will be root.**
   - `-p 49152:8888` : Map port (Host:Container). 8888 is for "jupyter-notebook", 6006 is for "tensorboard"
   - `-it tensorflow/tensorflow:1.12.0-gpu-py3 bash` : Download image and interact with "bash". About images and tag, refer:
     - https://hub.docker.com/r/tensorflow/tensorflow
     - https://docs.nvidia.com/cuda/cuda-toolkit-release-notes/index.html
     - https://www.tensorflow.org/install/source

   

3. In **host** terminal, move to the path where you mounted and download the model

   ```shell
   $ cd ~/dk-jaden-tensorflow
   $ git clone https://github.com/tensorflow/models.git
   ```

   Acording to:https://github.com/tensorflow/models/blob/master/research/object_detection/g3doc/installation.md, we will need Tensorflow Object Detection API depends on the following libraries:

   *   Protobuf 3.0.0
   *   Python-tk
   *   Pillow 1.0
   *   lxml
   *   tf Slim (which is included in the "tensorflow/models/research/" checkout)
   *   Jupyter notebook
   *   Matplotlib
   *   Tensorflow (>=1.12.0)
   *   Cython
   *   contextlib2
   *   cocoapi

4. Install google protocol buffer tool `protoc`, and compile the code:

   ```shell
   $ cd ~/dk-jaden-tensorflow
   $ mkdir protoc_3.3
   
   # Download and unzip protoc 3.3
   $ cd protoc_3.3
   $ wget https://github.com/google/protobuf/releases/download/v3.3.0/protoc-3.3.0-linux-x86_64.zip
   $ chmod 775 protoc-3.3.0-linux-x86_64.zip
   $ unzip protoc-3.3.0-linux-x86_64.zip
   $ cd ../models/research/
   
   # Compile /models/research/object_detection/protos/*.proto with protoc
   $ ../../protoc_3.3/bin/protoc object_detection/protos/*.proto --python_out=.
   ```

5. Download the cocoapi:

   ```shell
   $ cd ~/dk-jaden-tensorflow
   $ git clone https://github.com/cocodataset/cocoapi.git
   ```

   

6. Go back to **container**, install dependencies and copy the pycocotools subfolder to the tensorflow/models/research directory:

   ```shell
   $ cd /dk-jaden-tensorflow
   $ pip install lxml
   $ pip install cython
   $ pip install tensorflow-gpu==1.12.2
   $ cd cocoapi/PythonAPI
   $ make
   $ python setup.py install
   $ cp -rp pycocotools /dk-jaden-tensorflow/models/research/
   ```

   The docker environment already included a lot of dependencies.

   

7. Set PYTHONPATH:

   ```SHELL
   # add models/research and models/research/slim in PYTHONPATH
   $ cd /dk-jaden-tensorflow/models/research
   $ export PYTHONPATH=$PYTHONPATH:`pwd`:`pwd`/slim
   ```

   

8. Open jupyter notebook:

   ```shell
   $ cd /dk-jaden-tensorflow
   $ jupyter-notebook --allow-root
   ```

   Open your browser and access `[You host IP]:49152`, use the token which shows on terminal for this page. (Notice that host's 49152 port => container's 8888 port)

   Get in ./models/research/object_detection/, click `object_detection_tutorial.ipynb` to run.

   

9. Test the code:

   If you want to test this code with your own images, put some images in the path, the path is located at:

   /dk-jaden-tensorflow/models/research/object_detection/test_images

   Then change the code like this:

   ```
   PATH_TO_TEST_IMAGES_DIR = 'test_images'
   TEST_IMAGE_PATHS = [ os.path.join(PATH_TO_TEST_IMAGES_DIR, 'image{}.jpg'.format(i)) for i in range(1, 8) ]
   ```

   Means I have image1.jpg to image7.jpg for this example.



Next: [2_convert_VOC_to_record](./2_convert_VOC_to_record.md)

