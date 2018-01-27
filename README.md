# wiringpi-tft-tool
TFT Command Line Tool for Raspberry Pi/Orange Pi   

You can operate from command line.   
You can show any text string.   
You can choose BCM2835 library/WiringPi(WiringOp) library.   

I tested these TFT.   

SPI 128x128 ST7735   
SPI 128x128 ILI9163C   
SPI 128x160 ST7735   
SPI 240x320 ILI9340   
SPI 240x320 ILI9341   

8bit Parallel 240x320 ILI9325   
8bit Parallel 240x400 ILI9327   
8bit Parallel 240x320 ILI9341   
8bit Parallel 240x320 ILI9342   
8bit Parallel 320x480 ILI9481   
8bit Parallel 240x320 SPFD5408(Same as ILI9325)   
8bit Parallel 240x320 S6D1121   
8bit Parallel 240x320 R61505U (Same as ILI9325)   
8bit Parallel 240x400 R61509V   

----

Left to Right   
2.2 inch SPI 240x320 ILI9340   
1.44 inch SPI 128x128 ILI9163C   
1.8 inch SPI 128x160 ST7735   

![spi-tft-display-1](https://user-images.githubusercontent.com/6020549/28999994-c71fa140-7a97-11e7-858e-3110e922b466.JPG)

![spi-tft-display-2](https://user-images.githubusercontent.com/6020549/28999997-cdfd9aee-7a97-11e7-9eaf-51db1f0d74ac.JPG)

![spi-tft-display-3](https://user-images.githubusercontent.com/6020549/29000000-e99b0b74-7a97-11e7-9856-6ae5dc255c0b.JPG)

----

From Left Top to Right Buttom.   
2.4 inch 8Bit Parallel 240x320 ILI9341   
3.5 inch 8Bit Parallel 320x480 ILI9181   
2.4 inch 8Bit Parallel 240x320 ILI9325   
3.6 inch 8Bit Parallel 240x400 ILI9327   

![8bit-tft-1](https://user-images.githubusercontent.com/6020549/29200753-d60c3d90-7e92-11e7-8888-73a2d4bda61e.JPG)

----

Wirering for SPI TFT   

|TFT||Rpi/OPI|
|:-:|:-:|:-:|
|MISO|--|N/C|
|LED|--|3.3V|
|SCK|--|Pin#23(SPI SCLK)|
|MOSI|--|Pin#19(SPI MOSI)|
|RS|--|Pin#3(*)|
|RST|--|Pin#5(*)|
|CS|--|Pin#24(SPI CE0)(**)|
|GND|--|GND|
|VCC|--|3.3V|

\*You can change any pin.   
\**You can choose CE0 or CE1.   
Pin define is "spi.conf".   

Note:   
Opi have only 1 SPI.   
OPi-PC have CE0 and GPIO8.   
OPi ZERO have CE1 and GPIO8.   

----

Wirering for 8bit Parallel TFT   

|TFT||Rpi/Opi|
|:-:|:-:|:-:|
|LCD_RST|--|Pin#7|
|LCD_CS|--|Pin#3|
|LCD_RS|--|Pin#5|
|LCD_WR|--|Pin#26|
|LCD_RD|--|Pin#28|
|LCD_D0|--|Pin#29|
|LCD_D1|--|Pin#31|
|LCD_D2|--|Pin#33|
|LCD_D3|--|Pin#35|
|LCD_D4|--|Pin#37|
|LCD_D5|--|Pin#32|
|LCD_D6|--|Pin#36|
|LCD_D7|--|Pin#38|
|5V|--|5V(*)|
|3.3V|--|3.3V(*)|
|GND|--|GND|

\*When a regulator(It's often AMS1117) is mounted on the back, it's operated 5V.   
\*When a regulator is NOT mounted on the back, it's operated 3.3V.   

You can change any pin.   
Pin define is "pin.conf".   

----

Build for SPI TFT (using BCM2835 library, RPi Only, Very fast)  

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

Build for SPI TFT (using WiringPi/WiringOp library, Both of RPi/OPi)   

git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c spilib.c -lwiringPi -lm -DSPI -DWPI

----

Build for 8bit Parallel ILI9325 TFT (using WiringPi/WiringOp library, Both of RPi/OPi)   

git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -DILI9325   

----

Build for 8bit Parallel ILI9327 TFT (using WiringPi/WiringOp library, Both of RPi/OPi)   

git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -DILI9327   

----

Build for 8bit Parallel ILI9341 TFT (using WiringPi/WiringOp library, Both of RPi/OPi)   

git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -DILI9341   

----

Build for 8bit Parallel ILI9342 TFT (using WiringPi/WiringOp library, Both of RPi/OPi)   

git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -DILI9342   

----

Build for 8bit Parallel ILI9481 TFT (using WiringPi/WiringOp library, Both of RPi/OPi)   

git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -DILI9481   

----

Build for 8bit Parallel SPFD5408 TFT (using WiringPi/WiringOp library, Both of RPi/OPi)   

git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -DSPFD5408   

----

Build for 8bit Parallel S6D1121 TFT (using WiringPi/WiringOp library, Both of RPi/OPi)   

git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -DS6D1121   

----

Build for 8bit Parallel R61505U TFT (using WiringPi/WiringOp library, Both of RPi/OPi)   

git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -DR61505U   

----

Build for 8bit Parallel R61509V TFT (using WiringPi/WiringOp library, Both of RPi/OPi)   

git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -DR61509V   

----

How to use   

At first, edit "tft.conf".   
Set your TFT's resolution.   

If you use SPI TFT, check "spi.conf".   
Set your pin assignment.   

If you use 8bit Parallel TFT, check "pin.conf".   
Set your pin assignment.   

----

Command line parameters:   

s : show display(must root or sudo)   
r : remove all string
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
M : show build model   
P : show pin assignment   

Start Demo :   
chmod 777 test.sh   
./test.sh   

----

OrangePi   
![tft-opi](https://user-images.githubusercontent.com/6020549/29032790-c4407f94-7bcd-11e7-92f4-1c7e40978386.jpg)

RaspberryPi   
![tft-rpi](https://user-images.githubusercontent.com/6020549/29032792-c9fb2b32-7bcd-11e7-92e6-721f2ce276fa.jpg)

