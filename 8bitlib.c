#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <wiringPi.h>
#include "8bitlib.h"

#define _DEBUG_ 0

uint16_t _FONT_DIRECTION_;
uint16_t _FONT_FILL_;
uint16_t _FONT_FILL_COLOR_;
uint16_t _FONT_UNDER_LINE_;
uint16_t _FONT_UNDER_LINE_COLOR_;

uint16_t _model;
int16_t _width;
int16_t _height;
int16_t _offsetx;
int16_t _offsety;
uint16_t _interface;

int16_t LCD_RST;
int16_t LCD_CS;
int16_t LCD_RS;
int16_t LCD_WR;
int16_t LCD_RD;

int16_t LCD_D0;
int16_t LCD_D1;
int16_t LCD_D2;
int16_t LCD_D3;
int16_t LCD_D4;
int16_t LCD_D5;
int16_t LCD_D6;
int16_t LCD_D7;

void lcdWriteByte(uint8_t data) {
	digitalWrite(LCD_D0, data & 1);
	digitalWrite(LCD_D1, (data & 2) >> 1);
	digitalWrite(LCD_D2, (data & 4) >> 2);
	digitalWrite(LCD_D3, (data & 8) >> 3);
	digitalWrite(LCD_D4, (data & 16) >> 4); 
	digitalWrite(LCD_D5, (data & 32) >> 5);
	digitalWrite(LCD_D6, (data & 64) >> 6);
	digitalWrite(LCD_D7, (data & 128) >> 7);	

	digitalWrite(LCD_WR, LOW);
	//delayMicroseconds(10);
	digitalWrite(LCD_WR, HIGH);
}

void lcdWriteDataWord(uint16_t data) {
	digitalWrite(LCD_CS, LOW);
	digitalWrite(LCD_RS, HIGH);
	digitalWrite(LCD_RD, HIGH);
	digitalWrite(LCD_WR, HIGH);
	lcdWriteByte(data >> 8);
	lcdWriteByte(data);
	digitalWrite(LCD_CS, HIGH);  
}

void lcdWriteDataByte(uint8_t data) {
	digitalWrite(LCD_CS, LOW);
	digitalWrite(LCD_RS, HIGH);
	digitalWrite(LCD_RD, HIGH);
	digitalWrite(LCD_WR, HIGH);
	lcdWriteByte(data);
	digitalWrite(LCD_CS, HIGH);  
}

void lcdWriteCommandWord(uint16_t command) {
	digitalWrite(LCD_CS, LOW);
	digitalWrite(LCD_RS, LOW);
	digitalWrite(LCD_RD, HIGH);
	digitalWrite(LCD_WR, HIGH);  
	lcdWriteByte(command >> 8);
	lcdWriteByte(command);
	digitalWrite(LCD_CS, HIGH);		 
}

void lcdWriteCommandByte(uint8_t command) {
	digitalWrite(LCD_CS, LOW);
	digitalWrite(LCD_RS, LOW);
	digitalWrite(LCD_RD, HIGH);
	digitalWrite(LCD_WR, HIGH);
	lcdWriteByte(command);
	digitalWrite(LCD_CS, HIGH);		 
}


void lcdWriteRegisterWord(uint16_t addr, uint16_t data) {
	lcdWriteCommandWord(addr);
	lcdWriteDataWord(data);
}

void lcdWriteRegisterByte(uint8_t addr, uint16_t data) {
	lcdWriteCommandByte(addr);
	lcdWriteDataWord(data);
}


#define TFTLCD_DELAY 0xFFFF
#define TFTLCD_DELAY16 0xFFFF
#define TFTLCD_DELAY8 0x7F

void init_table(const void *table, int16_t size)
{
	int i;
	uint8_t *p = (uint8_t *) table;
	while (size > 0) {
		uint8_t cmd = *p++;
		uint8_t len = *p++;
		if (cmd == TFTLCD_DELAY8) {
			delay(len);
			len = 0;
		} else {
			lcdWriteCommandByte( cmd );
			for (i = 0; i < len; i++) {
				uint8_t data = *p++;
				lcdWriteDataByte( data );
			}
		}
		size -= len + 2;
	}
}

void init_table16(const void *table, int16_t size)
{
	uint16_t *p = (uint16_t *) table;
	while (size > 0) {
		uint16_t cmd = *p++;
		uint16_t dat = *p++;
		if (cmd == TFTLCD_DELAY)
			delay(dat);
		else {
			lcdWriteRegisterWord(cmd, dat);
		}
		size -= 2 * sizeof(int16_t);
	}
}

void lcdInit(uint16_t model, int16_t width, int16_t height, int offsetx, int offsety, TFTPin pin)
{
	_model = model;
	_width = width;
	_height = height;
	_offsetx = offsetx;
	_offsety = offsety;

	LCD_RST = pin.rst;
	LCD_CS	= pin.cs;
	LCD_RS	= pin.rs;
	LCD_WR	= pin.wr;
	LCD_RD	= pin.rd;

	LCD_D0	= pin.d0;
	LCD_D1	= pin.d1;
	LCD_D2	= pin.d2;
	LCD_D3	= pin.d3;
	LCD_D4	= pin.d4;
	LCD_D5	= pin.d5;
	LCD_D6	= pin.d6;
	LCD_D7	= pin.d7;

	if(wiringPiSetup() == -1) {
		printf("Setup Fail\n");
		return;
	}

	pinMode(LCD_CS, OUTPUT);
	pinMode(LCD_RS, OUTPUT);
	pinMode(LCD_WR, OUTPUT);
	pinMode(LCD_RD, OUTPUT);
	pinMode(LCD_RST, OUTPUT);

	pinMode(LCD_D0, OUTPUT);
	pinMode(LCD_D1, OUTPUT);
	pinMode(LCD_D2, OUTPUT);
	pinMode(LCD_D3, OUTPUT);	
	pinMode(LCD_D4, OUTPUT);	
	pinMode(LCD_D5, OUTPUT);
	pinMode(LCD_D6, OUTPUT);
	pinMode(LCD_D7, OUTPUT);	

	digitalWrite(LCD_CS, HIGH);
	digitalWrite(LCD_RS, HIGH);
	digitalWrite(LCD_WR, HIGH);
	digitalWrite(LCD_RD, HIGH);
	digitalWrite(LCD_RST, HIGH);	

	_FONT_DIRECTION_ = DIRECTION0;
	_FONT_FILL_ = false;
	_FONT_UNDER_LINE_ = false;
}

