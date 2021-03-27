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
- fontx/ILGH16XB.FNT // 8x16Dot Alphanumeric & Japanese Hankaku KATAKANA
- fontx/ILGH24XB.FNT // 12x24Dot Alphanumeric & Japanese Hankaku KATAKANA
- fontx/ILGH32XB.FNT // 16x32Dot Alphanumeric & Japanese Hankaku KATAKANA
- fontx/ILGZ16XB.FNT // 16x16Dot Japanese 2byte Code(SJIS, Not unicode)
- fontx/ILGZ24XB.FNT // 24x24Dot Japanese 2byte Code(SJIS, Not unicode)
- fontx/ILGZ32XB.FNT // 32x32Dot Japanese 2byte Code(SJIS, Not unicode)

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

# How to build your own font file   
You can change these fonts to display the characters of your country.   
- Default   
From 0x00 to 0x7f, the characters image of Alphanumeric are stored.   
From 0x80 to 0xff, the characters image of Japanese are stored.   
```
fontx/ILGH16XB.FNT // 8x16Dot Alphanumeric & Japanese Hankaku KATAKANA
fontx/ILGH24XB.FNT // 12x24Dot Alphanumeric & Japanese Hankaku KATAKANA
fontx/ILGH32XB.FNT // 16x32Dot Alphanumeric & Japanese Hankaku KATAKANA
```
0xCA is Japanese.   
![FONTX-EDITTOR-11](https://user-images.githubusercontent.com/6020549/112738034-b808f200-8fa2-11eb-9b91-5a937022d1c3.jpg)

- Localize   
From 0x00 to 0x7f, the characters image of Alphanumeric are stored.   
From 0x80 to 0xff, the characters image of your country are stored.   
```
fontx/XXXXX16.FNT // 8x16Dot Alphanumeric & Local characters
fontx/XXXXX24.FNT // 12x24Dot Alphanumeric & Local characters
fontx/XXXXX32.FNT // 16x32Dot Alphanumeric & Local characters
```
0xCA is LATIN.   
![FONTX-EDITTOR-12](https://user-images.githubusercontent.com/6020549/112738041-c48d4a80-8fa2-11eb-9d66-1424fc905dd3.jpg)

step1) download fontxedit.exe.   

step2) download fonts from https://github.com/fcambus/spleen.   

step3) import one of them into your fontxedit.exe.   
![FONTX-EDITTOR-1](https://user-images.githubusercontent.com/6020549/112736427-d7e5e900-8f95-11eb-80d5-11dd9df42903.jpg)

step4) adjust font size.   
![FONTX-EDITTOR-2](https://user-images.githubusercontent.com/6020549/112736434-e6cc9b80-8f95-11eb-8b8e-b523746c1c96.jpg)

step5) check font pattern.   
![FONTX-EDITTOR-3](https://user-images.githubusercontent.com/6020549/112736437-edf3a980-8f95-11eb-946a-93426b62c137.jpg)

step6) save as .fnt file from your fontedit.exe.   
![FONTX-EDITTOR-4](https://user-images.githubusercontent.com/6020549/112736443-095eb480-8f96-11eb-8516-b03e10aeadbe.jpg)

step7) upload your font file to PI.Your font file is put in fontx directory.   

---

# How to change display font   

step1) change font file   
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
step2) enable UTF to ISO   
By default,The 3-byte UTF code is converted to 2-byte SJIS code.   
SJIS is Japanese extended character set.   
The following changes will convert a 3-byte UTF code to a 1-byte ISO code.   
```
//sv.save[num].size = String2SJIS((unsigned char*)argv[2], strlen(argv[2]), sv.save[num].sjis, MAXCHAR);
sv.save[num].size = String2ISO((unsigned char*)argv[2], strlen(argv[2]), sv.save[num].sjis, MAXCHAR);
```

step3) change character set   
"ISO_8859-1" will convert 3-byte UTF code to 1-byte ISO code.    
Please change to the character set of your country.   
```
if((cd = iconv_open("ISO_8859-1","utf-8")) == (iconv_t)-1){
    return 0;
}
```

Picuture by Olaf Sabatschus.   
![image](https://user-images.githubusercontent.com/7696953/112719224-25137d80-8ef8-11eb-8fd2-cdff49838ff6.png)

More detail is [here](https://github.com/nopnop2002/wiringpi-tft-tool/issues/7).

---

# Font File Viewer   
```
$ cc -o dump dump.c fontx.c
$ ./dump fontx/font12x24.fnt 0xfc
argc=3
fontFile=[fontx/font12x24.fnt]
Font width=12
Font height=24
Code flag=1
Number of code blocks=0
character code=0xfc
code=fc
GetFontx OK. code=fc
00............
01............
02............
03..**....**..
04..**....**..
05............
06............
07............
08.**......**.
09.**......**.
10.**......**.
11.**......**.
12.**......**.
13.**......**.
14.**......**.
15.**......**.
16.**......**.
17..**.....**.
18...********.
19............
20............
21............
22............
23............
```

