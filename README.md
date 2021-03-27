# wiringpi-tft-tool
TFT Command Line Tool for Raspberry Pi/Orange Pi   

You can operate from command line.   
You can show any text string.   
You can choose BCM2835 library/WiringPi(WiringOp) library.   

I tested these TFT.   

SPI 128x128 ST7735   
SPI 128x160 ST7735   
SPI 240x320 ILI9340   
SPI 240x320 ILI9341   

8bit Parallel 240x320 ILI9325   
8bit Parallel 240x400 ILI9327   
8bit Parallel 240x320 ILI9341   
8bit Parallel 240x320 ILI9342   
8bit Parallel 320x480 ILI9481   
8bit Parallel 240x320 SPFD5408   
8bit Parallel 240x320 S6D1121   
8bit Parallel 240x320 R61505U   
8bit Parallel 240x400 R61509V   
8bit Parallel 240x320 ST7781   

----

Left to Right   
2.2 inch SPI 240x320 ILI9340   
1.44 inch SPI 128x128 ST7735   
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

# Wirering for SPI TFT   

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

# Wirering for 8bit Parallel TFT   

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

# Build for SPI TFT using BCM2835 library   
RPi Only, Very fast  

```
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
cc -o tft tft.c fontx.c spilib.c -lbcm2835 -lm -lpthread -DSPI -DBCM   
```

----

# Build for SPI TFT using WiringPi/WiringOp library  
Both of RPi/OPi   

```
git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c spilib.c -lwiringPi -lm -lpthread -DSPI -DWPI
```

----

# Build for 8bit Parallel ILI9325 TFT using WiringPi/WiringOp library  
Both of RPi/OPi   

```
git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -lpthread -DILI9325   
```

----

# Build for 8bit Parallel ILI9327 TFT using WiringPi/WiringOp library   
Both of RPi/OPi   

```
git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -lpthread -DILI9327   
```

----

# Build for 8bit Parallel ILI9341 TFT using WiringPi/WiringOp library   
Both of RPi/OPi   

```
git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -lpthread -DILI9341   
```

----

# Build for 8bit Parallel ILI9342 TFT using WiringPi/WiringOp library   
Both of RPi/OPi   

```
git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -lpthread -DILI9342   
```

----

# Build for 8bit Parallel ILI9481 TFT using WiringPi/WiringOp library   
Both of RPi/OPi   

```
git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -lpthread -DILI9481   
```

----

# Build for 8bit Parallel SPFD5408 TFT using WiringPi/WiringOp library   
Both of RPi/OPi   

```
git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -lpthread -DSPFD5408   
```

----

# Build for 8bit Parallel S6D1121 TFT using WiringPi/WiringOp library  
Both of RPi/OPi   

```
git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -lpthread -DS6D1121   
```

----

# Build for 8bit Parallel R61505U TFT using WiringPi/WiringOp library  
Both of RPi/OPi   

```
git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -lpthread -DR61505U   
```

----

# Build for 8bit Parallel R61509V TFT using WiringPi/WiringOp library  
Both of RPi/OPi   

```
git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -lpthread -DR61509V   
```

----

# Build for 8bit Parallel ST7781 TFT using WiringPi/WiringOp library  
Both of RPi/OPi   

```
git clone https://github.com/nopnop2002/wiringpi-tft-tool   
cd wiringpi-tft-tool   
cc -o tft tft.c fontx.c 8bitlib.c -lwiringPi -lm -lpthread -DST7781   
```

----

# How to use   

At first, edit "tft.conf".   
Set your TFT's resolution.   

If you use SPI TFT, check "spi.conf".   
Set your pin assignment.   

If you use 8bit Parallel TFT, check "pin.conf".   
Set your pin assignment.   

----

# Command line parameters   

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

----

# Start Demo    
chmod 777 test.sh   
./test.sh   

---

# Font File   
This tool uses the following as default fonts:   
- fontx/ILGH16XB.FNT // 8x16Dot Alphanumeric
- fontx/ILGH24XB.FNT // 12x24Dot Alphanumeric
- fontx/ILGH32XB.FNT // 16x32Dot Alphanumeric
- fontx/ILGZ16XB.FNT // 16x16Dot Japanese ShiftJIS(Not unicode)
- fontx/ILGZ24XB.FNT // 24x24Dot Japanese ShiftJIS(Not unicode)
- fontx/ILGZ32XB.FNT // 32x32Dot Japanese ShiftJIS(Not unicode)

Changing this file will change the font.

You can add your original fonts.   
The format of the font file is the FONTX format.   
Please refer [this](http://elm-chan.org/docs/dosv/fontx_e.html) page about FONTX format.   

---

# Font File Editor(FONTX Editor)   
[There](http://elm-chan.org/fsw/fontxedit.zip) is a font file editor.   
This can be work on Windows 10.   
You can download FONTX Editor fron [here](http://elm-chan.org/fsw_e.html).   
![FontxEditor](https://user-images.githubusercontent.com/6020549/78731275-3b889800-797a-11ea-81ba-096dbf07c4b8.png)

---

# How to build your own font file.   
step1) download fontxedit.exe.   
step2) download fonts from https://github.com/fcambus/spleen.   
step3) import one of them into your fontxedit.exe.   
![FONTX-EDITTOR-1](https://user-images.githubusercontent.com/6020549/112736427-d7e5e900-8f95-11eb-80d5-11dd9df42903.jpg)
step4) adjust font size.   
![FONTX-EDITTOR-2](https://user-images.githubusercontent.com/6020549/112736434-e6cc9b80-8f95-11eb-8b8e-b523746c1c96.jpg)
step5) check font pattern.   
![FONTX-EDITTOR-3](https://user-images.githubusercontent.com/6020549/112736437-edf3a980-8f95-11eb-946a-93426b62c137.jpg)
step6) save as .fmt file from your fontedit.exe.   
![FONTX-EDITTOR-4](https://user-images.githubusercontent.com/6020549/112736443-095eb480-8f96-11eb-8516-b03e10aeadbe.jpg)
step7) upload your font file to PI.Your font file is put in fontx directory.   
step8) change font file   
```
if (sv.fontsize == 16) {
  strcat(fnameh,"fontx/16dot_your_font_file");
  strcat(fnamez,"fontx/ILGZ16XB.FNT");
} else if (sv.fontsize == 24) {
  strcat(fnameh,"fontx/24dot_your_font_file");
  strcat(fnamez,"fontx/ILGZ24XB.FNT");
} else if (sv.fontsize == 32) {
  strcat(fnameh,"fontx/32dot_your_font_file");
  strcat(fnamez,"fontx/ILGZ32XB.FNT");
}
```
step8) change character set   
```
if((cd = iconv_open("ISO_8859-1","utf-8")) == (iconv_t)-1){
    return 0;
}
```