void lcdReset(void)
{
	digitalWrite(LCD_RST, LOW);
	delay(100); 
	digitalWrite(LCD_RST, HIGH);
	delay(100);
}


void lcdSetup(void)
{
	static const uint8_t reset_off[] = {
		0x01, 0,						//Soft Reset
		TFTLCD_DELAY8, 150, 
		0x28, 0,						//Display Off
		0x3A, 1, 0x55,			//Pixel read=565, write=565.
	};

	static const uint8_t wake_on[] = {
		0x11, 0,						//Sleep Out
		TFTLCD_DELAY8, 150,
		0x29, 0,						//Display On
	};

	if (_model == 0x9225) {
		static const uint16_t ILI9225_regValues[] = {
			/* Set SS bit and direction output from S528 to S1 */
			0x10, 0x0000, // Set SAP,DSTB,STB
			0x11, 0x0000, // Set APON,PON,AON,VCI1EN,VC
			0x12, 0x0000, // Set BT,DC1,DC2,DC3
			0x13, 0x0000, // Set GVDD
			0x14, 0x0000, // Set VCOMH/VCOML voltage
			TFTLCD_DELAY16, 40,

			// Power-on sequence
			0x11, 0x0018, // Set APON,PON,AON,VCI1EN,VC
			0x12, 0x6121, // Set BT,DC1,DC2,DC3
			0x13, 0x006F, // Set GVDD	/*007F 0088 */
			0x14, 0x495F, // Set VCOMH/VCOML voltage
			0x10, 0x0800, // Set SAP,DSTB,STB
			TFTLCD_DELAY16, 10,
			0x11, 0x103B, // Set APON,PON,AON,VCI1EN,VC
			TFTLCD_DELAY16, 50,

			//0x01, 0x011C, // set the display line number and display direction
			0x01, 0x021C, // set the display line number and display direction
			0x02, 0x0100, // set 1 line inversion
			0x03, 0x1030, // set GRAM write direction and BGR=1.
			0x07, 0x0000, // Display off
			0x08, 0x0808, // set the back porch and front porch
			0x0B, 0x1100, // set the clocks number per line
			0x0C, 0x0000, // CPU interface
			0x0F, 0x0D01, // Set Osc  /*0e01*/
			0x15, 0x0020, // Set VCI recycling
			0x20, 0x0000, // Horizontal GRAM Address Set
			0x21, 0x0000, // Vertical GRAM Address Set

			/* Set GRAM area */
			0x30, 0x0000,
			0x31, 0x00DB,
			0x32, 0x0000,
			0x33, 0x0000,
			0x34, 0x00DB,
			0x35, 0x0000,
			0x36, 0x00AF,
			0x37, 0x0000,
			0x38, 0x00DB,
			0x39, 0x0000,

			/* Set GAMMA curve */
			0x50, 0x0000,
			0x51, 0x0808,
			0x52, 0x080A,
			0x53, 0x000A,
			0x54, 0x0A08,
			0x55, 0x0808,
			0x56, 0x0000,
			0x57, 0x0A00,
			0x58, 0x0710,
			0x59, 0x0710,

			0x07, 0x0012,
			TFTLCD_DELAY16, 50,
			0x07, 0x1017,
		};
		init_table16(ILI9225_regValues, sizeof(ILI9225_regValues));
		_interface = 0x9325;

	// ILI9320
	} else if (_model == 0x9320) {
		static const uint16_t ILI9320_regValues[] = {
			0x00e5, 0x8000,
			0x0000, 0x0001,
			0x0001, 0x100,
			0x0002, 0x0700,
			0x0003, 0x1030,
			0x0004, 0x0000,
			0x0008, 0x0202,
			0x0009, 0x0000,
			0x000A, 0x0000,
			0x000C, 0x0000,
			0x000D, 0x0000,
			0x000F, 0x0000,
			//-----Power On sequence-----------------------
			0x0010, 0x0000,
			0x0011, 0x0007,
			0x0012, 0x0000,
			0x0013, 0x0000,
			TFTLCD_DELAY, 50,
			0x0010, 0x17B0,  //SAP=1, BT=7, APE=1, AP=3
			0x0011, 0x0007,  //DC1=0, DC0=0, VC=7
			TFTLCD_DELAY, 10,
			0x0012, 0x013A,  //VCMR=1, PON=3, VRH=10
			TFTLCD_DELAY, 10,
			0x0013, 0x1A00,  //VDV=26
			0x0029, 0x000c,  //VCM=12
			TFTLCD_DELAY, 10,
			//-----Gamma control-----------------------
			0x0030, 0x0000,
			0x0031, 0x0505,
			0x0032, 0x0004,
			0x0035, 0x0006,
			0x0036, 0x0707,
			0x0037, 0x0105,
			0x0038, 0x0002,
			0x0039, 0x0707,
			0x003C, 0x0704,
			0x003D, 0x0807,
			//-----Set RAM area-----------------------
			//0x0060, 0xA700,		//GS=1
			0x0060, 0x2700,		//GS=1
			0x0061, 0x0001,
			0x006A, 0x0000,
			0x0021, 0x0000,
			0x0020, 0x0000,
			//-----Partial Display Control------------
			0x0080, 0x0000,
			0x0081, 0x0000,
			0x0082, 0x0000,
			0x0083, 0x0000,
			0x0084, 0x0000,
			0x0085, 0x0000,
			//-----Panel Control----------------------
			0x0090, 0x0010,
			0x0092, 0x0000,
			0x0093, 0x0003,
			0x0095, 0x0110,
			0x0097, 0x0000,
			0x0098, 0x0000,
			//-----Display on-----------------------
			0x0007, 0x0173,
			TFTLCD_DELAY, 50,
		};
		init_table16(ILI9320_regValues, sizeof(ILI9320_regValues));
		_interface = 0x9325;

	// ILI9325
	} else if (_model == 0x9325) {
		static const uint16_t ILI9325_regValues[] = {
			0x00E5, 0x78F0,		// set SRAM internal timing
			0x0001, 0x0100,		// set Driver Output Control
			0x0002, 0x0200,		// set 1 line inversion
			0x0003, 0x1030,		// set GRAM write direction and BGR=1.
			0x0004, 0x0000,		// Resize register
			0x0005, 0x0000,		// .kbv 16bits Data Format Selection
			0x0008, 0x0207,		// set the back porch and front porch
			0x0009, 0x0000,		// set non-display area refresh cycle ISC[3:0]
			0x000A, 0x0000,		// FMARK function
			0x000C, 0x0000,		// RGB interface setting
			0x000D, 0x0000,		// Frame marker Position
			0x000F, 0x0000,		// RGB interface polarity
			// ----------- Power On sequence ----------- //
			0x0010, 0x0000,		// SAP, BT[3:0], AP, DSTB, SLP, STB
			0x0011, 0x0007,		// DC1[2:0], DC0[2:0], VC[2:0]
			0x0012, 0x0000,		// VREG1OUT voltage
			0x0013, 0x0000,		// VDV[4:0] for VCOM amplitude
			0x0007, 0x0001,
			TFTLCD_DELAY, 200,	// Dis-charge capacitor power voltage
			0x0010, 0x1690,		// SAP=1, BT=6, APE=1, AP=1, DSTB=0, SLP=0, STB=0
			0x0011, 0x0227,		// DC1=2, DC0=2, VC=7
			TFTLCD_DELAY, 50,	// wait_ms 50ms
			0x0012, 0x000D,		// VCIRE=1, PON=0, VRH=5
			TFTLCD_DELAY, 50,	// wait_ms 50ms
			0x0013, 0x1200,		// VDV=28 for VCOM amplitude
			0x0029, 0x000A,		// VCM=10 for VCOMH
			0x002B, 0x000D,		// Set Frame Rate
			TFTLCD_DELAY, 50,	// wait_ms 50ms
			0x0020, 0x0000,		// GRAM horizontal Address
			0x0021, 0x0000,		// GRAM Vertical Address
			// ----------- Adjust the Gamma Curve ----------//

			0x0030, 0x0000,
			0x0031, 0x0404,
			0x0032, 0x0003,
			0x0035, 0x0405,
			0x0036, 0x0808,
			0x0037, 0x0407,
			0x0038, 0x0303,
			0x0039, 0x0707,
			0x003C, 0x0504,
			0x003D, 0x0808,

			//------------------ Set GRAM area ---------------//
			//0x0060, 0x2700,	  // Gate Scan Line GS=0 [0xA700] 
			0x0060, 0xA700,		// Gate Scan Line GS=0 [0xA700] 
			0x0061, 0x0001,		// NDL,VLE, REV .kbv
			0x006A, 0x0000,		// set scrolling line
			//-------------- Partial Display Control ---------//
			0x0080, 0x0000,
			0x0081, 0x0000,
			0x0082, 0x0000,
			0x0083, 0x0000,
			0x0084, 0x0000,
			0x0085, 0x0000,
			//-------------- Panel Control -------------------//
			0x0090, 0x0010,
			0x0092, 0x0000,
			0x0007, 0x0133,		// 262K color and display ON
		};
		init_table16(ILI9325_regValues, sizeof(ILI9325_regValues));
		 _interface = 0x9325;

	// ILI9327
	} else if (_model == 0x9327) {
		static const uint8_t regValues[] = {
			//0x36, 1, 0x08,
			0x36, 1, 0x88,
			0x3A, 1, 0x05,
			0xC0, 6, 0x00, 0x35, 0x00, 0x00, 0x01, 0x02,
			0xC1, 4, 0x10, 0x10, 0x02, 0x02,
			0xC5, 1, 0x04,
			0xC8,15, 0x04, 0x67, 0x35, 0x04, 0x08, 0x06, 0x24, 0x01, 0x37, 0x40, 0x03, 0x10, 0x08, 0x80, 0x00,
			0xD0, 3, 0x07, 0x04, 0x00,
			0xD1, 3, 0x00, 0x0C, 0x0F,
			0xD2, 2, 0x01, 0x44,
		};
		init_table(reset_off, sizeof(reset_off));
		init_table(regValues, sizeof(regValues));
		init_table(wake_on, sizeof(wake_on));
		_interface = 0x9341;

	// ILI9341
	} else if (_model == 0x9341) {
		static const uint8_t regValues[] = {
			0xC0, 1, 0x23,
			0xC1, 1, 0x10,
			0xC5, 2, 0x3E, 0x28,
			0xC7, 1, 0x86,
			0x36, 1, 0xC8,
			0xB1, 2, 0x00, 0x18,
			0xB6, 4, 0x0A, 0xA2, 0x27, 0x04,
			0x26, 1, 0x01,
			0xE0,15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
			0xE1,15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
		};
		init_table(reset_off, sizeof(reset_off));
		init_table(regValues, sizeof(regValues));
		init_table(wake_on, sizeof(wake_on));
		_interface = 0x9341;

	// ILI9342
	} else if (_model == 0x9342) {
		static const uint8_t regValues[] = {
			0xC0, 2, 0x26, 0x09,
			0xC1, 1, 0x10,
			0xC5, 2, 0x3E, 0x28,
			0xC7, 1, 0x86,
			0x36, 1, 0x68,
			0xB1, 2, 0x00, 0x18,
			0xB6, 4, 0x0A, 0xA2, 0x27, 0x04,
			0x21, 0,	// Display Inversion ON
			0x26, 1, 0x01,
			0xE0,15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
			0xE1,15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
		};
		init_table(reset_off, sizeof(reset_off));
		init_table(regValues, sizeof(regValues));
		init_table(wake_on, sizeof(wake_on));
		_interface = 0x9341;

	// ILI9481
	} else if (_model == 0x9481) {
		static const uint8_t regValues[] = {
			0xB0, 1, 0x00,				// unlocks E0, F0
			0xB3, 4, 0x02, 0x00, 0x00, 0x00, //Frame Memory, interface [02 00 00 00]
			0xB4, 1, 0x00,				// Frame mode [00]
			0xD0, 3, 0x07, 0x42, 0x18,
			0xD1, 3, 0x00, 0x07, 0x18,
			0xD2, 2, 0x01, 0x02,
			0xD3, 2, 0x01, 0x02,		// Set Power for Partial Mode [01 22]
			0xD4, 2, 0x01, 0x02,		// Set Power for Idle Mode [01 22]
			//0xC0, 5, 0x10, 0x3B, 0x00, 0x02, 0x11,
			0xC0, 5, 0x14, 0x3B, 0x00, 0x02, 0x11,
			0xC1, 3, 0x10, 0x10, 0x88,	// Display Timing Normal [10 10 88]
			0xC5, 1, 0x03,		//Frame Rate [03]
			0xC6, 1, 0x02,		//Interface Control [02]
			0xC8, 12, 0x00, 0x32, 0x36, 0x45, 0x06, 0x16, 0x37, 0x75, 0x77, 0x54, 0x0C, 0x00,
			0xCC, 1, 0x00,		//Panel Control [00]
			0x36, 1, 0x18, //0x08,
		};
		init_table(reset_off, sizeof(reset_off));
		init_table(regValues, sizeof(regValues));
		init_table(wake_on, sizeof(wake_on));
		_interface = 0x9341;

	// ILI9486
	} else if (_model == 0x9486) {
		static const uint8_t regValues[] = {
			0xB0, 1, 0x00,				// unlocks E0, F0
			0xB3, 4, 0x02, 0x00, 0x00, 0x00, //Frame Memory, interface [02 00 00 00]
			0xB4, 1, 0x00,				// Frame mode [00]
			//0xB6, 3, 0x02, 0x02, 0x3B,  // Display Function Control [02 02 3B]
			0xB6, 3, 0x02, 0x42, 0x3B,	// Display Function Control [02 02 3B]
			0xD0, 3, 0x07, 0x42, 0x18,
			0xD1, 3, 0x00, 0x07, 0x18,
			0xD2, 2, 0x01, 0x02,
			0xD3, 2, 0x01, 0x02,		// Set Power for Partial Mode [01 22]
			0xD4, 2, 0x01, 0x02,		// Set Power for Idle Mode [01 22]
			//0xC0, 5, 0x10, 0x3B, 0x00, 0x02, 0x11,
			0xC0, 5, 0x14, 0x3B, 0x00, 0x02, 0x11,
			0xC1, 3, 0x10, 0x10, 0x88,	// Display Timing Normal [10 10 88]
			0xC5, 1, 0x03,		//Frame Rate [03]
			0xC6, 1, 0x02,		//Interface Control [02]
			0xC8, 12, 0x00, 0x32, 0x36, 0x45, 0x06, 0x16, 0x37, 0x75, 0x77, 0x54, 0x0C, 0x00,
			0xCC, 1, 0x00,		//Panel Control [00]
			0x36, 1, 0x18, //0x08,
		};
		init_table(reset_off, sizeof(reset_off));
		init_table(regValues, sizeof(regValues));
		init_table(wake_on, sizeof(wake_on));
		_interface = 0x9341;

	// ILI9488
	} else if (_model == 0x9488) {
		static const uint8_t regValues[] = {
			0xC0, 2, 0x10, 0x10,		//Power Control 1 [0E 0E]
			0xC1, 1, 0x41,		//Power Control 2 [43]
			0xC5, 4, 0x00, 0x22, 0x80, 0x40,	//VCOM	Control 1 [00 40 00 40]
			//0x36, 1, 0x68,	  //Memory Access [00]
			0x36, 1, 0x98,		//Memory Access [00]
			0xB0, 1, 0x00,		//Interface		[00]
			0xB1, 2, 0xB0, 0x11,		//Frame Rate Control [B0 11]
			0xB4, 1, 0x02,		//Inversion Control [02]
			0xB6, 3, 0x02, 0x02, 0x3B,	// Display Function Control [02 02 3B] .kbv NL=480
			0xB7, 1, 0xC6,		//Entry Mode	  [06]
			0x3A, 1, 0x55,		//Interlace Pixel Format [XX]
			0xF7, 4, 0xA9, 0x51, 0x2C, 0x82,	//Adjustment Control 3 [A9 51 2C 82]
		};
		init_table(reset_off, sizeof(reset_off));
		init_table(regValues, sizeof(regValues));
		init_table(wake_on, sizeof(wake_on));
		_interface = 0x9341;

	// S6D1121
	} else if (_model == 0x1121) {
		static const uint16_t S6D1121_regValues[] = {
			0x0011, 0x2004,
			0x0013, 0xCC00,
			0x0015, 0x2600,
			0x0014, 0x252A,
			0x0012, 0x0033,
			0x0013, 0xCC04,
			0x0013, 0xCC06,
			0x0013, 0xCC4F,
			0x0013, 0x674F,
			0x0011, 0x2003,
			0x0030, 0x2609,
			0x0031, 0x242C,
			0x0032, 0x1F23,
			0x0033, 0x2425,
			0x0034, 0x2226,
			0x0035, 0x2523,
			0x0036, 0x1C1A,
			0x0037, 0x131D,
			0x0038, 0x0B11,
			0x0039, 0x1210,
			0x003A, 0x1315,
			0x003B, 0x3619,
			0x003C, 0x0D00,
			0x003D, 0x000D,
			0x0016, 0x0007,
			0x0002, 0x0013,
			0x0001, 0x0127,
			//0x0001, 0x0027,
			0x0003, 0x0003,
			//0x0003, 0x0000,
			0x0008, 0x0303,
			0x000A, 0x000B,
			0x000B, 0x0003,
			0x000C, 0x0000,
			0x0041, 0x0000,
			0x0050, 0x0000,
			0x0060, 0x0005,
			0x0070, 0x000B,
			0x0071, 0x0000,
			0x0078, 0x0000,
			0x007A, 0x0000,
			0x0079, 0x0007,
			0x0007, 0x0051,
			0x0007, 0x0053,
			0x0079, 0x0000,
		};
		init_table16(S6D1121_regValues, sizeof(S6D1121_regValues));
		_interface = 0x1121;

	// R61509V
	} else if (_model == 0xB509) {
		static const uint16_t R61509_regValues[] = {
			//0x0400, 0x6200,
			0x0008, 0x0808,
			0x0010, 0x0016,		 //69.5Hz		  0016
			0x0011, 0x0101,
			0x0012, 0x0000,
			0x0013, 0x0001,
			0x0100, 0x0330,		 //BT,AP
			0x0101, 0x0237,		 //DC0,DC1,VC
			0x0103, 0x0D00,		 //VDV
			0x0280, 0x6100,		 //VCM
			0x0102, 0xC1B0,		 //VRH,VCMR,PSON,PON
			TFTLCD_DELAY16,50,
			//	 0x0001, 0x0100,
			0x0001, 0x0000,
			0x0002, 0x0100,
			0x0003, 0x1030,		  //1030
			0x0009, 0x0001,
			0x000C, 0x0000,
			0x0090, 0x8000,
			0x000F, 0x0000,
			0x0210, 0x0000,
			0x0211, 0x00EF,
			0x0212, 0x0000,
			0x0213, 0x018F,		  //432=01AF,400=018F
			0x0500, 0x0000,
			0x0501, 0x0000,
			0x0502, 0x005F,
			//0x0400, 0x6200,
			0x0400, 0xE200,
			0x0401, 0x0001,
			0x0404, 0x0000,
			TFTLCD_DELAY16,50,
			0x0007, 0x0100,			//BASEE
			TFTLCD_DELAY16,50,
			0x0200, 0x0000,
			0x0201, 0x0000,
		};
		init_table16(R61509_regValues, sizeof(R61509_regValues));
		_interface = 0xB509;

	// ST7781
	} else if (_model == 0x7781) {
		static const uint16_t ST7781_regValues[] = {
			0x00FF, 0x0001,		//can we do 0xFF
			0x00F3, 0x0008,
			0x0000, 0x0001,
			0x0001, 0x0000,		// Driver Output Control Register (R01h)
			0x0002, 0x0700,		// LCD Driving Waveform Control (R02h)
			0x0003, 0x1030,		// Entry Mode (R03h)
			//0x0003, 0x1000,	  // Entry Mode (R03h)
			0x0008, 0x0302,
			0x0009, 0x0000,
			0x0010, 0x0000,		// Power Control 1 (R10h)
			0x0011, 0x0007,		// Power Control 2 (R11h)
			0x0012, 0x0000,		// Power Control 3 (R12h)
			0x0013, 0x0000,		// Power Control 4 (R13h)
			TFTLCD_DELAY16,50,
			0x0010, 0x14B0,		// Power Control 1 SAP=1, BT=4, APE=1, AP=3
			TFTLCD_DELAY16,10,
			0x0011, 0x0007,		// Power Control 2 VC=7
			TFTLCD_DELAY16,10,
			0x0012, 0x008E,		// Power Control 3 VCIRE=1, VRH=14
			0x0013, 0x0C00,		// Power Control 4 VDV=12
			0x0029, 0x0015,		// NVM read data 2 VCM=21
			TFTLCD_DELAY16,10,
			0x0030, 0x0000,		// Gamma Control 1
			0x0031, 0x0107,		// Gamma Control 2
			0x0032, 0x0000,		// Gamma Control 3
			0x0035, 0x0203,		// Gamma Control 6
			0x0036, 0x0402,		// Gamma Control 7
			0x0037, 0x0000,		// Gamma Control 8
			0x0038, 0x0207,		// Gamma Control 9
			0x0039, 0x0000,		// Gamma Control 10
			0x003C, 0x0203,		// Gamma Control 13
			0x003D, 0x0403,		// Gamma Control 14
			//0x0060, 0xA700,	  // Driver Output Control (R60h) .kbv was 0xa700
			0x0060, 0x2700,		// Driver Output Control (R60h) .kbv was 0xa700
			0x0061, 0x0001,		// Driver Output Control (R61h)
			0x0090, 0X0029,		// Panel Interface Control 1 (R90h)
			0x0007, 0x0133,		// Display Control (R07h)
		};
		init_table16(ST7781_regValues, sizeof(ST7781_regValues));
		 _interface = 0x9325;

	// LGDP4532
	} else if (_model == 0x4532) {
		static const uint16_t LGDP4532_regValues[] = {
			0x0000,0x0001, //Device code read
			0x0010,0x0628, //Power control 1 SAP[2:0] BT[3:0] AP[2:0] DK DSTB SLP
			0x0012,0x0006, //Power control 3 PON VRH[3:0]
			//0x0013,0x0A32, //Power control 4 VCOMG VDV[4:0] VCM[6:0]
			0x0011,0x0040, //Power control 2; DC1[2:0] DC0[2:0] VC[2:0]
			//0x0015,0x0050, //Regulator control RSET RI[2:0] RV[2:0] RCONT[2:0]
			0x0012,0x0016, //Power control 3 PON VRH[3:0]
			TFTLCD_DELAY16,50,
			0x0010,0x5660, //Power control 1 SAP[2:0] BT[3:0] AP[2:0] DK DSTB SLP
			TFTLCD_DELAY16,50,
			//0x0013,0x2A4E, //Power control 4 VCOMG VDV[4:0] VCM[6:0]
			//0x0001,0x0100, //Driver output control SM SS
			//0x0002,0x0300, //LCD Driving Wave Control
			0x0003,0x1030, //Entry mode TRI DFM  BGR  ORG I/D[1:0] AM
			//0x0003,0x1020, //Entry mode TRI DFM  BGR	ORG I/D[1:0] AM
			//0x0003,0x1010, //Entry mode TRI DFM  BGR	ORG I/D[1:0] AM
			//0x0003,0x1000, //Entry mode TRI DFM  BGR	ORG I/D[1:0] AM
			//0x0007,0x0202, //Display Control 1 PTDE[1:0] BASEE GON DTE COL D[1:0]
			TFTLCD_DELAY16,50,
			//0x0008,0x0202, //Display Control 2 FP[3:0] BP[3:0] front and back porch (blank period at begin and end..)
			//0x000A,0x0000, //Test Register 1 (RA0h)
			//Gamma adjustment
			0x0030,0x0000,
			0x0031,0x0402,
			0x0032,0x0106,
			0x0033,0x0700,
			0x0034,0x0104,
			0x0035,0x0301,
			0x0036,0x0707,
			0x0037,0x0305,
			0x0038,0x0208,
			0x0039,0x0F0B,
			TFTLCD_DELAY16,50,
			//some of this stuff in range 41-93 really throws things off....
			//0x0041,0x0002,
			//0x0060,0x2700, //Driver Output Control (R60h)
			0x0060,0xA700, //Driver Output Control (R60h)
			0x0061,0x0001, //Base Image Display Control (R61h)
			//0x0090,0x0119,   //Panel Interface Control 1 (R90h) DIVI[1:0]  RTNI[4:0]
			//0x0092,0x010A,  //Panel Interface Control 2 (R92h)  NOWI[2:0] EQI2[1:0] EQI1[1:0]
			//0x0093,0x0004, //Panel Interface Control 3 (R93h) MCPI[2:0]
			//0x00A0,0x0100, //Test Register 1 (RA0h)
			TFTLCD_DELAY16,50,
			0x0007,0x0133, //Display Control 1 PTDE[1:0] BASEE GON DTE COL D[1:0]
			TFTLCD_DELAY16,50,
			//0x00A0,0x0000, //Test Register 1 (RA0h)
		};
		init_table16(LGDP4532_regValues, sizeof(LGDP4532_regValues));
		_interface = 0x9325;

	}
}


// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color
void lcdDrawPixel(uint16_t x, uint16_t y, uint16_t color) {
	if (x >= _width) return;
	if (y >= _height) return;

	uint16_t _x = x + _offsetx;
	uint16_t _y = y + _offsety;

	if (_interface == 0x9325) {
		lcdWriteRegisterWord(0x0020,_x); // RAM Address Set 1
		lcdWriteRegisterWord(0x0021,_y); // RAM Address Set 2
		lcdWriteRegisterWord(0x0022,color); // Write Data to GRAM

	} else if (_interface == 0x9341) {
		lcdWriteCommandByte(0x2A); // set column(x) address
		lcdWriteDataWord(_x);
		lcdWriteDataWord(_x);
		lcdWriteCommandByte(0x2B); // set Page(y) address
		lcdWriteDataWord(_y);
		lcdWriteDataWord(_y);
		lcdWriteCommandByte(0x2C); // Memory Write
		lcdWriteDataWord(color);

	} else if (_interface == 0x1121) {
		lcdWriteRegisterWord(0x0046,(_x << 8) | _x);
		lcdWriteRegisterWord(0x0047,_y);
		lcdWriteRegisterWord(0x0048,_y);
		lcdWriteRegisterWord(0x0020,_x); // RAM Address Set 1
		lcdWriteRegisterWord(0x0021,_y); // RAM Address Set 2
		lcdWriteRegisterWord(0x0022,color); // Write Data to GRAM

	} else if (_interface == 0xB509) {
		lcdWriteRegisterWord(0x0200,_x); // RAM Address Set 1
		lcdWriteRegisterWord(0x0201,_y); // RAM Address Set 2
		lcdWriteRegisterWord(0x0202,color); // Write Data to GRAM
	}
}

