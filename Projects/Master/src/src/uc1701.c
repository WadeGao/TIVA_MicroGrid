#include "uc1701.h"
#include "hw_uc1701.h"

#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"

#include "driverlib/sysctl.h"
#include "driverlib/rom_map.h"

#include "driverlib/gpio.h"
#include "driverlib/ssi.h"

#ifndef USE_U8G_LIB

#ifdef BUILT_IN_FONT
/* 大学计划德研电子科技温度转换蜂鸣器串口音频 */

#ifdef BUILT_IN_SIMSUN
const unsigned short SimSun_Index[] = {
/*		index   decimal  char */
/*		=====   =======  ==== */
      0xAEB4, /* 20018    串  */
      0xAEBB, /* 21010    划  */
      0xDABF, /* 21475    口  */
      0xF7C6, /* 22120    器  */
      0xF3B4, /* 22823    大  */
      0xD3D7, /* 23376    子  */
      0xA7D1, /* 23398    学  */
      0xC8B6, /* 24230    度  */
      0xC2B5, /* 24503    德  */
      0xBCBC, /* 25216    技  */
      0xBBBB, /* 25442    换  */
      0xC2CE, /* 28201    温  */
      0xE7B5, /* 30005    电  */
      0xD0D1, /* 30740    研  */
      0xC6BF, /* 31185    科  */
      0xE4B7, /* 34562    蜂  */
      0xC6BC, /* 35745    计  */
      0xAAD7, /* 36716    转  */
      0xF4D2, /* 38899    音  */
      0xB5C6, /* 39057    频  */
      0xF9C3, /* 40483    鸣  */
};

const unsigned char SimSun_Font[] = {
	/* character 0x4E32 ('串'): (width=16, offset=0) */
	0x00, 0x00, 0x3C, 0x24, 0x24, 0x24, 0x24, 0xFF, 
	0x24, 0x24, 0x24, 0x24, 0x3C, 0x00, 0x00, 0x00, 
	0x00, 0x1F, 0x09, 0x09, 0x09, 0x09, 0x09, 0xFF, 
	0x09, 0x09, 0x09, 0x09, 0x09, 0x1F, 0x00, 0x00, 

	/* character 0x5212 ('划'): (width=16, offset=32) */
	0x20, 0x20, 0x20, 0xFF, 0x20, 0x22, 0x14, 0x90, 
	0x10, 0x00, 0xF8, 0x00, 0x00, 0xFF, 0x00, 0x00, 
	0x40, 0x40, 0x20, 0x13, 0x0C, 0x14, 0x22, 0x41, 
	0xF8, 0x00, 0x0F, 0x40, 0x80, 0x7F, 0x00, 0x00, 

	/* character 0x53E3 ('口'): (width=16, offset=64) */
	0x00, 0x00, 0xFC, 0x04, 0x04, 0x04, 0x04, 0x04, 
	0x04, 0x04, 0x04, 0x04, 0xFC, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x7F, 0x20, 0x20, 0x20, 0x20, 0x20, 
	0x20, 0x20, 0x20, 0x20, 0x7F, 0x00, 0x00, 0x00, 

	/* character 0x5668 ('器'): (width=16, offset=96) */
	0x80, 0x80, 0x9E, 0x92, 0x92, 0x92, 0x9E, 0xE0, 
	0x80, 0x9E, 0xB2, 0xD2, 0x92, 0x9E, 0x80, 0x00, 
	0x08, 0x08, 0xF4, 0x94, 0x92, 0x92, 0xF1, 0x00, 
	0x01, 0xF2, 0x92, 0x94, 0x94, 0xF8, 0x08, 0x00, 

	/* character 0x5927 ('大'): (width=16, offset=128) */
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xFF, 
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 
	0x80, 0x80, 0x40, 0x20, 0x10, 0x0C, 0x03, 0x00, 
	0x03, 0x0C, 0x10, 0x20, 0x40, 0x80, 0x80, 0x00, 

	/* character 0x5B50 ('子'): (width=16, offset=160) */
	0x80, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0xE2, 
	0xA2, 0x92, 0x8A, 0x86, 0x82, 0x80, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x80, 0x7F, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	/* character 0x5B66 ('学'): (width=16, offset=192) */
	0x40, 0x30, 0x11, 0x96, 0x90, 0x90, 0x91, 0x96, 
	0x90, 0x90, 0x98, 0x14, 0x13, 0x50, 0x30, 0x00, 
	0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x84, 0x7E, 
	0x06, 0x05, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 

	/* character 0x5EA6 ('度'): (width=16, offset=224) */
	0x00, 0x00, 0xFC, 0x24, 0x24, 0x24, 0xFC, 0x25, 
	0x26, 0x24, 0xFC, 0x24, 0x24, 0x24, 0x04, 0x00, 
	0x40, 0x30, 0x8F, 0x80, 0x84, 0x4C, 0x55, 0x25, 
	0x25, 0x25, 0x55, 0x4C, 0x80, 0x80, 0x80, 0x00, 

	/* character 0x5FB7 ('德'): (width=16, offset=256) */
	0x10, 0x88, 0xC4, 0x33, 0x04, 0xF4, 0x94, 0x94, 
	0xF4, 0x9F, 0xF4, 0x94, 0x94, 0xF4, 0x04, 0x00, 
	0x01, 0x00, 0xFF, 0x00, 0x42, 0x32, 0x02, 0x72, 
	0x82, 0x86, 0x9A, 0x82, 0xE2, 0x0A, 0x32, 0x00, 

	/* character 0x6280 ('技'): (width=16, offset=0) */
	0x10, 0x10, 0x10, 0xFF, 0x10, 0x90, 0x08, 0x88, 
	0x88, 0x88, 0xFF, 0x88, 0x88, 0x88, 0x08, 0x00, 
	0x04, 0x44, 0x82, 0x7F, 0x01, 0x80, 0x80, 0x40, 
	0x43, 0x2C, 0x10, 0x28, 0x46, 0x81, 0x80, 0x00, 

	/* character 0x6362 ('换'): (width=16, offset=32) */
	0x10, 0x10, 0x10, 0xFF, 0x90, 0x20, 0x10, 0xE8, 
	0x27, 0x24, 0xE4, 0x34, 0x2C, 0xE0, 0x00, 0x00, 
	0x02, 0x42, 0x81, 0x7F, 0x00, 0x84, 0x84, 0x47, 
	0x24, 0x1C, 0x07, 0x1C, 0x24, 0x47, 0x84, 0x00, 

	/* character 0x6E29 ('温'): (width=16, offset=64) */
	0x10, 0x60, 0x02, 0x8C, 0x00, 0x00, 0xFE, 0x92, 
	0x92, 0x92, 0x92, 0x92, 0xFE, 0x00, 0x00, 0x00, 
	0x04, 0x04, 0x7E, 0x01, 0x40, 0x7E, 0x42, 0x42, 
	0x7E, 0x42, 0x7E, 0x42, 0x42, 0x7E, 0x40, 0x00, 

	/* character 0x7535 ('电'): (width=16, offset=96) */
	0x00, 0x00, 0xF8, 0x88, 0x88, 0x88, 0x88, 0xFF, 
	0x88, 0x88, 0x88, 0x88, 0xF8, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x1F, 0x08, 0x08, 0x08, 0x08, 0x7F, 
	0x88, 0x88, 0x88, 0x88, 0x9F, 0x80, 0xF0, 0x00, 

	/* character 0x7814 ('研'): (width=16, offset=128) */
	0x04, 0x84, 0xE4, 0x5C, 0x44, 0xC4, 0x80, 0x82, 
	0xFE, 0x82, 0x82, 0x82, 0xFE, 0x82, 0x80, 0x00, 
	0x02, 0x01, 0x7F, 0x10, 0x10, 0x3F, 0x80, 0x60, 
	0x1F, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 

	/* character 0x79D1 ('科'): (width=16, offset=160) */
	0x24, 0x24, 0xA4, 0xFE, 0xA3, 0x22, 0x00, 0x22, 
	0xCC, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x06, 0x01, 0xFF, 0x00, 0x01, 0x04, 0x04, 
	0x04, 0x04, 0x04, 0xFF, 0x02, 0x02, 0x02, 0x00, 

	/* character 0x8702 ('蜂'): (width=16, offset=192) */
	0x00, 0xF8, 0x08, 0xFF, 0x08, 0xF8, 0x80, 0x90, 
	0x4C, 0x57, 0xA4, 0x54, 0x4C, 0x84, 0x80, 0x00, 
	0x20, 0x63, 0x21, 0x1F, 0x11, 0x39, 0x10, 0x10, 
	0x15, 0x15, 0xFF, 0x15, 0x15, 0x10, 0x10, 0x00, 

	/* character 0x8BA1 ('计'): (width=16, offset=224) */
	0x40, 0x40, 0x42, 0xCC, 0x00, 0x40, 0x40, 0x40, 
	0x40, 0xFF, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 
	0x00, 0x00, 0x00, 0x7F, 0x20, 0x10, 0x00, 0x00, 
	0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

	/* character 0x8F6C ('转'): (width=16, offset=256) */
	0xC8, 0xB8, 0x8F, 0xE8, 0x88, 0x88, 0x40, 0x48, 
	0x48, 0xE8, 0x5F, 0x48, 0x48, 0x48, 0x40, 0x00, 
	0x08, 0x18, 0x08, 0xFF, 0x04, 0x04, 0x00, 0x02, 
	0x0B, 0x12, 0x22, 0xD2, 0x0A, 0x06, 0x00, 0x00, 

	/* character 0x97F3 ('音'): (width=16, offset=0) */
	0x40, 0x40, 0x44, 0x44, 0x54, 0x64, 0x45, 0x46, 
	0x44, 0x64, 0x54, 0x44, 0x44, 0x40, 0x40, 0x00, 
	0x00, 0x00, 0x00, 0xFF, 0x49, 0x49, 0x49, 0x49, 
	0x49, 0x49, 0x49, 0xFF, 0x00, 0x00, 0x00, 0x00, 

	/* character 0x9891 ('频'): (width=16, offset=32) */
	0x40, 0x7C, 0x40, 0x7F, 0x48, 0x48, 0x40, 0xF2, 
	0x12, 0x1A, 0xD6, 0x12, 0x12, 0xF2, 0x02, 0x00, 
	0x90, 0x8E, 0x40, 0x4F, 0x20, 0x1E, 0x80, 0x4F, 
	0x20, 0x18, 0x07, 0x10, 0x20, 0x4F, 0x80, 0x00, 

	/* character 0x9E23 ('鸣'): (width=16, offset=64) */
	0x00, 0xFC, 0x04, 0x04, 0xFC, 0x00, 0x00, 0xFC, 
	0x04, 0x16, 0x65, 0x04, 0x84, 0xFC, 0x00, 0x00, 
	0x00, 0x0F, 0x04, 0x04, 0x0F, 0x00, 0x10, 0x13, 
	0x12, 0x12, 0x12, 0x52, 0x92, 0x42, 0x3E, 0x00, 
};
#endif // BUILT_IN_SIMSUN

