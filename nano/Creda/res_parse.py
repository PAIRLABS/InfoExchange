import xml.etree.ElementTree as ET

def parse_rec(filename):
    """parse a pascal voc xml file"""
    tree = ET.parse(filename) # 
    objects = []
    for obj in tree.findall('object'):
        obj_struct = {}
        obj_struct['type'] = obj.find('type').text
        bbox = obj.find('bndbox')
        obj_struct['bbox'] = [int(float(bbox.find('xmin').text)),
                              int(float(bbox.find('ymin').text)),
                              int(float(bbox.find('xmax').text)),
                              int(float(bbox.find('ymax').text))]
        objects.append(obj_struct)
    return objects
