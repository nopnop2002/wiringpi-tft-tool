#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#ifdef SPI
#include "spilib.h"
#endif
#ifndef SPI
#include "8bitlib.h"
#endif

#define MAXLINE 8
#define MAXCHAR 32
#define _DEBUG_ 0

uint16_t _FONT_DIRECTION_;
uint16_t _FONT_FILL_;
uint16_t _FONT_FILL_COLOR_;
uint16_t _FONT_UNDER_LINE_;
uint16_t _FONT_UNDER_LINE_COLOR_;

typedef struct {
  uint8_t colum;
  uint16_t fcolor;
  uint16_t bcolor;
  uint8_t enhance;
  uint8_t size;
  uint16_t sjis[MAXCHAR];
} SaveData;

typedef struct {
  uint16_t basecolor;
  uint16_t fontsize;
  uint16_t direction;
  SaveData save[MAXLINE];
} SaveFrame;

void usage(char *prog);
void InitSaveData(SaveFrame *hoge);
void DumpSaveFrame(SaveFrame hoge);
int ReadTFTConfig(char *path, int *width, int *height);
int ReadPinConfig(TFTPin *pins, char *path);

int main(int argc, char **argv){
  int i,j,k;
  int XMAX,YMAX;
  char dir[128];
  char cpath[128];
  char dpath[128];
  char spath[128];
  char ppath[128];
  FILE *fp;
  SaveFrame sv;
  
if(_DEBUG_)  printf("argv[0]=%s\n",argv[0]);
  strcpy(dir, argv[0]);
  for(i=strlen(dir);i>0;i--) {
    if (dir[i-1] == '/') {
      dir[i] = 0;
      break;
    }
  }
if(_DEBUG_)  printf("dir=%s\n",dir);
  strcpy(dpath,dir);
  strcat(dpath,"tft.data");
if(_DEBUG_)  printf("dpath=%s\n",dpath);
  strcpy(cpath,dir);
  strcat(cpath,"tft.conf");
if(_DEBUG_)  printf("cpath=%s\n",cpath);
  strcpy(spath,dir);
  strcat(spath,"spi.conf");
if(_DEBUG_)  printf("spath=%s\n",spath);
  strcpy(ppath,dir);
  strcat(ppath,"pin.conf");
if(_DEBUG_)  printf("ppath=%s\n",ppath);

  if (ReadTFTConfig(cpath,&XMAX,&YMAX) == 0) {
    printf("%s Not found\n",cpath);
    return 0;
  }
if(_DEBUG_)  printf("ReadTFTConfig:XMAX=%d YMAX=%d\n",XMAX,YMAX);

#ifdef SPI
  TFTPin pins;
  pins.rst = 9;
  pins.rs  = 8;
  ReadPinConfig(&pins, spath);
if(_DEBUG_)printf("rst=%d cs=%d\n", pins.rst,pins.rs);
#endif

#ifndef SPI
  TFTPin pins;
  pins.rst = 7;
  pins.cs  = 8;
  pins.rs  = 9;
  pins.wr  = 11;
  pins.rd  = 31;
  pins.d0  = 21;
  pins.d1  = 22;
  pins.d2  = 23;
  pins.d3  = 24;
  pins.d4  = 25;
  pins.d5  = 26;
  pins.d6  = 27;
  pins.d7  = 28;
  ReadPinConfig(&pins, ppath);
if(_DEBUG_)printf("rst=%d cs=%d rs=%d wr=%d rd=%d\n",
  pins.rst,pins.cs,pins.rs,pins.wr,pins.rd);
if(_DEBUG_)printf("d0=%d d1=%d d2=%d d4=%d d4=%d d5=%d d6=%d d7=%d\n",
  pins.d0,pins.d1,pins.d2,pins.d3,pins.d4,pins.d5,pins.d6,pins.d7);
#endif


  struct stat stat_buf;
  if (stat(dpath,&stat_buf) == 0) {
if(_DEBUG_)printf("%s found\n",dpath);
    fp = fopen(dpath,"rb");
    fread(&sv,sizeof(sv),1,fp);
    fclose(fp);
  } else {
if(_DEBUG_)printf("%s not found\n",dpath);
    memset(&sv,0,sizeof(sv));
    InitSaveData(&sv);
  }
if(_DEBUG_)DumpSaveFrame(sv);

  if (argc == 1) {
    usage(basename(argv[0]));
    return 0;
  }

  char numc[5];
  int num;

  if ( (strcmp(argv[1],"+1") == 0) ||
       (strcmp(argv[1],"+2") == 0) ||
       (strcmp(argv[1],"+3") == 0) ||
       (strcmp(argv[1],"+4") == 0) ||
       (strcmp(argv[1],"+5") == 0) ||
       (strcmp(argv[1],"+6") == 0) ||
       (strcmp(argv[1],"+7") == 0) ||
       (strcmp(argv[1],"+8") == 0) ) {
    strcpy(numc,argv[1]);
    num = (numc[1] - '0') - 1;
      
if(_DEBUG_)printf("add string to line [%d]\n",num);
    sv.save[num].size = String2SJIS(argv[2], strlen(argv[2]), sv.save[num].sjis, MAXCHAR);
if(_DEBUG_)DumpSaveFrame(sv);
    fp = fopen(dpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( (strcmp(argv[1],"-1") == 0) ||
       (strcmp(argv[1],"-2") == 0) ||
       (strcmp(argv[1],"-3") == 0) ||
       (strcmp(argv[1],"-4") == 0) ||
       (strcmp(argv[1],"-5") == 0) ||
       (strcmp(argv[1],"-6") == 0) ||
       (strcmp(argv[1],"-7") == 0) ||
       (strcmp(argv[1],"-8") == 0) ) {
    strcpy(numc,argv[1]);
    num = (numc[1] - '0') - 1;
if(_DEBUG_)printf("delete string to line [%d]\n",num);
    sv.save[num].size = 0;
if(_DEBUG_)DumpSaveFrame(sv);
    fp = fopen(dpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( (strcmp(argv[1],"16") == 0) ||
       (strcmp(argv[1],"24") == 0) ||
       (strcmp(argv[1],"32") == 0) ) {
    if (strcmp(argv[1],"16") == 0) sv.fontsize = 16;
    if (strcmp(argv[1],"24") == 0) sv.fontsize = 24;
    if (strcmp(argv[1],"32") == 0) sv.fontsize = 32;
if(_DEBUG_)printf("set font size=%d\n",sv.fontsize);
    fp = fopen(dpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( (strcmp(argv[1],"R0") == 0) ||
       (strcmp(argv[1],"R90") == 0) ||
       (strcmp(argv[1],"R180") == 0) ||
       (strcmp(argv[1],"R270") == 0) ) {
    if (strcmp(argv[1],"R0") == 0) sv.direction = DIRECTION0;
    if (strcmp(argv[1],"R90") == 0) sv.direction = DIRECTION90;
    if (strcmp(argv[1],"R180") == 0) sv.direction = DIRECTION180;
    if (strcmp(argv[1],"R270") == 0) sv.direction = DIRECTION270;
if(_DEBUG_)printf("set direction=%d\n",sv.direction);
    fp = fopen(dpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( (strcmp(argv[1],"B0") == 0) ) {
    if (strcmp(argv[2],"RED") == 0) sv.basecolor = RED;
    if (strcmp(argv[2],"GREEN") == 0) sv.basecolor = GREEN;
    if (strcmp(argv[2],"BLUE") == 0) sv.basecolor = BLUE;
    if (strcmp(argv[2],"BLACK") == 0) sv.basecolor = BLACK;
    if (strcmp(argv[2],"WHITE") == 0) sv.basecolor = WHITE;
    if (strcmp(argv[2],"GRAY") == 0) sv.basecolor = GRAY;
    if (strcmp(argv[2],"YELLOW") == 0) sv.basecolor = YELLOW;
    if (strcmp(argv[2],"CYAN") == 0) sv.basecolor = CYAN;
    if (strcmp(argv[2],"PURPLE") == 0) sv.basecolor = PURPLE;
if(_DEBUG_)printf("set base color=%x\n",sv.basecolor);
    fp = fopen(dpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( (strcmp(argv[1],"F1") == 0) ||
       (strcmp(argv[1],"F2") == 0) || 
       (strcmp(argv[1],"F3") == 0) || 
       (strcmp(argv[1],"F4") == 0) || 
       (strcmp(argv[1],"F5") == 0) || 
       (strcmp(argv[1],"F6") == 0) || 
       (strcmp(argv[1],"F7") == 0) || 
       (strcmp(argv[1],"F8") == 0) ) {
    strcpy(numc,argv[1]);
    num = (numc[1] - '0') - 1;
    if (strcmp(argv[2],"RED") == 0) sv.save[num].fcolor = RED;
    if (strcmp(argv[2],"GREEN") == 0) sv.save[num].fcolor = GREEN;
    if (strcmp(argv[2],"BLUE") == 0) sv.save[num].fcolor = BLUE;
    if (strcmp(argv[2],"BLACK") == 0) sv.save[num].fcolor = BLACK;
    if (strcmp(argv[2],"WHITE") == 0) sv.save[num].fcolor = WHITE;
    if (strcmp(argv[2],"GRAY") == 0) sv.save[num].fcolor = GRAY;
    if (strcmp(argv[2],"YELLOW") == 0) sv.save[num].fcolor = YELLOW;
    if (strcmp(argv[2],"CYAN") == 0) sv.save[num].fcolor = CYAN;
    if (strcmp(argv[2],"PURPLE") == 0) sv.save[num].fcolor = PURPLE;
if(_DEBUG_)printf("set fcolor to line [%d]=%x\n",num,sv.save[num].fcolor);
    fp = fopen(dpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( (strcmp(argv[1],"B1") == 0) ||
       (strcmp(argv[1],"B2") == 0) ||
       (strcmp(argv[1],"B3") == 0) ||
       (strcmp(argv[1],"B4") == 0) ||
       (strcmp(argv[1],"B5") == 0) ||
       (strcmp(argv[1],"B6") == 0) ||
       (strcmp(argv[1],"B7") == 0) ||
       (strcmp(argv[1],"B8") == 0) ) {
    strcpy(numc,argv[1]);
    num = (numc[1] - '0') - 1;
    if (strcmp(argv[2],"RED") == 0) sv.save[num].bcolor = RED;
    if (strcmp(argv[2],"GREEN") == 0) sv.save[num].bcolor = GREEN;
    if (strcmp(argv[2],"BLUE") == 0) sv.save[num].bcolor = BLUE;
    if (strcmp(argv[2],"BLACK") == 0) sv.save[num].bcolor = BLACK;
    if (strcmp(argv[2],"WHITE") == 0) sv.save[num].bcolor = WHITE;
    if (strcmp(argv[2],"GRAY") == 0) sv.save[num].bcolor = GRAY;
    if (strcmp(argv[2],"YELLOW") == 0) sv.save[num].bcolor = YELLOW;
    if (strcmp(argv[2],"CYAN") == 0) sv.save[num].bcolor = CYAN;
    if (strcmp(argv[2],"PURPLE") == 0) sv.save[num].bcolor = PURPLE;
if(_DEBUG_)printf("set bcolor to line [%d]=%x\n",num,sv.save[num].bcolor);
    fp = fopen(dpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( (strcmp(argv[1],"+U") == 0) ||
       (strcmp(argv[1],"-U") == 0) ) {
    strcpy(numc,argv[2]);
    num = (numc[0] - '0') - 1;
if(_DEBUG_)printf("set/unset underline to line [%d]\n",num);
    if (num < 0 || num > (MAXLINE+1)) return 0;
    if (strcmp(argv[1],"+U") == 0) sv.save[num].enhance = 1;
    if (strcmp(argv[1],"-U") == 0) sv.save[num].enhance = 0;
    fp = fopen(dpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( strcmp(argv[1],"+L") == 0) {
    memcpy(&sv.save[0],&sv.save[1],sizeof(SaveData));
    memcpy(&sv.save[1],&sv.save[2],sizeof(SaveData));
    memcpy(&sv.save[2],&sv.save[3],sizeof(SaveData));
    memcpy(&sv.save[3],&sv.save[4],sizeof(SaveData));
    memcpy(&sv.save[4],&sv.save[5],sizeof(SaveData));
    memcpy(&sv.save[5],&sv.save[6],sizeof(SaveData));
    memcpy(&sv.save[6],&sv.save[7],sizeof(SaveData));
    memset(&sv.save[7],0,sizeof(SaveData));
    fp = fopen(dpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( strcmp(argv[1],"-L") == 0) {
    memcpy(&sv.save[7],&sv.save[6],sizeof(SaveData));
    memcpy(&sv.save[6],&sv.save[5],sizeof(SaveData));
    memcpy(&sv.save[5],&sv.save[4],sizeof(SaveData));
    memcpy(&sv.save[4],&sv.save[3],sizeof(SaveData));
    memcpy(&sv.save[3],&sv.save[2],sizeof(SaveData));
    memcpy(&sv.save[2],&sv.save[1],sizeof(SaveData));
    memcpy(&sv.save[1],&sv.save[0],sizeof(SaveData));
    memset(&sv.save[0],0,sizeof(SaveData));
    fp = fopen(dpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if ( (strcmp(argv[1],"P1") == 0) ||
       (strcmp(argv[1],"P2") == 0) ||
       (strcmp(argv[1],"P3") == 0) ||
       (strcmp(argv[1],"P4") == 0) ||
       (strcmp(argv[1],"P5") == 0) ||
       (strcmp(argv[1],"P6") == 0) ||
       (strcmp(argv[1],"P7") == 0) ||
       (strcmp(argv[1],"P8") == 0) ) {
    strcpy(numc,argv[1]);
    num = (numc[1] - '0') - 1;
    strcpy(numc,argv[2]);
    int col;
    col = atoi(numc) - 1;
    if (col < 0 || col > 16) return 0;
if(_DEBUG_)printf("set start colum to line [%d] = %d\n",num,col);
    sv.save[num].colum = col;
    fp = fopen(dpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if (strcmp(argv[1],"r") == 0) {
    memset(&sv,0,sizeof(sv));
    InitSaveData(&sv);
if(_DEBUG_)DumpSaveFrame(sv);
    fp = fopen(dpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if (strcmp(argv[1],"s") == 0) {
if(_DEBUG_)printf("show dislay\n");
    char fnameh[64];
    char fnamez[64];
    FontxFile fx[2];
    strcpy(fnameh,dir);
    strcpy(fnamez,dir);
if(_DEBUG_)printf("sv.fontsize=%d\n",sv.fontsize);
    if (sv.fontsize == 16) {
      strcat(fnameh,"fontx/ILGH16XB.FNT");
      strcat(fnamez,"fontx/ILGZ16XB.FNT");
    } else if (sv.fontsize == 24) {
      strcat(fnameh,"fontx/ILGH24XB.FNT");
      strcat(fnamez,"fontx/ILGZ24XB.FNT");
    } else if (sv.fontsize == 32) {
      strcat(fnameh,"fontx/ILGH32XB.FNT");
      strcat(fnamez,"fontx/ILGZ32XB.FNT");
    }
if(_DEBUG_)printf("fnameh=%s\nfnamez=%s\n",fnameh,fnamez);
    Fontx_init(fx,fnameh,fnamez);

#ifdef SPI
    lcdInit(XMAX, YMAX, pins);
    #define MODEL "SPI"
#endif

#ifdef ILI9325
    lcdInit(0x9325, XMAX, YMAX, pins);
    #define MODEL "ILI9325"
#endif

#ifdef ILI9327
    lcdInit(0x9327, XMAX, YMAX, pins);
    #define MODEL "ILI9327"
#endif

#ifdef ILI9341
    lcdInit(0x9341, XMAX, YMAX, pins);
    #define MODEL "ILI9341"
#endif

#ifdef ILI9342
    lcdInit(0x9342, XMAX, YMAX, pins);
    #define MODEL "ILI9342"
#endif

#ifdef ILI9481
    lcdInit(0x9481, XMAX, YMAX, pins);
    #define MODEL "ILI9481"
#endif

#ifdef SPFD5408
    lcdInit(0x5408, XMAX, YMAX, pins);
    #define MODEL "SPFD5408"
#endif

#ifdef S6D1121
    lcdInit(0x1121, XMAX, YMAX, pins);
    #define MODEL "S6D1121"
#endif

#ifdef R61505U
    lcdInit(0x1505, XMAX, YMAX, pins);
    #define MODEL "R61505U"
#endif

#ifdef R61509V
    lcdInit(0xB509, XMAX, YMAX, pins);
    #define MODEL "R61509V"
#endif

    lcdReset();
    lcdSetup();
    lcdFillScreen(sv.basecolor);
    lcdSetFontDirection(sv.direction);

    uint16_t xpos,ypos;

#if 0
  uint16_t color;
  xpos = 0;
  ypos = (YMAX -1)-(16*1);
  color = BLACK;
  xpos = lcdDrawSJISChar(fx16, xpos, ypos, 0x30, color);
#endif

    for (num=0;num<MAXLINE;num++) {
      if (sv.save[num].size == 0) continue;
      if (sv.direction == DIRECTION0) {
        xpos = sv.save[num].colum * sv.fontsize;
        ypos = (YMAX-1)-(sv.fontsize*(num+1));
      } else if (sv.direction == DIRECTION90) {
        xpos = (XMAX-1)-(sv.fontsize*(num+1));
        ypos = (YMAX-1)-(sv.save[num].colum * sv.fontsize);
      } else if (sv.direction == DIRECTION180) {
        xpos = (XMAX-1) - sv.save[num].colum * sv.fontsize;
        ypos = sv.fontsize*(num+1);
      } else if (sv.direction == DIRECTION270) {
        xpos = sv.fontsize*(num+1);
        ypos = sv.save[num].colum * sv.fontsize;
      }
if(_DEBUG_)printf("xpos=%d ypos=%d\n",xpos,ypos);
      lcdUnsetFontFill();
      lcdUnsetFontUnderLine();
      if (sv.save[num].bcolor) lcdSetFontFill(sv.save[num].bcolor);
      if (sv.save[num].enhance) lcdSetFontUnderLine(sv.save[num].fcolor);

      for (i=0;i<sv.save[num].size;i++) {
        if (sv.direction == DIRECTION0) {
          xpos = lcdDrawSJISChar(fx, xpos, ypos, sv.save[num].sjis[i], sv.save[num].fcolor);
        } else if (sv.direction == DIRECTION90) {
          ypos = lcdDrawSJISChar(fx, xpos, ypos, sv.save[num].sjis[i], sv.save[num].fcolor);
        } else if (sv.direction == DIRECTION180) {
          xpos = lcdDrawSJISChar(fx, xpos, ypos, sv.save[num].sjis[i], sv.save[num].fcolor);
        } else if (sv.direction == DIRECTION270) {
          ypos = lcdDrawSJISChar(fx, xpos, ypos, sv.save[num].sjis[i], sv.save[num].fcolor);
        }
if(_DEBUG_)printf("xpos(2)=%d ypos(2)=%d\n",xpos,ypos);
      }
    }
  }

  if (strcmp(argv[1],"D") == 0) {
    DumpSaveFrame(sv);
  }

  if (strcmp(argv[1],"M") == 0) {
    printf("%s(%3dx%3d)\n",MODEL,XMAX,YMAX);
  }

  if (strcmp(argv[1],"P") == 0) {
#ifdef SPI
    printf("RST=%d\n",pins.rst);
    printf("RS =%d\n",pins.rs);
#endif

#ifndef SPI
    printf("RST=%d\n",pins.rst);
    printf("CS =%d\n",pins.cs);
    printf("RS =%d\n",pins.rs);
    printf("WR =%d\n",pins.wr);
    printf("RD =%d\n",pins.rd);
    printf("D0 =%d\n",pins.d0);
    printf("D1 =%d\n",pins.d1);
    printf("D2 =%d\n",pins.d2);
    printf("D3 =%d\n",pins.d3);
    printf("D4 =%d\n",pins.d4);
    printf("D5 =%d\n",pins.d5);
    printf("D6 =%d\n",pins.d6);
    printf("D7 =%d\n",pins.d7);
#endif
  }

}


/*
Show Usage
*/

void usage(char *prog){
  printf("Usage : %s option\n",prog);
  printf("Options:\n");
  printf("  s             show display\n");
  printf("  r             remove all string\n");
  printf("  +n string     add string to line#n\n");
  printf("  -n            delete string from line#n\n");
  printf("  Fn COLOR      set fore ground color to line#n\n");
  printf("  Bn COLOR      set back ground color to line#n\n");
  printf("  B0 COLOR      set base color(Default is WHITE)\n");
  printf("     RED/GREEN/BLUE/BLACK/WHITE/GRAY/YELLOW/CYAN/PURPLE\n");
  printf("  +U n          set underline to line#n\n");
  printf("  -U n          unset underline to line#n\n");
  printf("  +L            line scroll up\n");
  printf("  -L            line scroll down\n");
  printf("  Pn c          set start colum c to line#n\n");
  printf("  16            draw 16 dot fonts(Default)\n");
  printf("  24            draw 24 dot fonts\n");
  printf("  32            draw 32 dot fonts\n");
  printf("  R0            set angle 0(Default)\n");
  printf("  R90           set angle 90\n");
  printf("  R180          set angle 180\n");
  printf("  R270          set angle 270\n");
}

void InitSaveData(SaveFrame *hoge) {
  hoge->basecolor = WHITE;
  hoge->fontsize  = 16;
  hoge->direction  = 0;
}

void DumpSaveFrame(SaveFrame hoge) {
  int i,j;
  
  printf("basecolor=%x\n",hoge.basecolor);
  printf("fontsize=%d\n",hoge.fontsize);
  printf("direction=%d\n",hoge.direction);
  for(i=0;i<MAXLINE;i++) {
    printf("[%d].colum=%d ",i,hoge.save[i].colum);
    printf("[%d].fcolor=%x ",i,hoge.save[i].fcolor);
    printf("[%d].bcolor=%x ",i,hoge.save[i].bcolor);
    printf("[%d].enhance=%d",i,hoge.save[i].enhance);
    printf("[%d].size=%d\n",i,hoge.save[i].size);
    for(j=0;j<hoge.save[i].size;j++) {
      printf("[%d].sjis[%d]=%x\n",i,j,hoge.save[i].sjis[j]);
    }
  }

}

int ReadTFTConfig(char *path, int *width, int *height) {
  FILE *fp;
  char buff[128];
  
//  printf("path=%s\n",path);
  fp = fopen(path,"r");
  if(fp == NULL) return 0;
  while (fgets(buff,128,fp) != NULL) {
//    printf("buf=%s\n",buff);
//    printf("buff[0]=%x\n",buff[0]);
    if (buff[0] == '#') continue;
    if (buff[0] == 0x0a) continue;
    sscanf(buff, "width=%d height=%d",width,height);
//    printf("width=%d height=%d\n",*width,*height);
  }
  fclose(fp);
  return 1;
}

int ReadPinConfig(TFTPin *pin, char *path) {
  FILE *fp;
  char buff[128];
  int wk;
  
//  printf("path=%s\n",path);
  fp = fopen(path,"r");
  if(fp == NULL) return 0;
  while (fgets(buff,128,fp) != NULL) {
//    printf("buf=%s\n",buff);
//    printf("buff[0]=%x\n",buff[0]);
    if (buff[0] == '#') continue;
    if (buff[0] == 0x0a) continue;
    if (strncmp(buff,"RST=",4) == 0) {
      sscanf(buff, "RST=%d", &(pin->rst));
    } else if (strncmp(buff,"RS=",3) == 0) {
      sscanf(buff, "RS=%d", &(pin->rs));
    }

#ifndef SPI
    if (strncmp(buff,"CS=",3) == 0) {
      sscanf(buff, "CS=%d", &(pin->cs));
    } else if (strncmp(buff,"WR=",3) == 0) {
      sscanf(buff, "WR=%d", &(pin->wr));
    } else if (strncmp(buff,"RD=",3) == 0) {
      sscanf(buff, "RD=%d", &(pin->rd));
    } else if (strncmp(buff,"D0=",3) == 0) {
      sscanf(buff, "D0=%d", &(pin->d0));
    } else if (strncmp(buff,"D1=",3) == 0) {
      sscanf(buff, "D1=%d", &(pin->d1));
    } else if (strncmp(buff,"D2=",3) == 0) {
      sscanf(buff, "D2=%d", &(pin->d2));
    } else if (strncmp(buff,"D3=",3) == 0) {
      sscanf(buff, "D3=%d", &(pin->d3));
    } else if (strncmp(buff,"D4=",3) == 0) {
      sscanf(buff, "D4=%d", &(pin->d4));
    } else if (strncmp(buff,"D5=",3) == 0) {
      sscanf(buff, "D5=%d", &(pin->d5));
    } else if (strncmp(buff,"D6=",3) == 0) {
      sscanf(buff, "D6=%d", &(pin->d6));
    } else if (strncmp(buff,"D7=",3) == 0) {
      sscanf(buff, "D7=%d", &(pin->d7));
    }
#endif

  }
  fclose(fp);
  return 1;

}