unsigned char FixedSys_Font[] = {
    /* character 0x0020 (' '): (width=8, offset=0) */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    /* character 0x0021 ('!'): (width=8, offset=16) */
    0x00, 0x00, 0xE0, 0xF0, 0xF0, 0xE0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x1B, 0x1B, 0x00, 0x00, 0x00, 

    /* character 0x0022 ('"'): (width=8, offset=32) */
    0x00, 0x70, 0x70, 0x00, 0x00, 0x70, 0x70, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    /* character 0x0023 ('#'): (width=8, offset=48) */
    0x00, 0x40, 0xF0, 0xF0, 0x40, 0xF0, 0xF0, 0x40, 
    0x00, 0x04, 0x1F, 0x1F, 0x04, 0x1F, 0x1F, 0x04, 

    /* character 0x0024 ('$'): (width=8, offset=64) */
    0x00, 0x60, 0xF0, 0x9C, 0x1C, 0x30, 0x20, 0x00, 
    0x00, 0x08, 0x18, 0x71, 0x73, 0x1E, 0x0C, 0x00, 

    /* character 0x0025 ('%'): (width=8, offset=80) */
    0x30, 0x78, 0x48, 0x78, 0xB0, 0xC0, 0x60, 0x00, 
    0x00, 0x0C, 0x06, 0x1B, 0x3D, 0x24, 0x3C, 0x18, 

    /* character 0x0026 ('&'): (width=8, offset=96) */
    0x00, 0x60, 0xF0, 0x90, 0xF0, 0x60, 0x00, 0x00, 
    0x00, 0x0F, 0x1F, 0x10, 0x12, 0x0E, 0x1E, 0x12, 

    /* character 0x0027 ('''): (width=8, offset=112) */
    0x00, 0x00, 0x00, 0x70, 0x70, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    /* character 0x0028 ('('): (width=8, offset=128) */
    0x00, 0x00, 0x80, 0xE0, 0x70, 0x10, 0x00, 0x00, 
    0x00, 0x00, 0x0F, 0x3F, 0x70, 0x40, 0x00, 0x00, 

    /* character 0x0029 (')'): (width=8, offset=0) */
    0x00, 0x00, 0x10, 0x70, 0xE0, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x40, 0x70, 0x3F, 0x0F, 0x00, 0x00, 

    /* character 0x002A ('*'): (width=8, offset=16) */
    0x00, 0x00, 0x40, 0xC0, 0x80, 0xC0, 0x40, 0x00, 
    0x00, 0x01, 0x05, 0x07, 0x03, 0x07, 0x05, 0x01, 

    /* character 0x002B ('+'): (width=8, offset=32) */
    0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0x01, 0x07, 0x07, 0x01, 0x01, 0x00, 

    /* character 0x002C (','): (width=8, offset=48) */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x58, 0x78, 0x38, 0x00, 0x00, 

    /* character 0x002D ('-'): (width=8, offset=64) */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 

    /* character 0x002E ('.'): (width=8, offset=80) */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 

    /* character 0x002F ('/'): (width=8, offset=96) */
    0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0x30, 0x00, 
    0x00, 0x30, 0x3C, 0x0F, 0x03, 0x00, 0x00, 0x00, 

    /* character 0x0030 ('0'): (width=8, offset=256) */
    0x00, 0xE0, 0xF0, 0x10, 0xD0, 0xF0, 0xE0, 0x00, 
    0x00, 0x0F, 0x1F, 0x16, 0x10, 0x1F, 0x0F, 0x00, 

    /* character 0x0031 ('1'): (width=8, offset=272) */
    0x00, 0x40, 0x40, 0x60, 0xF0, 0xF0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x00, 0x00, 

    /* character 0x0032 ('2'): (width=8, offset=288) */
    0x00, 0x60, 0x70, 0x10, 0x10, 0xF0, 0xE0, 0x00, 
    0x00, 0x18, 0x1C, 0x16, 0x13, 0x11, 0x10, 0x00, 

    /* character 0x0033 ('3'): (width=8, offset=304) */
    0x00, 0x60, 0x70, 0x10, 0x10, 0xF0, 0xE0, 0x00, 
    0x00, 0x0C, 0x1C, 0x11, 0x11, 0x1F, 0x0E, 0x00, 

    /* character 0x0034 ('4'): (width=8, offset=320) */
    0x00, 0x00, 0xF0, 0xF0, 0x00, 0xC0, 0xC0, 0x00, 
    0x00, 0x06, 0x07, 0x05, 0x04, 0x1F, 0x1F, 0x04, 

    /* character 0x0035 ('5'): (width=8, offset=336) */
    0x00, 0xF0, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x00, 
    0x00, 0x11, 0x11, 0x11, 0x19, 0x0F, 0x06, 0x00, 

    /* character 0x0036 ('6'): (width=8, offset=352) */
    0x00, 0x80, 0xC0, 0xF0, 0xB0, 0x90, 0x00, 0x00, 
    0x00, 0x0F, 0x1F, 0x10, 0x10, 0x1F, 0x0F, 0x00, 

    /* character 0x0037 ('7'): (width=8, offset=368) */
    0x00, 0x10, 0x10, 0x10, 0xD0, 0xF0, 0x30, 0x00, 
    0x00, 0x00, 0x1C, 0x1F, 0x03, 0x00, 0x00, 0x00, 

    /* character 0x0038 ('8'): (width=8, offset=0) */
    0x00, 0xE0, 0xF0, 0x90, 0x10, 0xF0, 0xE0, 0x00, 
    0x00, 0x0E, 0x1F, 0x11, 0x13, 0x1F, 0x0E, 0x00, 

    /* character 0x0039 ('9'): (width=8, offset=16) */
    0x00, 0xE0, 0xF0, 0x10, 0x10, 0xF0, 0xE0, 0x00, 
    0x00, 0x01, 0x13, 0x1A, 0x1E, 0x07, 0x03, 0x00, 

    /* character 0x003A (':'): (width=8, offset=32) */
    0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 

    /* character 0x003B (';'): (width=8, offset=48) */
    0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x58, 0x78, 0x38, 0x00, 0x00, 

    /* character 0x003C ('<'): (width=8, offset=64) */
    0x00, 0x00, 0x80, 0xC0, 0x60, 0x30, 0x10, 0x00, 
    0x00, 0x01, 0x03, 0x06, 0x0C, 0x18, 0x10, 0x00, 

    /* character 0x003D ('='): (width=8, offset=80) */
    0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 
    0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 

    /* character 0x003E ('>'): (width=8, offset=96) */
    0x00, 0x10, 0x30, 0x60, 0xC0, 0x80, 0x00, 0x00, 
    0x00, 0x10, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00, 

    /* character 0x003F ('?'): (width=8, offset=112) */
    0x00, 0x60, 0x70, 0x10, 0x90, 0xF0, 0x60, 0x00, 
    0x00, 0x00, 0x00, 0x1B, 0x1B, 0x00, 0x00, 0x00, 

    /* character 0x0040 ('@'): (width=8, offset=128) */
    0xE0, 0xF0, 0x10, 0x10, 0x90, 0x90, 0xF0, 0xE0, 
    0x0F, 0x1F, 0x10, 0x13, 0x17, 0x14, 0x17, 0x17, 

    /* character 0x0041 ('A'): (width=8, offset=528) */
    0x00, 0xC0, 0xE0, 0x30, 0x30, 0xE0, 0xC0, 0x00, 
    0x00, 0x1F, 0x1F, 0x02, 0x02, 0x1F, 0x1F, 0x00, 

    /* character 0x0042 ('B'): (width=8, offset=544) */
    0x00, 0xF0, 0xF0, 0x10, 0x10, 0xF0, 0xE0, 0x00, 
    0x00, 0x1F, 0x1F, 0x11, 0x11, 0x1F, 0x0E, 0x00, 

    /* character 0x0043 ('C'): (width=8, offset=560) */
    0x00, 0xE0, 0xF0, 0x10, 0x10, 0x70, 0x60, 0x00, 
    0x00, 0x0F, 0x1F, 0x10, 0x10, 0x1C, 0x0C, 0x00, 

    /* character 0x0044 ('D'): (width=8, offset=576) */
    0x00, 0xF0, 0xF0, 0x10, 0x30, 0xE0, 0xC0, 0x00, 
    0x00, 0x1F, 0x1F, 0x10, 0x18, 0x0F, 0x07, 0x00, 

    /* character 0x0045 ('E'): (width=8, offset=592) */
    0x00, 0xF0, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x00, 
    0x00, 0x1F, 0x1F, 0x11, 0x11, 0x11, 0x10, 0x00, 

    /* character 0x0046 ('F'): (width=8, offset=608) */
    0x00, 0xF0, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x00, 
    0x00, 0x1F, 0x1F, 0x01, 0x01, 0x01, 0x00, 0x00, 

    /* character 0x0047 ('G'): (width=8, offset=624) */
    0x00, 0xE0, 0xF0, 0x10, 0x10, 0x70, 0x60, 0x00, 
    0x00, 0x0F, 0x1F, 0x10, 0x12, 0x1E, 0x1E, 0x00, 

    /* character 0x0048 ('H'): (width=8, offset=640) */
    0x00, 0xF0, 0xF0, 0x00, 0x00, 0xF0, 0xF0, 0x00, 
    0x00, 0x1F, 0x1F, 0x01, 0x01, 0x1F, 0x1F, 0x00, 

    /* character 0x0049 ('I'): (width=8, offset=656) */
    0x00, 0x00, 0x10, 0xF0, 0xF0, 0x10, 0x00, 0x00, 
    0x00, 0x00, 0x10, 0x1F, 0x1F, 0x10, 0x00, 0x00, 

    /* character 0x004A ('J'): (width=8, offset=672) */
    0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0x00, 
    0x00, 0x0C, 0x1C, 0x10, 0x10, 0x1F, 0x0F, 0x00, 

    /* character 0x004B ('K'): (width=8, offset=688) */
    0x00, 0xF0, 0xF0, 0x00, 0xC0, 0xF0, 0x30, 0x00, 
    0x00, 0x1F, 0x1F, 0x01, 0x07, 0x1E, 0x18, 0x00, 

    /* character 0x004C ('L'): (width=8, offset=704) */
    0x00, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x1F, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x00, 

    /* character 0x004D ('M'): (width=8, offset=720) */
    0x00, 0xF0, 0xF0, 0x40, 0x80, 0x40, 0xF0, 0xF0, 
    0x00, 0x1F, 0x1F, 0x00, 0x03, 0x00, 0x1F, 0x1F, 

    /* character 0x004E ('N'): (width=8, offset=736) */
    0x00, 0xF0, 0xF0, 0xC0, 0x80, 0x00, 0xF0, 0xF0, 
    0x00, 0x1F, 0x1F, 0x00, 0x01, 0x03, 0x1F, 0x1F, 

    /* character 0x004F ('O'): (width=8, offset=0) */
    0x00, 0xE0, 0xF0, 0x10, 0x10, 0xF0, 0xE0, 0x00, 
    0x00, 0x0F, 0x1F, 0x10, 0x10, 0x1F, 0x0F, 0x00, 

    /* character 0x0050 ('P'): (width=8, offset=16) */
    0x00, 0xF0, 0xF0, 0x10, 0x10, 0xF0, 0xE0, 0x00, 
    0x00, 0x1F, 0x1F, 0x01, 0x01, 0x01, 0x00, 0x00, 

    /* character 0x0051 ('Q'): (width=8, offset=32) */
    0x00, 0xE0, 0xF0, 0x10, 0x10, 0xF0, 0xE0, 0x00, 
    0x00, 0x0F, 0x1F, 0x10, 0x30, 0x7F, 0x4F, 0x00, 

    /* character 0x0052 ('R'): (width=8, offset=48) */
    0x00, 0xF0, 0xF0, 0x10, 0x10, 0xF0, 0xE0, 0x00, 
    0x00, 0x1F, 0x1F, 0x01, 0x03, 0x1F, 0x1C, 0x00, 

    /* character 0x0053 ('S'): (width=8, offset=64) */
    0x00, 0x60, 0xF0, 0x90, 0x10, 0x30, 0x20, 0x00, 
    0x00, 0x08, 0x18, 0x11, 0x13, 0x1E, 0x0C, 0x00, 

    /* character 0x0054 ('T'): (width=8, offset=80) */
    0x00, 0x10, 0x10, 0xF0, 0xF0, 0x10, 0x10, 0x00, 
    0x00, 0x00, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x00, 

    /* character 0x0055 ('U'): (width=8, offset=96) */
    0x00, 0xF0, 0xF0, 0x00, 0x00, 0xF0, 0xF0, 0x00, 
    0x00, 0x0F, 0x1F, 0x10, 0x10, 0x1F, 0x0F, 0x00, 

    /* character 0x0056 ('V'): (width=8, offset=112) */
    0x00, 0xF0, 0xF0, 0x00, 0x00, 0xF0, 0xF0, 0x00, 
    0x00, 0x07, 0x0F, 0x18, 0x18, 0x0F, 0x07, 0x00, 

    /* character 0x0057 ('W'): (width=8, offset=128) */
    0x00, 0xF0, 0xF0, 0x00, 0x80, 0x00, 0xF0, 0xF0, 
    0x00, 0x03, 0x1F, 0x1C, 0x03, 0x1C, 0x1F, 0x03, 

    /* character 0x0058 ('X'): (width=8, offset=0) */
    0x00, 0x30, 0x70, 0xC0, 0x80, 0x70, 0x30, 0x00, 
    0x00, 0x1C, 0x1E, 0x01, 0x03, 0x1E, 0x1C, 0x00, 

    /* character 0x0059 ('Y'): (width=8, offset=16) */
    0x00, 0xF0, 0xF0, 0x00, 0x00, 0xF0, 0xF0, 0x00, 
    0x00, 0x00, 0x01, 0x1F, 0x1F, 0x01, 0x00, 0x00, 

    /* character 0x005A ('Z'): (width=8, offset=32) */
    0x00, 0x10, 0x10, 0x10, 0x90, 0xF0, 0x70, 0x00, 
    0x00, 0x1C, 0x1E, 0x13, 0x11, 0x10, 0x10, 0x00, 

    /* character 0x005B ('['): (width=8, offset=48) */
    0x00, 0x00, 0xF0, 0xF0, 0x10, 0x10, 0x00, 0x00, 
    0x00, 0x00, 0xFF, 0xFF, 0x80, 0x80, 0x00, 0x00, 

    /* character 0x005C ('\'): (width=8, offset=64) */
    0x00, 0x30, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x03, 0x0F, 0x3C, 0x30, 0x00, 

    /* character 0x005D (']'): (width=8, offset=80) */
    0x00, 0x00, 0x10, 0x10, 0xF0, 0xF0, 0x00, 0x00, 
    0x00, 0x00, 0x80, 0x80, 0xFF, 0xFF, 0x00, 0x00, 

    /* character 0x005E ('^'): (width=8, offset=96) */
    0x00, 0x10, 0x18, 0x0C, 0x0C, 0x18, 0x10, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    /* character 0x005F ('_'): (width=8, offset=112) */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 

    /* character 0x0060 ('`'): (width=8, offset=128) */
    0x00, 0x00, 0x04, 0x0C, 0x1C, 0x10, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    /* character 0x0061 ('a'): (width=8, offset=1040) */
    0x00, 0x00, 0x40, 0x40, 0x40, 0xC0, 0x80, 0x00, 
    0x00, 0x0C, 0x1E, 0x12, 0x12, 0x1F, 0x1F, 0x00, 

    /* character 0x0062 ('b'): (width=8, offset=1056) */
    0x00, 0xF0, 0xF0, 0x40, 0x40, 0xC0, 0x80, 0x00, 
    0x00, 0x1F, 0x1F, 0x10, 0x10, 0x1F, 0x0F, 0x00, 

    /* character 0x0063 ('c'): (width=8, offset=1072) */
    0x00, 0x80, 0xC0, 0x40, 0x40, 0xC0, 0x80, 0x00, 
    0x00, 0x0F, 0x1F, 0x10, 0x10, 0x18, 0x08, 0x00, 

    /* character 0x0064 ('d'): (width=8, offset=1088) */
    0x00, 0x80, 0xC0, 0x40, 0x40, 0xF0, 0xF0, 0x00, 
    0x00, 0x0F, 0x1F, 0x10, 0x10, 0x1F, 0x1F, 0x00, 

    /* character 0x0065 ('e'): (width=8, offset=1104) */
    0x00, 0x80, 0xC0, 0x40, 0x40, 0xC0, 0x80, 0x00, 
    0x00, 0x0F, 0x1F, 0x12, 0x12, 0x13, 0x03, 0x00, 

    /* character 0x0066 ('f'): (width=8, offset=1120) */
    0x00, 0x00, 0xE0, 0xF0, 0x10, 0x10, 0x10, 0x00, 
    0x00, 0x01, 0x1F, 0x1F, 0x01, 0x01, 0x01, 0x00, 

    /* character 0x0067 ('g'): (width=8, offset=1136) */
    0x00, 0x80, 0xC0, 0x40, 0x40, 0xC0, 0xC0, 0x00, 
    0x00, 0x8F, 0x9F, 0x90, 0x90, 0xFF, 0x7F, 0x00, 

    /* character 0x0068 ('h'): (width=8, offset=0) */
    0x00, 0xF0, 0xF0, 0x40, 0x40, 0xC0, 0x80, 0x00, 
    0x00, 0x1F, 0x1F, 0x00, 0x00, 0x1F, 0x1F, 0x00, 

    /* character 0x0069 ('i'): (width=8, offset=16) */
    0x00, 0x40, 0x40, 0xD8, 0xD8, 0x00, 0x00, 0x00, 
    0x00, 0x10, 0x10, 0x1F, 0x1F, 0x10, 0x10, 0x00, 

    /* character 0x006A ('j'): (width=8, offset=32) */
    0x00, 0x00, 0x40, 0x40, 0xD8, 0xD8, 0x00, 0x00, 
    0x00, 0x80, 0x80, 0x80, 0xFF, 0x7F, 0x00, 0x00, 

    /* character 0x006B ('k'): (width=8, offset=48) */
    0x00, 0xF0, 0xF0, 0x00, 0x00, 0xC0, 0xC0, 0x00, 
    0x00, 0x1F, 0x1F, 0x02, 0x07, 0x1D, 0x18, 0x00, 

    /* character 0x006C ('l'): (width=8, offset=64) */
    0x00, 0x10, 0x10, 0xF0, 0xF0, 0x00, 0x00, 0x00, 
    0x00, 0x10, 0x10, 0x1F, 0x1F, 0x10, 0x10, 0x00, 

    /* character 0x006D ('m'): (width=8, offset=80) */
    0x00, 0xC0, 0xC0, 0x40, 0xC0, 0x40, 0xC0, 0x80, 
    0x00, 0x1F, 0x1F, 0x00, 0x0F, 0x00, 0x1F, 0x1F, 

    /* character 0x006E ('n'): (width=8, offset=96) */
    0x00, 0xC0, 0xC0, 0x40, 0x40, 0xC0, 0x80, 0x00, 
    0x00, 0x1F, 0x1F, 0x00, 0x00, 0x1F, 0x1F, 0x00, 

    /* character 0x006F ('o'): (width=8, offset=112) */
    0x00, 0x80, 0xC0, 0x40, 0x40, 0xC0, 0x80, 0x00, 
    0x00, 0x0F, 0x1F, 0x10, 0x10, 0x1F, 0x0F, 0x00, 

    /* character 0x0070 ('p'): (width=8, offset=128) */
    0x00, 0xC0, 0xC0, 0x40, 0x40, 0xC0, 0x80, 0x00, 
    0x00, 0xFF, 0xFF, 0x10, 0x10, 0x1F, 0x0F, 0x00, 

    /* character 0x0071 ('q'): (width=8, offset=0) */
    0x00, 0x80, 0xC0, 0x40, 0x40, 0xC0, 0xC0, 0x00, 
    0x00, 0x0F, 0x1F, 0x10, 0x10, 0xFF, 0xFF, 0x00, 

    /* character 0x0072 ('r'): (width=8, offset=16) */
    0x00, 0xC0, 0xC0, 0x00, 0x80, 0xC0, 0xC0, 0x00, 
    0x00, 0x1F, 0x1F, 0x01, 0x00, 0x00, 0x00, 0x00, 

    /* character 0x0073 ('s'): (width=8, offset=32) */
    0x00, 0x80, 0xC0, 0x40, 0x40, 0x40, 0x40, 0x00, 
    0x00, 0x11, 0x13, 0x12, 0x12, 0x1E, 0x0C, 0x00, 

    /* character 0x0074 ('t'): (width=8, offset=48) */
    0x00, 0x40, 0xF0, 0xF0, 0x40, 0x40, 0x40, 0x00, 
    0x00, 0x00, 0x0F, 0x1F, 0x10, 0x10, 0x10, 0x00, 

    /* character 0x0075 ('u'): (width=8, offset=64) */
    0x00, 0xC0, 0xC0, 0x00, 0x00, 0xC0, 0xC0, 0x00, 
    0x00, 0x0F, 0x1F, 0x10, 0x10, 0x1F, 0x1F, 0x00, 

    /* character 0x0076 ('v'): (width=8, offset=80) */
    0x00, 0xC0, 0xC0, 0x00, 0x00, 0xC0, 0xC0, 0x00, 
    0x00, 0x07, 0x0F, 0x18, 0x18, 0x0F, 0x07, 0x00, 

    /* character 0x0077 ('w'): (width=8, offset=96) */
    0x00, 0xC0, 0xC0, 0x00, 0x80, 0x00, 0xC0, 0xC0, 
    0x00, 0x07, 0x1F, 0x18, 0x07, 0x18, 0x1F, 0x07, 

    /* character 0x0078 ('x'): (width=8, offset=112) */
    0x00, 0xC0, 0xC0, 0x00, 0x00, 0xC0, 0xC0, 0x00, 
    0x00, 0x18, 0x1D, 0x07, 0x07, 0x1D, 0x18, 0x00, 

    /* character 0x0079 ('y'): (width=8, offset=128) */
    0x00, 0xC0, 0xC0, 0x00, 0x00, 0xC0, 0xC0, 0x00, 
    0x80, 0x8F, 0x9F, 0xD0, 0x70, 0x3F, 0x0F, 0x00, 

    /* character 0x007A ('z'): (width=8, offset=0) */
    0x00, 0x40, 0x40, 0x40, 0x40, 0xC0, 0xC0, 0x00, 
    0x00, 0x18, 0x1C, 0x16, 0x13, 0x11, 0x10, 0x00, 

    /* character 0x007B ('{'): (width=8, offset=16) */
    0x00, 0x00, 0x00, 0xE0, 0xF0, 0x10, 0x00, 0x00, 
    0x00, 0x02, 0x07, 0x3D, 0x78, 0x40, 0x00, 0x00, 

    /* character 0x007C ('|'): (width=8, offset=32) */
    0x00, 0x00, 0x00, 0xF0, 0xF0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 

    /* character 0x007D ('}'): (width=8, offset=48) */
    0x00, 0x00, 0x10, 0xF0, 0xE0, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x40, 0x78, 0x3D, 0x07, 0x02, 0x00, 

    /* character 0x007E ('~'): (width=8, offset=64) */
    0x60, 0x30, 0x10, 0x30, 0x60, 0x40, 0x60, 0x30, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    /* character 0x007F (''): (width=8, offset=80) */
    0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 
    0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, 
};
#endif // BUILT_IN_FONT

