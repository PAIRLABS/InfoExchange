## Training

**My environment:**

tensorflow-gpu: 1.12.2

cuda: 9.0.167

python: 3.5.2

**Requirement:**

- [1_getting_started](./1_getting_started.md)
- [2_convert_VOC_to_record](./2_convert_VOC_to_record.md)

After [2_convert_VOC_to_record](./2_convert_VOC_to_record.md), you should have your tfrecord files (training and testing).

1. Preparation, in **host**:

   ```shell
   $ cd ~/dk-jaden-tensorflow/models/research/object_detection
   $ mkdir ssd_model
   $ mv ~/dk-jaden-tensorflow/training_data/record_data/train.record ~/dk-jaden-tensorflow/testing_data/record_data/test.record ./ssd_model
   ```

   Move `label_map.pbtxt` file to ./ssd_model:

   ```shell
   $ mv ~/dk-jaden-tensorflow/training_data/label_map.pbtxt ./ssd_model
   ```

   The purpose of `label_map.pbtxt` is to map Class ID to Class name. looks like:

   ```pbtxt
   item {
     id: 1
     name: 'class1'
   }
   
   item {
     id: 2
     name: 'class2'
   }
   
   item {
     id: 3
     name: 'class3'
   }
   	.
   	.
   	.
   
   ```

   

2. In **host**, download pre-trained model from [here](https://github.com/tensorflow/models/blob/master/research/object_detection/g3doc/detection_model_zoo.md), and extract it like this:

   ```shell
   $ wget http://download.tensorflow.org/models/object_detection/ssd_mobilenet_v1_coco_2018_01_28.tar.gz -P ~/dk-jaden-tensorflow/
   $ tar -xzvf ~/dk-jaden-tensorflow/ssd_mobilenet_v1_coco_2018_01_28.tar.gz --directory ~/dk-jaden-tensorflow
   $ mv ~/dk-jaden-tensorflow/ssd_mobilenet_v1_coco_2018_01_28/* ./ssd_model/
   ```

   I used the "[ssd_mobilenet_v1_coco](http://download.tensorflow.org/models/object_detection/ssd_mobilenet_v1_coco_2018_01_28.tar.gz)".

   

3. Also, download model config file from [here](https://github.com/tensorflow/models/tree/master/research/object_detection/samples/configs), and modify it:

   ```shell
   $ cp ~/dk-jaden-tensorflow/models/research/object_detection/samples/configs/ssd_mobilenet_v1_coco.config ./ssd_model/
   ```

   I used the "[ssd_mobilenet_v1_coco.config](https://github.com/tensorflow/models/blob/master/research/object_detection/samples/configs/ssd_mobilenet_v1_coco.config)" here.

   ```config
   line 9:
   	num_classes: 3
       → number of class
   	
   line 141:
       batch_size: 12
       → recommend 8-12, reduce this number if your GPU ram is not enough.
   
   line 156:
       fine_tune_checkpoint: "/dk-jaden-tensorflow/models/research/object_detection/ssd_model/model.ckpt"
       →path to the pre-trained model, put "/model.ckpt" at the tail.
   
   line 162:
       num_steps: 200000
       →When will it stop, or you can just use ctrl+c
   
   line 173:
       train_input_reader: {
         tf_record_input_reader {
           input_path: "/dk-jaden-tensorflow/models/research/object_detection/ssd_model/train.record"
               →Path to train.record.
               
   line 185 add:
   	metrics_set: "coco_detection_metrics"
               
   line 188:
       eval_input_reader: {
         tf_record_input_reader {
           input_path: "/dk-jaden-tensorflow/models/research/object_detection/ssd_model/test.record"
               →Path to test.record.
               
   line 177 and 192:
       label_map_path: "/dk-jaden-tensorflow/models/research/object_detection/ssd_model/label_map.pbtxt"
       →Path to pascal_label_map.pbtxt
   ```

   Now, your ssd_model directory should have:

   ![1557901972787](./images/ssd_model_ls.png)

   

4. Training:

   In **container**:

   ```shell
   $ cd /dk-jaden-tensorflow/models/research
   # Set PYTHONPATH
   $ export PYTHONPATH=$PYTHONPATH:`pwd`:`pwd`/slim
   ```

   If you didn't turn off your terminal after  [1_getting_started](./1_getting_started.md), then you don't need to set PYTHONPATH.

   Start to train:

   ```shell
   $ python object_detection/legacy/train.py --train_dir object_detection/train --pipeline_config_path object_detection/ssd_model/ssd_mobilenet_v1_coco.config
   ```

   --train_dir: checkpoint, model.ckpt.....output's directory.

   --pipeline_config_path: Path of config file.

   It will start to train.

   Using tensorflow (CPU):

   ![1557904357837](./images/Itistraining.png)

   Using tensorflow-gpu==1.12.2 (GPU):

   ![1557904357837](./images/Itistraining2.png)

   as you can see, GPU is 10 times faster than CPU.

   

5. Evaluate:

   Modify `~/dk-jaden-tensorflow/models/research/object_detection/utils/object_detection_evaluation.py`:

   ```python
   # replace line 213:
   	category_name = unicode(category_name, 'utf-8')
   # to
   	category_name = str(category_name, 'utf-8')
   ```

   Start to evaluage:

   ```shell
   $ cd /dk-jaden-tensorflow/models/research
   $ python object_detection/legacy/eval.py   --logtostderr   --pipeline_config_path=object_detection/ssd_model/ssd_mobilenet_v1_coco.config   --checkpoint_dir=object_detection/train --eval_dir=object_detection/eval
   ```

6. Monitor it by **tensorboard**:

   ```shell
   $ tensorboard --logdir /models/research/object_detection/
   ```

   and access [Host IP:port] on your browser.



Note:

If you encounter this while you are starting to train: 

```
............ImportError: libcublas.so.10.0: cannot open shared object file: No such file or directory
```

Check your tensorflow-gpu version:

```shell
$ pip show tensorflow
#or
$ pip show tensorflow-gpu
```

and check CUDA version:

```shell
$ cat /usr/local/cuda/version.txt
#or
$ nvcc --version
```

Compatible table: 

![1557912961580](./images/tensorflowCUDAtable.png)

from: https://www.tensorflow.org/install/source#gpu_support_2



Previous: [2_convert_VOC_to_record](./2_convert_VOC_to_record.md)