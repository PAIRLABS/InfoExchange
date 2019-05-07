from argparse import ArgumentParser

import os
import shutil


def BranchDataToBranchFolder(vfolder_path,test_folder_path,train_folder_path): # ex : /home/nano/test/CreDa converter/input/10_00
    ori_data = list()
    with open(vfolder_path+'/ImageSets/All.txt') as f:
        ori_data = f.readlines()
        f.close()
    ori_data = [x.strip() for x in ori_data]

    test_data = ori_data[::18]                                          #ex : All/1_40_00.mp4/1_40_00.mp4_00013
    train_data = [data for data in ori_data if data not in test_data]      #ex : All/1_40_00.mp4/1_40_00.mp4_00013

    video_name = vfolder_path.split('/')[-1]
    print('dealing with ' + video_name + '...')

    Annotation_folder = video_name+'/Annotation/All/'+video_name+'.mp4'
    JPEGImages_folder = video_name+'/JPEGImages/All/'+video_name+'.mp4'

    os.makedirs(test_folder_path+Annotation_folder)  
    os.makedirs(test_folder_path+video_name+'/ImageSets')
    os.makedirs(test_folder_path+JPEGImages_folder)

    os.makedirs(train_folder_path+Annotation_folder)
    os.makedirs(train_folder_path+video_name+'/ImageSets')
    os.makedirs(train_folder_path+JPEGImages_folder)

    cnt=0

    with open(test_folder_path+video_name+'/ImageSets/All.txt', 'w') as f:
        f.write('\n'.join(test_data))
        f.close

    for data in test_data:
        src = vfolder_path+'/Annotation/'+data+'.xml'
        des = test_folder_path+Annotation_folder
        shutil.copy2(src,des)

        src = vfolder_path+'/JPEGImages/'+data+'.jpg'
        des = test_folder_path+JPEGImages_folder
        shutil.copy2(src,des)
        cnt+=1

    with open(train_folder_path+video_name+'/ImageSets/All.txt', 'w') as f:
        f.write('\n'.join(train_data))
        f.close

    for data in train_data:
        src = vfolder_path+'/Annotation/'+data+'.xml'
        des = train_folder_path+Annotation_folder
        shutil.copy2(src,des)

        src = vfolder_path+'/JPEGImages/'+data+'.jpg'
        des = train_folder_path+JPEGImages_folder
        shutil.copy2(src,des)
        cnt+=1

    return cnt


def GenTwoBranchFolder(test_folder_path,train_folder_path):

    try:
        shutil.rmtree(test_folder_path)
    except:
        pass

    try:
        shutil.rmtree(train_folder_path)
    except:
        pass

    try:
        os.makedirs(test_folder_path)
    except:
        pass

    try:
        os.makedirs(train_folder_path)
    except:
        pass

def AssignFolderByCmd(vfolders_path):
    parser = ArgumentParser()
    parser.add_argument("-f", "--folder", help="get input folder", dest="folder",type=str, default=os.getcwd()+'/input/')
    args = parser.parse_args()

    
    if not os.path.isabs(args.folder) :  #not absouluted root
        args.folder = os.getcwd()+'/'+args.folder+'/'

    temp_list = [ args.folder+vfolder for vfolder in os.listdir(args.folder) if os.path.isdir(args.folder+vfolder) ]
    
    temp_list.sort()
    vfolders_path+=temp_list

def main():

    vfolders_path=list()

    test_folder_path = os.getcwd() + '/test_data/'
    train_folder_path= os.getcwd() + '/train_data/'

    AssignFolderByCmd(vfolders_path)
    GenTwoBranchFolder(test_folder_path,train_folder_path)

    img_cnt=0
    for vfolder_path in vfolders_path:
        img_cnt+=BranchDataToBranchFolder(vfolder_path,test_folder_path,train_folder_path)

    print('img count : ' + str(img_cnt))

if __name__ == '__main__':
    main()