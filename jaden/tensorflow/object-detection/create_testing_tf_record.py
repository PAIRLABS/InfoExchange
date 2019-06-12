from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import print_function
from argparse import ArgumentParser

import os
import glob
import pandas as pd
import xml.etree.ElementTree as ET
import io
import tensorflow as tf

from PIL import Image
from object_detection.utils import dataset_util
from collections import namedtuple, OrderedDict

parser = ArgumentParser(usage="python create_train_test_set.py [-p input-path]", description="Description: Convert xml to csv, csv to tf_record, and generate label_map_from_testing.pbtxt")
parser.add_argument("-p", "--inputPath", help="Path to the input txt file. default=\'ImageSets/Splitted/test.txt\'", dest="inputPath", default="ImageSets/Splitted/test.txt")
args = parser.parse_args()

flags = tf.app.flags
flags.DEFINE_string('csv_input', 'csv_data/test_labels.csv', '')
flags.DEFINE_string('output_path', 'record_data/test.record', '')
flags.DEFINE_string('image_dir', '', '')
FLAGS = flags.FLAGS
classes_text_type_amount = []
classes_text_type_amount.append('')


# TO-DO replace this with label map
def class_text_to_int(row_label):
    # if row_label == 'pedestrian':
    #     return 1
    # if row_label == 'rider':
    #     return 2
    # if row_label == 'vehicle':
    #     return 3
    global classes_text_type_amount
    if row_label in classes_text_type_amount:
        #print('row_label='+str(row_label)+' :'+str(classes_text_type_amount.index(row_label)))
        return int(classes_text_type_amount.index(row_label))
    else:
        None


def split(df, group):
    data = namedtuple('data', ['filename', 'object'])
    gb = df.groupby(group)
    return [data(filename, gb.get_group(x)) for filename, x in zip(gb.groups.keys(), gb.groups)]

def scan_tf_example(group, path):   
    global classes_text_type_amount
    
    for index, row in group.object.iterrows():
        classtmp = str(row['class'].encode('utf8')).split('b')[1]
        classtmp = classtmp.split('\'')[1]
        if (classtmp in classes_text_type_amount) == False:
            classes_text_type_amount.append(classtmp)
            print(classtmp)

def create_tf_example(group, path):
    with tf.gfile.GFile(os.path.join(path, '{}'.format(group.filename)), 'rb') as fid:
        encoded_jpg = fid.read()
    encoded_jpg_io = io.BytesIO(encoded_jpg)
    image = Image.open(encoded_jpg_io)
    width, height = image.size

    filename = group.filename.encode('utf8')
    image_format = b'jpg'
    xmins = []
    xmaxs = []
    ymins = []
    ymaxs = []
    classes_text = []
    classes = []

    for index, row in group.object.iterrows():
        xmins.append(row['xmin'] / width)
        xmaxs.append(row['xmax'] / width)
        ymins.append(row['ymin'] / height)
        ymaxs.append(row['ymax'] / height)
        classes_text.append(row['class'].encode('utf8'))
        classes.append(class_text_to_int(row['class']))

    tf_example = tf.train.Example(features=tf.train.Features(feature={
        'image/height': dataset_util.int64_feature(height),
        'image/width': dataset_util.int64_feature(width),
        'image/filename': dataset_util.bytes_feature(filename),
        'image/source_id': dataset_util.bytes_feature(filename),
        'image/encoded': dataset_util.bytes_feature(encoded_jpg),
        'image/format': dataset_util.bytes_feature(image_format),
        'image/object/bbox/xmin': dataset_util.float_list_feature(xmins),
        'image/object/bbox/xmax': dataset_util.float_list_feature(xmaxs),
        'image/object/bbox/ymin': dataset_util.float_list_feature(ymins),
        'image/object/bbox/ymax': dataset_util.float_list_feature(ymaxs),
        'image/object/class/text': dataset_util.bytes_list_feature(classes_text),
        'image/object/class/label': dataset_util.int64_list_feature(classes),
    }))
    return tf_example

