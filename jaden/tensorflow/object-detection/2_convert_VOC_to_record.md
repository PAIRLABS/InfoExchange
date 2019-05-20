## Generate the tf_record file for object detection model.

Object detection's input is `.record` file, usually our raw dataset would be VOC format, so we have to convert it.

Requirement:

- [1_getting_started](./1_getting_started.md)

#### My VOC architecture:

```
.
└── training_data           #training VOC's root
|	├── Annotations        # xml files, one on one map to JPEGImages's photos
|	├── ImageSets          # txt files, contains photo path
|	└── JPEGImages         # jpg files, photos 
└──testing_data            #testing VOC's root
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
   	└── training_data           #training VOC's root
   	|	├── Annotations        # xml files, one on one map to JPEGImages's photos
   	|	├── ImageSets          # txt files, contains photo path
   	|	└── JPEGImages         # jpg files, photos
   	└── testing_data           #testing VOC's root
   		├── Annotations        # xml files, one on one map to JPEGImages's photos
   		├── ImageSets          # txt files, contains photo path
   		└── JPEGImages         # jpg files, photos
   ```

   

2. #### Convert VOC data to tfrecord

   Before converting, you may need to split your data for training and testing. I didn't, cause I got two of VOC data, one for training and the other one for testing, so...please search this part on google.

   To convert VOC to tfrecord file, use [training_xml_to_csv_to_tfrecord.py](./training_xml_to_csv_to_tfrecord.py) and [testing_xml_to_csv_to_tfrecord.py](./testing_xml_to_csv_to_tfrecord.py). Put them into training_data and testing_data directories.

   Go back to container, usage:

   ```shell
   # For training data
   $ cd /dk-jaden-tensorflow/training_data
   $ python ./training_xml_to_csv_to_tfrecord.py
   # For testing data
   $ cd /dk-jaden-tensorflow/testing_data
   $ python ./testing_xml_to_csv_to_tfrecord.py
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

   Notice: label_map.pbtxt of training and testing should be the same.

   

3. Finally, your data architecture should be:

   ```
   ~
   └── dk-jaden-tensorflow        # mounted path
   	├── models                 # tensorflow/models package
   	├── protoc_3.3             # google protocol buffer tool
   	└── training_data          #training VOC's root
   	|	├── Annotations        # xml files, one on one map to JPEGImages's photos
   	|	├── ImageSets          # txt files, contains photo path
   	|	├── JPEGImages         # jpg files, photos
   	|	├── csv_data           # csv files
   	|	├── record_data        # tfrevoed files
   	|	├── label_map.pbtxt
   	|	└── training_xml_to_csv_to_tfrecord.py
   	└── testing_data           #testing VOC's root
   		├── Annotations        # xml files, one on one map to JPEGImages's photos
   		├── ImageSets          # txt files, contains photo path
   		├── JPEGImages         # jpg files, photos
   		├── csv_data           # csv files
   		├── record_data        # tfrevoed files
   		├── label_map.pbtxt
   		└── testing_xml_to_csv_to_tfrecord.py
   ```

   

Previous: [1_getting_started](./1_getting_started.md)

Next: [3_start_to_train](./3_start_to_train.md)