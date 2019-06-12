from __future__ import print_function
from argparse import ArgumentParser
import os
import random

parser = ArgumentParser(usage="python create_train_test_set.py [-p input-path]", description="Description: Randomly split ./ImageSets/*.txt to training, validation and testing part, by 90:5:5")
parser.add_argument("-p", "--inputPath", help="Path to the input txt file. default=\'ImageSets/All.txt\'", dest="inputPath", default="ImageSets/All.txt")
args = parser.parse_args()

def _main():
    untrain_percent = 0.1
    te_percent = 0.5
    xmlfilepath = str(args.inputPath)
    ftotal_xml = open(xmlfilepath,'r')
    #print(ftotal_xml.readlines())
    total_xml = ftotal_xml.readlines()

    num = len(total_xml)
    list = range(num)
    ut = int(num * untrain_percent)
    te = int(ut * te_percent)
    untrain = random.sample(list, ut)
    test = random.sample(untrain, te)

    if not os.path.exists('ImageSets/Splitted'):
        os.mkdir('ImageSets/Splitted')
        print('Directory ImageSets/Splitted Created')
    else:    
        print('')
    fval = open('ImageSets/Splitted/val.txt', 'w')
    ftest = open('ImageSets/Splitted/test.txt', 'w')
    ftrain = open('ImageSets/Splitted/train.txt', 'w')


    for i in list:
        name = total_xml[i]
        if i in untrain:            #0.1
            if i in test:           #0.1*0.5
                ftest.write(name)
            else:                   #0.1*0.5
                fval.write(name)
        else:                       #0.9
            ftrain.write(name)

    ftrain.close()
    fval.close()
    ftest.close()


if __name__ == '__main__':
    _main()