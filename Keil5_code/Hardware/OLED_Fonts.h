#ifndef __OLED_FONT_H
#define __OLED_FONT_H

#include <stdint.h>



/*�����ַ��ֽڿ��*/
#define OLED_CHN_CHAR_WIDTH			(2)		//UTF-8�����ʽ��3��GB2312�����ʽ��2

/*��ģ������Ԫ*/
typedef struct 
{
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//��������
	uint8_t Data[60];						//��ģ����
} ChineseCell20x20_t;
typedef struct 
{
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//��������
	uint8_t Data[32];						//��ģ����
} ChineseCell16x16_t;
typedef struct 
{
    char Index[OLED_CHN_CHAR_WIDTH + 1];   // ��������
    uint8_t Data[24];                      // ��ģ���ݣ�12*12�ĺ�����Ҫ24�ֽ�
} ChineseCell12x12_t;

typedef struct 
{
    char Index[OLED_CHN_CHAR_WIDTH + 1];   // ��������
    uint8_t Data[8];                      // ��ģ���ݣ�8*8�ĺ�����Ҫ24�ֽ�
} ChineseCell8x8_t;

/*ASCII��ģ��������*/
extern const uint8_t OLED_F10x20[][30];
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F7x12[][14];
extern const uint8_t OLED_F6x8[][6];

/*������ģ��������*/
extern const ChineseCell20x20_t OLED_CF20x20[];
extern const ChineseCell16x16_t OLED_CF16x16[];
extern const ChineseCell12x12_t OLED_CF12x12[];  // ����12x12��ģ����
extern const ChineseCell8x8_t OLED_CF8x8[];  // ����12x12��ģ����
/*ͼ����������*/
extern const uint8_t Arrow[];
extern const uint8_t UnKnown[];

extern const uint8_t Image_setings[];
extern const uint8_t Image_QRcode[];
extern const uint8_t Image_calc[];
extern const uint8_t Image_night[];
extern const uint8_t Image_ADDV[];
extern const uint8_t Image_ADDV2[];
extern const uint8_t Image_BIPAN[];
extern const uint8_t Image_GAME[];
extern const uint8_t Image_MUSIC[];
extern const uint8_t Image_VIDEO[];
extern const uint8_t Image_humiture[];
extern const uint8_t Image_flashlight[];
extern const uint8_t Image_second[];

extern const uint8_t OLED_UI_LOGO[];
extern const uint8_t OLED_UI_LOGOTEXT[];
extern const uint8_t OLED_UI_LOGOTEXT64[];
extern const uint8_t OLED_UI_LOGOGithub[];
extern const uint8_t OLED_UI_SettingsLogo[];

extern const uint8_t t1[];
extern const uint8_t t2[];
extern const uint8_t t3[];
extern const uint8_t t4[];
extern const uint8_t t5[];
extern const uint8_t t6[];
extern const uint8_t t7[];
extern const uint8_t t8[];
extern const uint8_t t9[];
extern const uint8_t t0[];

extern const uint8_t i1[];
extern const uint8_t i2[];
extern const uint8_t i3[];
extern const uint8_t i4[];
extern const uint8_t i5[];
extern const uint8_t i6[];
extern const uint8_t i7[];
extern const uint8_t i8[];
extern const uint8_t i9[];
extern const uint8_t i0[];
/*��������ĸ�ʽ�������λ�ü����µ�ͼ����������*/
//...

#endif