// Draw rectangule of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void lcdDrawFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	if (x1 >= _width) return;
	if (x2 >= _width) x2=_width-1;
	if (y1 >= _height) return;
	if (y2 >= _height) y2=_height-1;

	uint16_t _x1 = x1 + _offsetx;
	uint16_t _x2 = x2 + _offsetx;
	uint16_t _y1 = y1 + _offsety;
	uint16_t _y2 = y2 + _offsety;

	if (_interface == 0x9325) {
		for(int _y=_y1;_y<=_y2;_y++){
			lcdWriteRegisterWord(0x0020,_x1); // RAM Address Set 1
			lcdWriteRegisterWord(0x0021,_y); // RAM Address Set 2
			lcdWriteCommandWord(0x0022); // Write Data to GRAM
			for(int _x=_x1;_x<=_x2;_x++){
				lcdWriteDataWord(color); // Write Data to GRAM
			}
		}

	} else if (_interface == 0x9341) {
		lcdWriteCommandByte(0x2A); // set column(x) address
		lcdWriteDataWord(_x1);
		lcdWriteDataWord(_x2);
		lcdWriteCommandByte(0x2B); // set Page(y) address
		lcdWriteDataWord(_y1);
		lcdWriteDataWord(_y2);
		lcdWriteCommandByte(0x2C); // Memory Write
		for(int _x=_x1;_x<=_x2;_x++){
			for(int _y=_y1;_y<=_y2;_y++){
			 lcdWriteDataWord(color);
			}
		}

	} else if (_interface == 0x1121) {
		for(int _y=_y1;_y<=_y2;_y++){
			lcdWriteRegisterWord(0x0046,(_x2 << 8) | _x1);
			lcdWriteRegisterWord(0x0047,_y);
			lcdWriteRegisterWord(0x0048,_y);
			lcdWriteRegisterWord(0x0020,_x1); // RAM Address Set 1
			lcdWriteRegisterWord(0x0021,_y); // RAM Address Set 2
			lcdWriteCommandWord(0x0022); // Write Data to GRAM
			for(int _x=_x1;_x<=_x2;_x++){
				lcdWriteDataWord(color); // Write Data to GRAM
			}
		}

	} else if (_interface == 0xB509) {
		for(int _x=_x1;_x<=_x2;_x++){
			for(int _y=_y1;_y<=_y2;_y++){
				lcdWriteRegisterWord(0x0200,_x);
				lcdWriteRegisterWord(0x0201,_y);
				lcdWriteRegisterWord(0x0202,color);
			}
		}
	}

}