---

# Font File Viewer   
```
$ cc -o dump dump.c fontx.c
$ ./dump fontx/ILGZ16XB.FNT 0x93FA
argc=3
fontFile=[fontx/ILGZ16XB.FNT]
Font width=16
Font height=16
Code flag=0
Number of code blocks=94
Block 0 start=8140 end=817e
Block 1 start=8180 end=81fc
Block 2 start=8240 end=827e
Block 3 start=8280 end=82fc
Block 4 start=8340 end=837e
Block 5 start=8380 end=83fc
Block 6 start=8440 end=847e
Block 7 start=8480 end=84fc
Block 8 start=8540 end=857e
Block 9 start=8580 end=85fc
Block 10 start=8640 end=867e
Block 11 start=8680 end=86fc
Block 12 start=8740 end=877e
Block 13 start=8780 end=87fc
Block 14 start=8840 end=887e
Block 15 start=8880 end=88fc
Block 16 start=8940 end=897e
Block 17 start=8980 end=89fc
Block 18 start=8a40 end=8a7e
Block 19 start=8a80 end=8afc
Block 20 start=8b40 end=8b7e
Block 21 start=8b80 end=8bfc
Block 22 start=8c40 end=8c7e
Block 23 start=8c80 end=8cfc
Block 24 start=8d40 end=8d7e
Block 25 start=8d80 end=8dfc
Block 26 start=8e40 end=8e7e
Block 27 start=8e80 end=8efc
Block 28 start=8f40 end=8f7e
Block 29 start=8f80 end=8ffc
Block 30 start=9040 end=907e
Block 31 start=9080 end=90fc
Block 32 start=9140 end=917e
Block 33 start=9180 end=91fc
Block 34 start=9240 end=927e
Block 35 start=9280 end=92fc
Block 36 start=9340 end=937e
Block 37 start=9380 end=93fc
Block 38 start=9440 end=947e
Block 39 start=9480 end=94fc
Block 40 start=9540 end=957e
Block 41 start=9580 end=95fc
Block 42 start=9640 end=967e
Block 43 start=9680 end=96fc
Block 44 start=9740 end=977e
Block 45 start=9780 end=97fc
Block 46 start=9840 end=987e
Block 47 start=9880 end=98fc
Block 48 start=9940 end=997e
Block 49 start=9980 end=99fc
Block 50 start=9a40 end=9a7e
Block 51 start=9a80 end=9afc
Block 52 start=9b40 end=9b7e
Block 53 start=9b80 end=9bfc
Block 54 start=9c40 end=9c7e
Block 55 start=9c80 end=9cfc
Block 56 start=9d40 end=9d7e
Block 57 start=9d80 end=9dfc
Block 58 start=9e40 end=9e7e
Block 59 start=9e80 end=9efc
Block 60 start=9f40 end=9f7e
Block 61 start=9f80 end=9ffc
Block 62 start=e040 end=e07e
Block 63 start=e080 end=e0fc
Block 64 start=e140 end=e17e
Block 65 start=e180 end=e1fc
Block 66 start=e240 end=e27e
Block 67 start=e280 end=e2fc
Block 68 start=e340 end=e37e
Block 69 start=e380 end=e3fc
Block 70 start=e440 end=e47e
Block 71 start=e480 end=e4fc
Block 72 start=e540 end=e57e
Block 73 start=e580 end=e5fc
Block 74 start=e640 end=e67e
Block 75 start=e680 end=e6fc
Block 76 start=e740 end=e77e
Block 77 start=e780 end=e7fc
Block 78 start=e840 end=e87e
Block 79 start=e880 end=e8fc
Block 80 start=e940 end=e97e
Block 81 start=e980 end=e9fc
Block 82 start=ea40 end=ea7e
Block 83 start=ea80 end=eafc
Block 84 start=eb40 end=eb7e
Block 85 start=eb80 end=ebfc
Block 86 start=ec40 end=ec7e
Block 87 start=ec80 end=ecfc
Block 88 start=ed40 end=ed7e
Block 89 start=ed80 end=edfc
Block 90 start=ee40 end=ee7e
Block 91 start=ee80 end=eefc
Block 92 start=ef40 end=ef7e
Block 93 start=ef80 end=effc
character code=0x93FA
code=93fa
GetFontx OK. code=93fa
00................
01................
02...**********...
03...*........*...
04...*........*...
05...*........*...
06...*........*...
07...**********...
08...*........*...
09...*........*...
10...*........*...
11...*........*...
12...**********...
13...*........*...
14................
15................
```

