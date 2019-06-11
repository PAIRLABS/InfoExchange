## Generate the tf_record file for object detection model.

Object detection's input is `.record` file, usually our raw dataset would be VOC format, so we have to convert it.

Requirement:

- [1_getting_started](./1_getting_started.md)

#### My VOC architecture:

```
.
└── training_data          # training VOC's root
|	├── Annotations        # xml files, one on one map to JPEGImages's photos
|	├── ImageSets          # txt files, contains photo path
|	└── JPEGImages         # jpg files, photos 
└──testing_data            # testing VOC's root
	├── Annotations        # xml files, one on one map to JPEGImages's photos
	├── ImageSets          # txt files, contains photo path
	└── JPEGImages         # jpg files, photos 
```

1. **Download your dataset**

   Put your dataset at the mounted path, so our data architecture become:

   ```
   ~
   └── dk-jaden-tensorflow        # mounted path
   	├── models                 # tensorflow/models package
   	├── protoc_3.3             # google protocol buffer tool
   	└── training_data          # training VOC's root
   	|	├── Annotations        # xml files, one on one map to JPEGImages's photos
   	|	├── ImageSets          # txt files, contains photo path
   	|	└── JPEGImages         # jpg files, photos
   	└── testing_data           # testing VOC's root
   		├── Annotations        # xml files, one on one map to JPEGImages's photos
   		├── ImageSets          # txt files, contains photo path
   		└── JPEGImages         # jpg files, photos
   ```

   

2. #### Convert VOC data to tfrecord

   Before converting, you need to split your data for training and testing.

   To split VOC's txt file under `ImageSets/` directory, use [create_splitted_set.py](./create_splitted_set.py) , put this python program into training_data directory.

   Go back to container, usage:

   ```shell
   $ cd /dk-jaden-tensorflow/training_data
   $ python ./create_splitted_set.py
   #Use "python ./create_splitted_set.py -h" for more detail
   ```

   Then you will get 3 splitted txt files in `./ImageSets/Splitted/`: 

   `"train.txt"`: Contains random 90% of training_data's path.

   `"test.txt"`: Contains random 5% of training_data's path.

   `"val.txt"`: Contains random 5% of training_data's path.

   For normal case,  `"train.txt"` and `"val.txt"` would be used in training phase, and `"test.txt"` is for testing phase. But we have another testing_data VOC, so we will only use `"train.txt"` and `"test.txt"` here.

   

   To convert VOC to tfrecord file, use [create_training_tf_record.py](./create_training_tf_record.py) and [create_testing_tf_record.py](./create_testing_tf_record.py). Put them into training_data directory.

   Usage:

   ```shell
   # For training data
   $ python ./create_training_tf_record.py
   # For testing data
   $ python ./create_testing_tf_record.py
   # Use --help for more detail
   ```

   It will take a while...

   Then, you will get a csv file called `train_labels.csv` in csv_data. It contains 6 columns which are ['filename', 'width', 'height', 'class', 'xmin', 'ymin', 'xmax', 'ymax'].

   `filename`: Path to the target photo.

   `width`: Width of the photo.

   `height`: Height of the photo.

   `class`: Such as pedestrian, rider, vehicle....

   `xmin`: Minimum x of bounding box of the object in the photo.

   `ymin`: Minimum y of bounding box of the object in the photo.

   `xmax`: Maximum x of bounding box of the object in the photo.

   `ymax`: Maximum y of bounding box of the object in the photo.

   

   And you will get the tfrecord files. Check the total size of your tfrecord files and compare it with the total file size of your image files, they should be about the same size.

   

   Also, the program generated the `label_map.pbtxt` for next step: [3_start_to_train](./3_start_to_train.md). 

   Notice: label_map.pbtxt and label_map_from_test.pbtxt should be the same.

   

3. Finally, your data architecture should be:

   ```
   ~
   └── dk-jaden-tensorflow        # mounted path
   	├── models                 # tensorflow/models package
   	├── protoc_3.3             # google protocol buffer tool
   	└── training_data          # training VOC's root
   	|	├── Annotations        # xml files, one on one map to JPEGImages's photos
   	|	├── ImageSets          # txt files, contains photo path
   	|	├── JPEGImages         # jpg files, photos
   	|	├── csv_data           # csv files
   	|	├── record_data        # tfrevoed files
   	|	├── label_map.pbtxt
   	|	├── label_map_from_test.pbtxt
   	|	├── create_training_tf_record.py
   	|	└── create_testing_tf_record.py
   	└── testing_data           # testing VOC's root
   		├── Annotations        # xml files, one on one map to JPEGImages's photos
   		├── ImageSets          # txt files, contains photo path
   		└── JPEGImages         # jpg files, photos
   ```

   

Previous: [1_getting_started](./1_getting_started.md)

Next: [3_start_to_train](./3_start_to_train.md)