// Display Off
void lcdDisplayOff(void) {
	if (_interface == 0x9325) {
		lcdWriteRegisterWord(0x0007, 0x0000); // Set GON=0 DTE=0 D1=0, D0=0

	} else if (_interface == 0x9341) {
		lcdWriteCommandByte(0x28); 

	} else if (_interface == 0x1121) {
		lcdWriteRegisterWord(0x0007, 0x0000); // Set GON=0 DTE=0 D1=0, D0=0

	} else if (_interface == 0xB509) {
		lcdWriteRegisterWord(0x0007, 0x0000);					//BASEE
	}
}

// Display On
void lcdDisplayOn(void) {
	if (_interface == 0x9325) {
		lcdWriteRegisterWord(0x0007, 0x0173); // Set GON=1 DTE=1 D1=1, D0=1

	} else if (_interface == 0x9341) {
		lcdWriteCommandByte(0x29); 

	} else if (_interface == 0x1121) {
		lcdWriteRegisterWord(0x0007, 0x0173); // Set GON=1 DTE=1 D1=1, D0=1

	} else if (_interface == 0xB509) {
		lcdWriteRegisterWord(0x0007, 0x0100);					//BASEE
	}
}

// Fill screen
// color:color
void lcdFillScreen(uint16_t color) {
	lcdDrawFillRect(0, 0, _width-1, _height-1, color);
}

