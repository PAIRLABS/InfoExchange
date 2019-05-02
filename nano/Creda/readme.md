# convert Pascal VOC of CreDa to video

> step0：git clone
>
> step1：download  Pascal VOC from CreDa data
>
> step2：unzip the folder
>
> step3：execute convert_video.py



#### step0：git clone

```
 $ git clone git@github.com:Nano1201/InfoExchange.git
 $ cd InfoExchange/nano/Creda
```



#### step1：download voc from creDa data

#### ![p1](C:\Users\Nano\Desktop\ai實習\logs\creDa\img\p1.png)



#### step2 : unzip the folder (to input folder)

```
~/Download$ unzip [zipfile] -d [input folder]
```

1. I suggest that input folder should contain only folders of Pascal Voc(without other folders).

   ​	![p2](C:\Users\Nano\Desktop\ai實習\logs\creDa\img\p2.png)



#### step3 : execute gen_video.py

> the output video will save in a folder named 'output' in the directory you execute gen_video.py

1. defautly: convert pascal voc from local folder 'input'

   ```
   $ python3 gen_video.py
   ```

2. or use `-f` to assign input folder  

   ```
   $ python3 gen_video.py -f [absolute path or relative path of input folder]
   ```



#### step4 : check output file

```
[path you execute gen_video.py] $ cd output 
```

