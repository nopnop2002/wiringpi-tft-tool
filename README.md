# wiringpi-tft-tool
TFT Command Line Tool for Raspberry Pi/Orange Pi   

This is Command Line Tool for Raspberry Pi/Orange Pi.   
You can operate from command line.   
You can choose BCM2835 library/WiringPi(WiringOp) library.   

I tested these TFT.   
1.44 inch SPI 128x128 ST7735   
1.44 inch SPI 128x128 ILI9163C   
1.8 inch SPI 128x160 ST7735   
2.2 inch SPI 240x320 ILI9340   
2.4 inch SPI 240x320 ILI9341   
2.4 inch SPI 240x320 ILI9341   

----

Wirering for SPI TFT   

|SPI-TFT||Rpi/OPI|
|:-:|:-:|:-:|
|MISO|--|N/C|
|LED|--|3.3V|
|SCK|--|Pin#23(SPI SCLK)|
|MOSI|--|Pin#19(SPI MOSI)|
|D/C|--|Pin#3(*)|
|RES|--|Pin#5(*)|
|CS|--|Pin#24(SPI CE0)|
|GND|--|GND|
|VCC|--|3.3V|

(*) You can change any pin.   

----

build for SPI TFT (using BCM2835 library, RPi Only, Very fast)  

wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.42.tar.gz   
tar zxvf bcm2835-1.42.tar.gz   
cd bcm2835-1.42   
./configure   
make   
sudo make check   
sudo make install   
cd $HOME   
git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c spilib.c -lbcm2835 -lm -DSPI -DBCM   

----

build for SPI TFT (using WiringPi/WiringOp library)   

git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd spi-tft-tool   
cc -o tft tft.c fontx.c spilib.c -lwiringPi -lm -DSPI -DWPI

----

How to use   

At first,edit tft.conf.   
Set your TFT's resolution.   

Command line parameters:   
+n String : String for #n line   
-n : delete #n line   
Fn : set fore ground color for #n line   
Bn : set back ground color for #n line   
B0 : set base color(Default is WHITE)   
+U n : Set underline mode #n Line   
-U n : Unset underline mode #n Line   
+L : Scroll Up 1Line   
-L : Scroll Down 1Line   
Pn n : Set start colum n to line#n   
16 : draw 16 dot fonts   
24 : draw 24 dot fonts   
32 : draw 32 dot fonts   
R0 : set angle 0   
R90 : set angle 0   
R180 : set angle 0   
R270 : set angle 0   

Start Demo :   
chmod 777 test.sh   
./test.sh   

----

Left to Right   
2.2 inch SPI 240x320 ILI9340   
1.44 inch SPI 128x128 ILI9163C   
1.8 inch SPI 128x160 ST7735   

![spi-tft-display-1](https://user-images.githubusercontent.com/6020549/28999994-c71fa140-7a97-11e7-858e-3110e922b466.JPG)

![spi-tft-display-2](https://user-images.githubusercontent.com/6020549/28999997-cdfd9aee-7a97-11e7-9eaf-51db1f0d74ac.JPG)

![spi-tft-display-3](https://user-images.githubusercontent.com/6020549/29000000-e99b0b74-7a97-11e7-9856-6ae5dc255c0b.JPG)

![spi-tft-display-4](https://user-images.githubusercontent.com/6020549/29019732-71e6de4c-7b9a-11e7-8bab-5515bd38be34.jpg)