// Draw line
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color 
void lcdDrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	int i;
	int dx,dy;
	int sx,sy;
	int E;

	/* distance between two points */
	dx = ( x2 > x1 ) ? x2 - x1 : x1 - x2;
	dy = ( y2 > y1 ) ? y2 - y1 : y1 - y2;

	/* direction of two point */
	sx = ( x2 > x1 ) ? 1 : -1;
	sy = ( y2 > y1 ) ? 1 : -1;

	/* inclination < 1 */
	if ( dx > dy ) {
		E = -dx;
		for ( i = 0 ; i <= dx ; i++ ) {
			lcdDrawPixel( x1, y1, color );
			x1 += sx;
			E += 2 * dy;
			if ( E >= 0 ) {
				y1 += sy;
				E -= 2 * dx;
			}
		}
	/* inclination >= 1 */
	} else {
		E = -dy;
		for ( i = 0 ; i <= dy ; i++ ) {
			lcdDrawPixel( x1, y1, color );
			y1 += sy;
			E += 2 * dx;
			if ( E >= 0 ) {
				x1 += sx;
				E -= 2 * dy;
			}
		}
	}
}

// Draw rectangule
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End		X coordinate
// y2:End		Y coordinate
// color:color
void lcdDrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	lcdDrawLine(x1,y1,x2,y1,color);
	lcdDrawLine(x2,y1,x2,y2,color);
	lcdDrawLine(x2,y2,x1,y2,color);
	lcdDrawLine(x1,y2,x1,y1,color);
}