uint8_t inverse_factor = 0; // xor factor

#ifdef UC1701_USE_DMA
uint8_t display_buffer[UC1701_BUFFER_SIZE];
#endif // UC1701_USE_DMA

static void delay_ms(long ms)
{
    SysCtlDelay(SysCtlClockGet() * ms / (1000 * 3));
}

void UC1701Init(unsigned long ulSpiClock)
{
    //
    // Enable the GPIOx port which is connected with UC1701
    //
    //MAP_SysCtlPeripheralEnable(LCD_PERIPH_BKL);
    MAP_SysCtlPeripheralEnable(LCD_PERIPH_SPI_CS);
    MAP_SysCtlPeripheralEnable(LCD_PERIPH_CD);
#ifndef USE_DY_PB_2
    MAP_SysCtlPeripheralEnable(LCD_PERIPH_RESET);
#endif // USE_DY_PB_2
    
    //
    // Enable the SPIx which is connected with UC1701
    //
    MAP_GPIOPinTypeGPIOOutput(LCD_GPIO_SPI_CS, LCD_PIN_SPI_CS);     // PB2 - CS
    
    //
    // Set the chip select/reset pin as OUT_MODE
    //
    //MAP_GPIOPinTypeGPIOOutput(LCD_GPIO_BKL  , LCD_PIN_BKL);         // PA5 - BKL
    MAP_GPIOPinTypeGPIOOutput(LCD_GPIO_CD, LCD_PIN_CD);          // PD2 - CD
    
#ifdef DY_1_5V_FIX
    MAP_GPIOPadConfigSet(LCD_GPIO_CD, LCD_PIN_CD, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    
    MAP_GPIOPinWrite(LCD_GPIO_CD, LCD_PIN_CD, LCD_PIN_CD);
    MAP_GPIOPinWrite(LCD_GPIO_CD, LCD_PIN_CD, ~LCD_PIN_CD);
#endif // DY_1_5V_FIX

#ifndef USE_DY_PB_2
    MAP_GPIOPinTypeGPIOOutput(LCD_GPIO_RESET, LCD_PIN_RESET);       // PE0 - RST
#endif // USE_DY_PB_2
    
    //
    // Configure MCU as a master device, 8 bits data width, MSB first, Mode_3
    //
    MAP_SSIConfigSetExpClk(LCD_PIN_SPI_PORT, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,
        SSI_MODE_MASTER, ulSpiClock, 8);
    MAP_SSIEnable(LCD_PIN_SPI_PORT);
    
    //
    // Disable UC1701 when Power up
    //
    MAP_GPIOPinWrite(LCD_GPIO_SPI_CS, LCD_PIN_SPI_CS, LCD_PIN_SPI_CS);  // CS
   
    // Power on the bkl
    //MAP_GPIOPinWrite(LCD_GPIO_BKL, LCD_PIN_BKL, LCD_PIN_BKL);           // BKL

#ifndef USE_DY_PB_2
    //
    // Reset the chip
    //
    MAP_GPIOPinWrite(LCD_GPIO_RESET, LCD_PIN_RESET, ~LCD_PIN_RESET);    // RST
    
    delay_ms(2);    // wait > 1ms
    
    //xGPIOSPinWrite(LCD_PIN_RESET, 1);
    MAP_GPIOPinWrite(LCD_GPIO_RESET, LCD_PIN_RESET, LCD_PIN_RESET);     // RST
#endif // USE_DY_PB_2
    
    delay_ms(6);    // wait > 5ms

#if 0
    UC1701CmdWrite(0x0e2);      /* soft reset */
    UC1701CmdWrite(0x040);      /* set display start line to 0 */
    UC1701CmdWrite(0x0a0);      /* ADC set to reverse */
    UC1701CmdWrite(0x0c8);      /* common output mode */
    UC1701CmdWrite(0x0a1);      /* display normal, bit val 0: LCD pixel off. */
    UC1701CmdWrite(0x0a2);      /* LCD bias 1/9 */
    UC1701CmdWrite(0x02f);      /* all power  control circuits on */
    UC1701CmdWrite(0x0f8);      /* set booster ratio to */
    UC1701CmdWrite(0x000);      /* 4x */
    UC1701CmdWrite(0x023);      /* set V0 voltage resistor ratio to large */
    UC1701CmdWrite(0x081);      /* set contrast */
    UC1701CmdWrite(0x027);      /* contrast value */
    UC1701CmdWrite(0x0ac);      /* indicator */
    UC1701CmdWrite(0x000);      /* disable */
    UC1701CmdWrite(0x0af);      /* display on */
#else
    // issue commands
    UC1701CmdWrite(0xE2);   // system reset
    delay_ms(200);

    UC1701CmdWrite(0xFA);   // set adv program control
    UC1701CmdWrite(0x93);
      
    UC1701CmdWrite(0xA1);   // set seg direction
    
    UC1701CmdWrite(0xC8);   // set com direction
    
    UC1701CmdWrite(0x2F);   // set power control ?!

    UC1701CmdWrite(0xA2);   // set lcd bias ratio
    
    UC1701CmdWrite(0x81);   // set electronic volume
    UC1701CmdWrite(0x1F);

    UC1701CmdWrite(0xAF);   // set display enable
#endif
}

void UC1701DataWrite(unsigned char ucData)
{
#ifdef UC1701_USE_DMA
    
#else // UC1701_USE_DMA
    uint32_t ulTemp;
    
    //GPIOPinConfigure(SDC_RX_GPIO_CONFIG);              // switch rx pin to ssi mode
    //MAP_GPIOPinTypeGPIOOutput(LCD_GPIO_CD, LCD_PIN_CD);          // PD2 - CD

    //
    //Step 1 Select Chip
    //
    //xGPIOSPinWrite(LCD_PIN_SPI_CS, 0);
    MAP_GPIOPinWrite(LCD_GPIO_SPI_CS, LCD_PIN_SPI_CS, ~LCD_PIN_SPI_CS);  // CS
    
    //
    //Step 2 Set Display Enable
    //
    ulTemp = ucData;
    MAP_GPIOPinWrite(LCD_GPIO_CD, LCD_PIN_CD, LCD_PIN_CD);  // CD
    MAP_SSIDataPut(LCD_PIN_SPI_PORT, ulTemp);
    MAP_SSIDataGet(LCD_PIN_SPI_PORT, &ulTemp); // flush data
    
    // TODO: use isr handle this
    while(MAP_SSIBusy(LCD_PIN_SPI_PORT))
        ;
    
    //
    //Step 3 Disable chip select
    //
    MAP_GPIOPinWrite(LCD_GPIO_SPI_CS, LCD_PIN_SPI_CS, LCD_PIN_SPI_CS);  // CS
#endif // UC1701_USE_DMA
}

void UC1701CmdWrite(unsigned char ucCmd)
{
#ifdef UC1701_USE_DMA
#else // UC1701_USE_DMA
    uint32_t ulTemp; 
    
    //MAP_GPIOPinTypeGPIOOutput(LCD_GPIO_CD, LCD_PIN_CD);          // PD2 - CD
    
    //
    //Step 1 Select Chip
    //
    MAP_GPIOPinWrite(LCD_GPIO_SPI_CS, LCD_PIN_SPI_CS, ~LCD_PIN_SPI_CS);  // CS
    
    //
    //Step 2 Send a command
    //
    ulTemp = ucCmd;
    MAP_GPIOPinWrite(LCD_GPIO_CD, LCD_PIN_CD, ~LCD_PIN_CD);  // CD
    
    MAP_SSIDataPut(LCD_PIN_SPI_PORT, ulTemp);
    MAP_SSIDataGet(LCD_PIN_SPI_PORT, &ulTemp); // flush data
    
    // TODO: use isr handle this
    while(SSIBusy(LCD_PIN_SPI_PORT))
        ;
    
    //
    //Step 3 Disable chip select
    //
    MAP_GPIOPinWrite(LCD_GPIO_SPI_CS, LCD_PIN_SPI_CS, LCD_PIN_SPI_CS);  // CS
#endif // UC1701_USE_DMA
}

void UC1701DoubleCmdWrite(unsigned char ucCmd, unsigned char ucData)
{
    unsigned long ulTemp1, ulTemp2; 
    
    //MAP_GPIOPinTypeGPIOOutput(LCD_GPIO_CD, LCD_PIN_CD);          // PD2 - CD
    
    //
    //Step 1 Select Chip
    //
    MAP_GPIOPinWrite(LCD_GPIO_SPI_CS, LCD_PIN_SPI_CS, ~LCD_PIN_SPI_CS);  // CS
    
    //
    //Step 2 Set command
    //
    ulTemp1 = ucCmd;
    ulTemp2 = ucData;
    MAP_GPIOPinWrite(LCD_GPIO_CD, LCD_PIN_CD, ~LCD_PIN_CD);             // CD
    SSIDataPut(LCD_PIN_SPI_PORT, ulTemp1);
    
    // TODO: use isr handle this
    while(SSIBusy(LCD_PIN_SPI_PORT))
        ;
    
    SSIDataPut(LCD_PIN_SPI_PORT, ulTemp2);
    
    // TODO: use isr handle this
    while(SSIBusy(LCD_PIN_SPI_PORT))
        ;
    
    //
    //Step 3 Disable chip select
    //
    MAP_GPIOPinWrite(LCD_GPIO_SPI_CS, LCD_PIN_SPI_CS, LCD_PIN_SPI_CS);  // CS
}

void UC1701AddressSet(unsigned char ucPA, unsigned char ucCA)
{
#if 1
    ucCA += 2;
#endif
    
    //
    //Step 1 Set Page Address
    //
    UC1701CmdWrite((ucPA & UC1701_SET_PA_MASK) | UC1701_SET_PA);
    
    //
    //Step 2 Set Column Address MSB
    //
    UC1701CmdWrite(((ucCA>>4) & UC1701_SET_CA_MASK) | UC1701_SET_CA_MSB);

    //
    //Step 3 Set Column Address LSB
    //
    UC1701CmdWrite(((ucCA) & UC1701_SET_CA_MASK) | UC1701_SET_CA_LSB);
}

#ifdef BUILT_IN_FONT
void UC1701Display(unsigned char ucLine, unsigned char ucRow, unsigned char ucAsciiWord)
{
    int i=0,k=0;
    unsigned char uctemp;
    k=(ucAsciiWord-32)*16;
    
    for(i=0;i<8;i++)
    {
        UC1701AddressSet((ucLine*2),(129-(ucRow*8)-i));
        uctemp=FixedSys_Font[k+i]^inverse_factor;
        UC1701DataWrite(uctemp);
    }
    for(i=0;i<8;i++)
    {
        UC1701AddressSet((ucLine*2)+1,(129-(ucRow*8)-i));
        uctemp=FixedSys_Font[k+i+8]^inverse_factor;
        UC1701DataWrite(uctemp);
    }
}

void UC1701ChineseDispaly(unsigned char ucLine, unsigned char ucRow, unsigned short ucGB2312Word)
{
#ifdef BUILT_IN_SIMSUN
    int i;
    unsigned char gb2312index;
    
    for (gb2312index = 0; gb2312index < (sizeof(SimSun_Index)/sizeof(unsigned short)); gb2312index ++)
    {
        if (ucGB2312Word == SimSun_Index[gb2312index])
            break;
    }
    
    for(i=0;i<16;i++)
    {
        UC1701AddressSet((ucLine*2),(129-(ucRow*8)-i));
        UC1701DataWrite(SimSun_Font[gb2312index*32+i]^inverse_factor);
    }
    
    for(i=0;i<16;i++)
    {
        UC1701AddressSet((ucLine*2)+1,(129-(ucRow*8)-i));
        UC1701DataWrite(SimSun_Font[gb2312index*32+16+i]^inverse_factor);
    }
#endif // BUILT_IN_SIMSUN
}

void UC1701CharDispaly(unsigned char ucLine, unsigned char ucRow, char *pcChar)
{
    // xASSERT((ucLine <= 3) && (ucRow <= 15));
    do
    {
        UC1701Display(ucLine,ucRow,*pcChar++);
        ucRow++;
        if(ucRow>15)
        {
            ucLine++;
            ucRow=0;
            if(ucLine>3)
                break;
        }
    }
    while(*pcChar!='\0');  
}

void UC1701DisplayN(unsigned char ucLine, unsigned char ucRow, unsigned long n)
{
    char pcBuf[16], *p;

    if (n == 0)
    {
        UC1701Display(ucLine, ucRow, '0');
    }
    else
    {
        p = pcBuf;
        while (n != 0)
        {
            *p++ = (n % 10) + '0';
            n /= 10;
        }

        while (p > pcBuf)
        {
            UC1701Display(ucLine, ucRow, *--p);
            ucRow++;
            if (ucRow > 14)
            {
                ucLine++;
                if(ucLine > 3)
                {
                    break;
                }
            }
        }
    }
}
#endif // BUILT_IN_FONT

// use the data from font chip to paint font
void UC1701FontDisplay(unsigned char ucLine, unsigned char ucRow,
                       unsigned char fontWidth, unsigned char fontHeight,
                       unsigned char *fontBuffer)
{
     int i=0;
     unsigned char uctemp;
 
     for(i=0;i<fontWidth;i++)
     {
         UC1701AddressSet((ucLine*2),(129-(ucRow*8)-i));
         uctemp=(*(fontBuffer+i))^inverse_factor;
         UC1701DataWrite(uctemp);
     }
     for(i=0;i<fontWidth;i++)
     {
         UC1701AddressSet((ucLine*2)+1,(129-(ucRow*8)-i));
         uctemp=(*(fontBuffer+i+16))^inverse_factor;
         UC1701DataWrite(uctemp);
     }
}

void UC1701Clear(void)
{
    unsigned char i,j;

    for(i=0;i<8;i++)
    {
        UC1701CmdWrite(0xb0+i);
        UC1701CmdWrite(0x10);
        UC1701CmdWrite(0x00);
        for(j=0;j<132;j++)
        {
            UC1701DataWrite(0x00);
        }
    }
}

void UC1701InverseEnable(void)
{
#if 0
    uint32_t x,y;
    for(y=0;y<8;y++)
    {    
        UC1701CmdWrite(0xb0+y);
        UC1701CmdWrite(0x10);
        UC1701CmdWrite(0x00);
        for(x=0;x<128;x++) 
        if(x%2==0) UC1701CmdWrite(0); 
        else UC1701CmdWrite(0xFF);
    }

    UC1701CmdWrite(UC1701_SET_DC0_EN);
#else
    inverse_factor = 0xFF;
#endif
}

void UC1701InverseDisable(void)
{
#if 0
    UC1701CmdWrite(UC1701_SET_DC0);
#else
    inverse_factor = 0x0;
#endif
}

void UC1701AllPixelOnEnable(void)
{
    UC1701CmdWrite(UC1701_SET_DC1_EN);
}

void UC1701AllPixelOnDisable(void)
{
    UC1701CmdWrite(UC1701_SET_DC1);
}

void UC1701DisplayOn(void)
{
    UC1701CmdWrite(UC1701_SET_DC2_EN);
}

void UC1701DisplayOff(void)
{
    UC1701CmdWrite(UC1701_SET_DC2);
}

void UC1701ScrollLineSet(unsigned char ucLine)
{
    //xASSERT((ucLine <= 63) && (ucLine >= 0));
    UC1701CmdWrite(UC1701_SET_SL | (ucLine & UC1701_SET_SL_MASK));
}

void UC1701PMSet(unsigned char ucPM)
{
    UC1701DoubleCmdWrite(UC1701_SET_PM, ucPM);
}

void UC1701BRSet(unsigned char ucBR)
{
    UC1701CmdWrite(UC1701_SET_BR_MASK & ucBR);
}

void UC1701SegDirSet(unsigned char ucSegDir)
{
    UC1701CmdWrite(UC1701_SEG_DIR_MASK & ucSegDir);
}

void UC1701ComDirSet(unsigned char ucComDir)
{
    UC1701CmdWrite(UC1701_COM_DIR_MASK & ucComDir);
}

void UC1701Test(void)
{
    int x,y;
    for(y=0;y<8;y++)
    {    
        UC1701CmdWrite(0xb0+y);
        UC1701CmdWrite(0x10);
        UC1701CmdWrite(0x00);
        for(x=0;x<132;x++) 
            UC1701DataWrite(0x55);
    }
}

#endif // USE_U8G_LIB
