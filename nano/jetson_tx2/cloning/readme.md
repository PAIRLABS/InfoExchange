# how to do Jetson tx2 cloning

> step 1：Place TX2  system in Force USB Recovery Mode
>
> step 2：Read APP partition from TX2 board and save image to file.
>
> step 3：Restore your image to any board

### step 1：Place TX2  system in Force USB Recovery Mode

1. Power down the device. If connected, remove the AC adapter from the device. The device must be powered OFF, and **not** in suspend or sleep state.

2. Connect the Micro-B plug on the USB cable to the Recovery (USB Micro-B) Port on TX2 and the other end to an available USB port on the host PC.

3. Connect the power adapter to the device

4. do below action continuously (https://youtu.be/HaDy9tryzWc)：

   1. Power on TX2 (s4)
   2. Quickly press and hold the RECOVERY FORCE button (s3).
   3. While depressing the RECOVERY FORCE button (s3), deeply press and release the RESET button (s1). 
   4. Wait 2 seconds and release the RECOVERY FORCE button.

5. check if Recovery mode is successful, in host terminal：

   ```
   $ lsusb
   ...
   Bus xxx Device XXX: ID xxxx:xxxx NVidia Corp.
   ...
   ```



### step 2：Read APP partition from TX2 board and save image to file.

1. In host terminal, find the location of `flash.sh` in your jetpack

   ```
   #example
   nano@e2:~/Downloads$ locate flash.sh
   /home/nano/Downloads/jetpack/64_TX2/Linux_for_Tegra/flash.sh
   nano@e2:~/Downloads$ cd /home/nano/Downloads/jetpack/64_TX2/Linux_for_Tegra/
   nano@e2:~/Downloads/jetpack/64_TX2/Linux_for_Tegra$
   ```

2. read APP partition from TX2 board and save image to file

   ```
   [flash.sh location] $ sudo ./flash.sh -r -k APP -G my_backup.img jetson-tx2 mmcblk0p1
   ```

3. check if `my_backup.img` is exist, this is your backup file,



### step 3：Restore your image to any board

1. As step 1, place TX2  system in Force USB Recovery Mode

2. As step 2-1, in host terminal, find the location of `flash.sh` in your jetpack

   ```
   #example
   nano@e2:~/Downloads$ locate flash.sh
   /home/nano/Downloads/jetpack/64_TX2/Linux_for_Tegra/flash.sh
   nano@e2:~/Downloads$ cd /home/nano/Downloads/jetpack/64_TX2/Linux_for_Tegra/
   nano@e2:~/Downloads/jetpack/64_TX2/Linux_for_Tegra$
   ```

3. rename `my_backup.img` to `bootloader/system.img`, which will replace the old file, you can backup the older `system.img` if you want.

4. flashing the saved image to your board

   ```
   $ sudo ./flash.sh -r -k APP jetson-tx2 mmcblk0p1
   ```