// Draw round
// x0:Central X coordinate
// y0:Central Y coordinate
// r:radius
// color:color
void lcdDrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
	int x;
	int y;
	int err;
	int old_err;
	
	x=0;
	y=-r;
	err=2-2*r;
	do{
		lcdDrawPixel(x0-x,y0+y,color); 
		lcdDrawPixel(x0-y,y0-x,color); 
		lcdDrawPixel(x0+x,y0-y,color); 
		lcdDrawPixel(x0+y,y0+x,color); 
		if ((old_err=err)<=x)		err+=++x*2+1;
		if (old_err>y || err>x) err+=++y*2+1;		 
	} while(y<0);
	
}

// Draw round of filling
// x0:Central X coordinate
// y0:Central Y coordinate
// r:radius
// color:color
void lcdDrawFillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
	int x;
	int y;
	int err;
	int old_err;
	int ChangeX;
	
	x=0;
	y=-r;
	err=2-2*r;
	ChangeX=1;
	do{
		if(ChangeX) {
			lcdDrawLine(x0-x,y0-y,x0-x,y0+y,color);
			lcdDrawLine(x0+x,y0-y,x0+x,y0+y,color);
		} // if
		ChangeX=(old_err=err)<=x;
		if (ChangeX)						err+=++x*2+1;
		if (old_err>y || err>x) err+=++y*2+1;
	} while(y<=0);
		
} 

// Draw rectangule with round corner
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End		X coordinate
// y2:End		Y coordinate
// r:radius
// color:color
void lcdDrawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color)
{
	int x;
	int y;
	int err;
	int old_err;
	unsigned char temp;

	if(x1>x2) {
		temp=x1; x1=x2; x2=temp;
	} // if
	
	if(y1>y2) {
		temp=y1; y1=y2; y2=temp;
	} // if
	
	
	x=0;
	y=-r;
	err=2-2*r;

	do{
		if(x) {
			lcdDrawPixel(x1+r-x,y1+r+y,color); 
			lcdDrawPixel(x2-r+x,y1+r+y,color); 
			lcdDrawPixel(x1+r-x,y2-r-y,color); 
			lcdDrawPixel(x2-r+x,y2-r-y,color);
		} // if 
		if ((old_err=err)<=x)		err+=++x*2+1;
		if (old_err>y || err>x) err+=++y*2+1;		 
	} while(y<0);
	 
	lcdDrawLine(x1+r,y1  ,x2-r,y1  ,color);
	lcdDrawLine(x1+r,y2  ,x2-r,y2  ,color);
	lcdDrawLine(x1	,y1+r,x1	,y2-r,color);
	lcdDrawLine(x2	,y1+r,x2	,y2-r,color);  
} 

// Draw arrow
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End		X coordinate
// y2:End		Y coordinate
// w:Width of the botom
// color:color
// Thanks http://k-hiura.cocolog-nifty.com/blog/2010/11/post-2a62.html
void lcdDrawArrow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t w,uint16_t color){

	 double Vx= x1 - x0;
	 double Vy= y1 - y0;
	 double v = sqrt(Vx*Vx+Vy*Vy);
//	 printf("v=%f\n",v);
	 double Ux= Vx/v;
	 double Uy= Vy/v;

	 uint16_t L[2],R[2];
	 L[0]= x1 - Uy*w - Ux*v;
	 L[1]= y1 + Ux*w - Uy*v;
	 R[0]= x1 + Uy*w - Ux*v;
	 R[1]= y1 - Ux*w - Uy*v;
//	 printf("L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);

//	 lcdDrawLine(x0,y0,x1,y1,color);
	 lcdDrawLine(x1,y1,L[0],L[1],color);
	 lcdDrawLine(x1,y1,R[0],R[1],color);
	 lcdDrawLine(L[0],L[1],R[0],R[1],color);
}


// Draw arrow of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End		X coordinate
// y2:End		Y coordinate
// w:Width of the botom
// color:color
void lcdDrawFillArrow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t w,uint16_t color){

	 double Vx= x1 - x0;
	 double Vy= y1 - y0;
	 double v = sqrt(Vx*Vx+Vy*Vy);
//	 printf("v=%f\n",v);
	 double Ux= Vx/v;
	 double Uy= Vy/v;

	 uint16_t L[2],R[2];
	 L[0]= x1 - Uy*w - Ux*v;
	 L[1]= y1 + Ux*w - Uy*v;
	 R[0]= x1 + Uy*w - Ux*v;
	 R[1]= y1 - Ux*w - Uy*v;
//	 printf("L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);

	 lcdDrawLine(x0,y0,x1,y1,color);
	 lcdDrawLine(x1,y1,L[0],L[1],color);
	 lcdDrawLine(x1,y1,R[0],R[1],color);
	 lcdDrawLine(L[0],L[1],R[0],R[1],color);

	 int ww;
	 for(ww=w-1;ww>0;ww--) {
		 L[0]= x1 - Uy*ww - Ux*v;
		 L[1]= y1 + Ux*ww - Uy*v;
		 R[0]= x1 + Uy*ww - Ux*v;
		 R[1]= y1 - Ux*ww - Uy*v;
//		 printf("Fill>L=%d-%d R=%d-%d\n",L[0],L[1],R[0],R[1]);
		 lcdDrawLine(x1,y1,L[0],L[1],color);
		 lcdDrawLine(x1,y1,R[0],R[1],color);
	 }
}


// RGB565 conversion
// RGB565 is R(5)+G(6)+B(5)=16bit color format.
// Bit image "RRRRRGGGGGGBBBBB"
uint16_t rgb565_conv(uint16_t r,uint16_t g,uint16_t b)
{
	 unsigned int RR,GG,BB;
	 RR = (r * 31 / 255) << 11;
	 GG = (g * 63 / 255) << 5;
	 BB = (b * 31 / 255);
	 return(RR | GG | BB);
}