def xml_to_csv(directory):
    xml_list = []
    all_path_set = list()
    # cnt=0
    with open(str(args.inputPath)) as f1:
        all_path_set = f1.readlines()
        f1.close()
    all_path_set = [x.strip() for x in all_path_set]

    for each_path in all_path_set:
        #print(os.path.join(directory,each_path)+'.xml')
        xml_file = os.path.join(directory,each_path)+'.xml'    
        tree = ET.parse(xml_file)
        root = tree.getroot()
        if not root.findall('object'):
            # cnt=cnt+1
            # print(xml_file+'No object, No:'+str(cnt))
            value = (os.path.join('JPEGImages', each_path)+'.jpg',
                    1920 if root.find('size') is None  else int(root.find('size')[0].text),
                    1080 if root.find('size') is None  else int(root.find('size')[1].text),
                    root.find('name').text,
                    int(root.find('bndbox')[0].text),
                    int(root.find('bndbox')[1].text),
                    int(root.find('bndbox')[2].text),
                    int(root.find('bndbox')[3].text)
                    )
            xml_list.append(value)

        # if len(root.findall('object'))==1:
        #     print(xml_file+'1 object, No:'+str(cnt))

        for member in root.findall('object'):
            value = (os.path.join('JPEGImages', each_path)+'.jpg',
                    1920 if root.find('size') is None  else int(root.find('size')[0].text),
                    1080 if root.find('size') is None  else int(root.find('size')[1].text),
                    member.find('name').text,
                    int(member.find('bndbox')[0].text),
                    int(member.find('bndbox')[1].text),
                    int(member.find('bndbox')[2].text),
                    int(member.find('bndbox')[3].text)
                    )
            xml_list.append(value)
    column_name = ['filename', 'width', 'height', 'class', 'xmin', 'ymin', 'xmax', 'ymax']
    xml_df = pd.DataFrame(xml_list, columns=column_name)
    return xml_df


def main(_):
    # Create csv_data Directory if don't exist
    if not os.path.exists('csv_data'):
        os.mkdir('csv_data')
        print('Directory csv_data Created')
    else:    
        print('')
    
    for directory in ['Annotations/']:
        xml_df = xml_to_csv(directory)
        xml_df.to_csv('csv_data/test_labels.csv', index=None)
        print('Successfully converted xml to csv.')

    if FLAGS.output_path == 'record_data/test.record':
        # Create record_data Directory if don't exist
        if not os.path.exists('record_data'):
            os.mkdir('record_data')
            print('Directory record_data Created')
        else:    
            print('')

    writer = tf.python_io.TFRecordWriter(FLAGS.output_path)
    path = os.path.join(FLAGS.image_dir)
    examples = pd.read_csv(FLAGS.csv_input)
    grouped = split(examples, 'filename')
    
    # Scan labels csv
    print('Scaning classes...')
    for group in grouped:
        scan_tf_example(group, path)
    print('Scan finished, Classes after sort():')
    classes_text_type_amount.sort()
    print(classes_text_type_amount[1:])
    
    # Start to generate tfrecord
    print('Generating tfrecord file...')
    for group in grouped:
        tf_example = create_tf_example(group, path)
        writer.write(tf_example.SerializeToString())
    writer.close()
    output_path = os.path.join(os.getcwd(), FLAGS.output_path)
    print('Successfully created the TFRecords: {}'.format(output_path))
    
    # Generate pbtxt
    f = open('label_map_from_testing.pbtxt', 'w', encoding='UTF-8')
    for c in range(1,len(classes_text_type_amount)):
        f.write('item {\n')
        f.write('  id: {}\n'.format(c))
        f.write('  name: \'{}\'\n'.format(classes_text_type_amount[c]))
        f.write('}\n\n')
    f.close()
    print('Acording to Classes created the label_map_from_testing.pbtxt successfully')


tf.app.run()