Kernel source for Qumo Quest 510 / Cubot X6
=========================================
Basic   | Spec Sheet
-------:|:-------------------------
CPU     | 1.7GHz Octa-Core MT6592
GPU     | Mali - 450 MP4
Memory  | 1GB RAM
Shipped Android Version | 5.1
Storage | Qumo - 8GB / Cubot - 16GB
Battery | 1920 mAh
Display | 5" 720 x 1280 px
Camera  | 8MPx + 2Mpx, LED Flash

![Qumo](http://qumo.ru/upload/iblock/bdb/qumo_quest_510_black.jpg "Qumo Quest 510")

=========================================

cd ~/q510_kernel

mkdir out

make ARCH=arm ARCH_MTK_PLATFORM=mt6592 O=out q510_defconfig

make ARCH=arm ARCH_MTK_PLATFORM=mt6592 O=out

=========================================

* WORKING LCM - OTA1283_HD_PNGD

* NOT WORKING
  * PROXIMITY SENSOR
  * LIGHT SENSOR 
  * MAIN IMGSENSOR
  * SUB IMGSENSOR