// Draw SJIS character
// x:X coordinate
// y:Y coordinate
// sjis: SJIS code
// color:color
int lcdDrawSJISChar(FontxFile *fx, uint16_t x,uint16_t y,uint16_t sjis,uint16_t color) {
	uint16_t xx,yy,bit,ofs;
	unsigned char fonts[128]; // font pattern
	unsigned char pw, ph;
	int h,w;
	uint16_t mask;
	bool rc;
	
if(_DEBUG_)printf("_FONT_DIRECTION_=%d\n",_FONT_DIRECTION_);
//	sjis = UTF2SJIS(utf8);
//if(_DEBUG_)printf("sjis=%04x\n",sjis);

	rc = GetFontx(fx, sjis, fonts, &pw, &ph); // SJIS -> Font pattern
if(_DEBUG_)printf("GetFontx rc=%d pw=%d ph=%d\n",rc,pw,ph);
	if (!rc) return 0;

	int16_t xd1 = 0;
	int16_t yd1 = 0;
	int16_t xd2 = 0;
	int16_t yd2 = 0;
	int16_t xss = 0;
	int16_t yss = 0;
	int16_t xsd = 0;
	int16_t ysd = 0;
	int16_t next = 0;
	int16_t x0	= 0;
	int16_t x1	= 0;
	int16_t y0	= 0;
	int16_t y1	= 0;

	if (_FONT_DIRECTION_ == 0) {
		xd1 = +1;
		yd1 = +1;
		xd2 =  0;
		yd2 =  0;
		xss =  x;
		yss =  y - (ph - 1);
		xsd =  1;
		ysd =  0;
		next = x + pw;

		x0	= x;
		y0	= y - (ph-1);
		x1	= x + (pw-1);
		y1	= y;
	} else if (_FONT_DIRECTION_ == 2) {
		xd1 = -1;
		yd1 = -1;
		xd2 =  0;
		yd2 =  0;
		xss =  x;
		yss =  y + ph + 1;
		xsd =  1;
		ysd =  0;
		next = x - pw;

		x0	= x - (pw-1);
		y0	= y;
		x1	= x;
		y1	= y + (ph-1);
	} else if (_FONT_DIRECTION_ == 1) {
		xd1 =  0;
		yd1 =  0;
		xd2 = -1;
		yd2 = +1;
		xss =  x + ph;
		yss =  y;
		xsd =  0;
		ysd =  1;
		next = y + pw;

		x0	= x;
		y0	= y;
		x1	= x + (ph-1);
		y1	= y + (pw-1);
	} else if (_FONT_DIRECTION_ == 3) {
		xd1 =  0;
		yd1 =  0;
		xd2 = +1;
		yd2 = -1;
		xss =  x - (ph - 1);
		yss =  y;
		xsd =  0;
		ysd =  1;
		next = y - pw;

		x0	= x - (ph-1);
		y0	= y - (pw-1);
		x1	= x;
		y1	= y;
	}

	if (_FONT_FILL_) lcdDrawFillRect(x0, y0, x1, y1, _FONT_FILL_COLOR_);

	int bits;
if(_DEBUG_)printf("xss=%d yss=%d\n",xss,yss);
	ofs = 0;
	yy = yss;
	xx = xss;
	for(h=0;h<ph;h++) {
		if(xsd) xx = xss;
		if(ysd) yy = yss;
		//for(w=0;w<(pw/8);w++) {
		bits = pw;
		for(w=0;w<((pw+4)/8);w++) {
			mask = 0x80;
			for(bit=0;bit<8;bit++) {
				bits--;
				if (bits < 0) continue;
//if(_DEBUG_)printf("xx=%d yy=%d mask=%02x fonts[%d]=%02x\n",xx,yy,mask,ofs,fonts[ofs]);
				if (fonts[ofs] & mask) {
					lcdDrawPixel(xx,yy,color);
#if 0
				} else {
					if (_FONT_FILL_) lcdDrawPixel(xx,yy,_FONT_FILL_COLOR_);
#endif
				}
				if (h == (ph-2) && _FONT_UNDER_LINE_)
					lcdDrawPixel(xx,yy,_FONT_UNDER_LINE_COLOR_);
				if (h == (ph-1) && _FONT_UNDER_LINE_)
					lcdDrawPixel(xx,yy,_FONT_UNDER_LINE_COLOR_);

				xx = xx + xd1;
				yy = yy + yd2;
				mask = mask >> 1;
			}
			ofs++;
		}
		yy = yy + yd1;
		xx = xx + xd2;

	}
	
	if (next < 0) next = 0;
	return next;
}

// Draw UTF8 character
// x:X coordinate
// y:Y coordinate
// utf8: UTF8 code
// color:color
int lcdDrawUTF8Char(FontxFile *fx, uint16_t x,uint16_t y,uint8_t *utf8,uint16_t color) {
	uint16_t sjis[1];
	
	sjis[0] = UTF2SJIS(utf8);
if(_DEBUG_)printf("sjis=%04x\n",sjis[0]);
	return lcdDrawSJISChar(fx,x,y,sjis[0],color);
}

// Draw UTF8 string
// x:X coordinate
// y:Y coordinate
// utfs: UTF8 string
// color:color
int lcdDrawUTF8String(FontxFile *fx, uint16_t x,uint16_t y,unsigned char *utfs,uint16_t color) {

	int i;
	int spos;
	uint16_t sjis[64];
	spos = String2SJIS(utfs, strlen((char *)utfs), sjis, 64);
if(_DEBUG_)printf("spos=%d\n",spos);
	for(i=0;i<spos;i++) {
if(_DEBUG_)printf("sjis[%d]=%x y=%d\n",i,sjis[i],y);
		if (_FONT_DIRECTION_ == 0)
			x=lcdDrawSJISChar(fx,x,y,sjis[i],color);
		if (_FONT_DIRECTION_ == 1)
			y=lcdDrawSJISChar(fx,x,y,sjis[i],color);
		if (_FONT_DIRECTION_ == 2)
			x=lcdDrawSJISChar(fx,x,y,sjis[i],color);
		if (_FONT_DIRECTION_ == 3)
			y=lcdDrawSJISChar(fx,x,y,sjis[i],color);
	}
	if (_FONT_DIRECTION_ == 0) return x;
	if (_FONT_DIRECTION_ == 2) return x;
	if (_FONT_DIRECTION_ == 1) return y;
	if (_FONT_DIRECTION_ == 3) return y;
	return 0;
}

// Set font direction
// dir:Direction
void lcdSetFontDirection(uint16_t dir) {
	_FONT_DIRECTION_ = dir;
}

// Set font filling
// color:fill color
void lcdSetFontFill(uint16_t color) {
	_FONT_FILL_ = true;
	_FONT_FILL_COLOR_ = color;
}

// UnSet font filling
void lcdUnsetFontFill(void) {
	_FONT_FILL_ = false;
}

// Set font underline
// color:frame color
void lcdSetFontUnderLine(uint16_t color) {
	_FONT_UNDER_LINE_ = true;
	_FONT_UNDER_LINE_COLOR_ = color;
}

// UnSet font underline
void lcdUnsetFontUnderLine(void) {
	_FONT_UNDER_LINE_ = false;
}


