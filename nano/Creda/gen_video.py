from res_parse import parse_rec
from PIL import Image
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

from argparse import ArgumentParser

import os

def update(img,ax,folder_path,type_color,color_list):

    img_file = folder_path+'/JPEGImages/'+img+'.jpg'
    xml_file = folder_path+'/Annotation/'+img+'.xml'

    object_list = parse_rec(xml_file)
    
    ax.clear()
    ax.imshow(Image.open(img_file), aspect='equal')

    for obj in object_list:
        bbox = obj['bbox']
        obj_type = obj['type']
        

        if obj_type not in type_color:
            try:
                type_color[obj_type]=color_list.pop(0)
            except:
                type_color[obj_type]='black'  #color_list pop to length 0

        
        ax.add_patch(
            plt.Rectangle((bbox[0], bbox[1]),
                            bbox[2] - bbox[0],
                            bbox[3] - bbox[1], fill=False,
                          edgecolor=type_color[obj_type], linewidth=3.5
            )
        )#画矩形
        ax.text(bbox[0], bbox[1] - 2,
                '{:s}'.format(obj_type),
                bbox=dict(facecolor='blue', alpha=0.5),
                fontsize=14, color='white')#矩形框的标签

    ax.set_title(img.split('/')[2])


def gen_video(folder_path,type_color,color_list):
  
    img_set = list()
    with open(folder_path+'/ImageSets/All.txt') as f:
        img_set = f.readlines()
        f.close()
    img_set = [x.strip() for x in img_set]
    img_num = len(img_set)

    print('dealing with video '+ folder_path.split('/')[-1] +' ( image num:'+ str(img_num) + ', video time:' +str(img_num/30)+'s )')

    fig, ax = plt.subplots(figsize=(12, 12))
    plt.tight_layout()
    ani = FuncAnimation(fig,update,frames=img_set,fargs=[ax,folder_path,type_color,color_list],interval=1,repeat=False)
    mp4_name = folder_path.split('/')[-1]+'.mp4'
    ani.save(os.getcwd()+'/output/'+mp4_name,writer='ffmpeg',fps=30)
    #plt.show()
    plt.close()
    print('--------------------')
    return img_num

def AssignFolderByCmd(video_list):
    parser = ArgumentParser()
    parser.add_argument("-f", "--folder", help="assign input folder", dest="folder",type=str, default=os.getcwd()+'/input/')
    args = parser.parse_args()

    
    if not os.path.isabs(args.folder) :  #not absouluted root
        args.folder = os.getcwd()+'/'+args.folder+'/'

    files = [ args.folder+item for item in os.listdir(args.folder) if os.path.isdir(args.folder+item) ]
    
    files.sort()
    #print(files)
    video_list+=files
    #return args.folder

def main():

    video_list=list()
    AssignFolderByCmd(video_list)

    type_color = {'car':'red', 'cycles':'yellow', 'human':'green' }
    color_list = ['magenta','lightBlue','orange','darkGreen','gray','white','blue']

    img_num = 0

    print('Convert video in ' + '/'.join(video_list[0].split('/')[0:-1]) + ' : \n')

    for folder_path in video_list:
        #folder_path = set_path+folder_name
        img_num+=gen_video(folder_path,type_color,color_list)

    print('color map : ')
    print(type_color)
    print('total image num : ' + str(img_num))
    print('total video time :' + str(img_num/30) + 's')

if __name__ == '__main__':
    main()
