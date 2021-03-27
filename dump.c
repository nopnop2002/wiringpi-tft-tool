#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "fontx.h"

int main(int argc, char *argv[]) {
	printf("argc=%d\n",argc);
	if (argc == 1) {
		printf("Usage:%s font_file_path [charcater_code]\n",argv[0]);
		return 1;
	}
	char fontFile[128];
	strcpy(fontFile, argv[1]);
	printf("fontFile=[%s]\n",fontFile);

	FontxFile fxs[2];
	Fontx_init(fxs, "", fontFile);
	if (!Fontx_openFontxFile(&fxs[1])) {
		printf("openFontxFile FAIL\n");
		return 1;
	}

	printf("Font width=%d\n", fxs[1].w);
	printf("Font height=%d\n", fxs[1].h);
	printf("Code flag=%d\n", fxs[1].is_ank);
	printf("Number of code blocks=%d\n", fxs[1].bc);

	uint16_t block[2];
	for(int i=0;i<fxs[1].bc;i++) {
		if(fread(block, 1, 4, fxs[1].file) != 4){
			printf("fread failed\n");
			return 1;
		}
		printf("Block %d start=%x end=%x\n", i, block[0], block[1]);
	}

	if (argc == 3) {
		printf("character code=%s\n", argv[2]);
		long lv = strtol(argv[2], NULL, 16);
		uint16_t code = lv;
		printf("code=%x\n",code);

		unsigned char fonts[128]; // font pattern
		unsigned char pw, ph;
		if (!GetFontx(fxs, code, fonts, &pw, &ph)) { // code -> Font pattern
			printf("GetFontx FAIL. code=%x\n", code);
		} else {
			printf("GetFontx OK. code=%x\n", code);
			ShowFont(fonts, pw, ph);
		}
	}

	fclose(fxs[1].file);
}
